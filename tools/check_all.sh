#!/bin/bash
# Required checks. Fail immediately and retain the failing command's log/status.
# Usage: tools/check_all.sh [--no-walk] [--offline]
# --offline runs build/native/emulator checks without launching the Windows game.
set -euo pipefail
cd "$(dirname "$0")/.."
OFFLINE=0
WALK=1
for arg in "$@"; do
  case "$arg" in
    --offline) OFFLINE=1; WALK=0 ;;
    --no-walk) WALK=0 ;;
    *) echo "unknown option: $arg" >&2; exit 2 ;;
  esac
done
mkdir -p work
check() {
  local label=$1 log=$2 result
  shift 2
  if "$@" > "$log" 2>&1; then
    echo "$label: PASS"
    tail -n 1 "$log"
  else
    result=$?
    echo "$label: FAIL (exit=$result; $log)" >&2
    tail -n 20 "$log" >&2
    return "$result"
  fi
}
B=${CSF_CHECK_RW_BUILD:-$PWD/work/rwcheck-asan}
check "rw configure" "$B.cfg.log" cmake -S . -B "$B" -G Ninja -DCSF_RW_CPU_SLICE=ON \
  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
  '-DCMAKE_CXX_FLAGS=-fsanitize=address,undefined -fno-omit-frame-pointer -g' \
  '-DCMAKE_EXE_LINKER_FLAGS=-fsanitize=address,undefined'
# Most targets are explicit; world/world_userdata/clump are declared by foreach.
mapfile -t T < <(sed -n 's/.*add_executable(\(csf_rw_[a-z0-9_]*_link\).*/\1/p' src/boundary/rw/CMakeLists.txt | sed '/d3d9/d' | sort -u)
T+=(csf_rw_world_link csf_rw_world_userdata_link csf_rw_clump_link)
check "rw tests build" "$B.build.log" cmake --build "$B" --target "${T[@]}" -j 8
for t in "${T[@]}"; do
  check "$t" "$B.$t.log" env ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1 "$B/src/boundary/rw/$t"
done
PY=.venv/bin/python
check "linktest" work/linktest.log "$PY" tools/linktest.py -j 8
# Build first: stub checks must inspect the object from the current sources.
check "standalone build" work/standalone_build.log cmake --build build --target csf_standalone -j 8
check "hook build" work/hook_build.log cmake --build build --target csf_hook -j 8
O=build/standalone/src_boundary_netz_stubs.cpp.obj
check "NetZ manifest" work/netzstub-check.log "$PY" tools/netzstub.py --check --object "$O"
check "NetZ offline" work/netz-offline.log "$PY" src/boundary/netz/test_offline.py "$O"
check "startup reachability" work/initreach-check.log "$PY" tools/initreach.py --root 0x509250
if [ "$OFFLINE" = 0 ]; then
  check "dryrun" work/check-dryrun.log "$PY" tools/dryrun.py
  check "dryrun result" work/check-dryrun-result.log "$PY" -c \
    'import json, sys; sys.exit(not json.load(open("work/dryrun.json"))["finished"])'
else
  echo "dryrun skipped (--offline)"
fi
for test in wmrest_offline rw_particles_original rw_team_original audio1_offline test_datagen_text \
            rw_scene_finaliser_original mission1_offline mission_catalog_offline profile_new_record; do
  check "$test" "work/check-$test.log" "$PY" "tests/$test.py"
done
check "tool regressions" work/check-tools.log "$PY" -m unittest discover -s tests -p 'test_*.py'
if [ "$WALK" = 1 ]; then
  check "menu walk" work/check-walk.log "$PY" tools/dryrun.py --walk
fi
echo "all requested checks passed"
