#!/bin/bash
# Everything a patch must pass before commit: native RW tests (ASan/UBSan), linktest, NetZ stub checks, both builds,
# the dry run, the offline Unicorn tests, then the unattended menu walk (tools/dryrun.py --walk). One line per step.
# Usage: tools/check_all.sh [--no-walk]   (about 15 minutes with the walk)
cd "$(dirname "$0")/.."
B=$PWD/work/rwcheck-asan
cmake -S . -B $B -G Ninja -DCSF_RW_CPU_SLICE=ON -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
  '-DCMAKE_CXX_FLAGS=-fsanitize=address,undefined -fno-omit-frame-pointer -g' '-DCMAKE_EXE_LINKER_FLAGS=-fsanitize=address,undefined' > $B.cfg.log 2>&1
T="$(grep -o "add_executable(csf_rw_[a-z0-9_]*_link" src/boundary/rw/CMakeLists.txt | sed 's/add_executable(//' | grep -v d3d9 | sort -u | tr '\n' ' ') csf_rw_world_link csf_rw_world_userdata_link csf_rw_clump_link"  # the last three: a foreach
cmake --build $B --target $T -j 8 > $B.build.log 2>&1; echo "rw tests build exit=$?"
for t in $T; do ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1 $B/src/boundary/rw/$t > $B.$t.log 2>&1; echo "$t exit=$? $(tail -1 $B.$t.log | cut -c1-80)"; done
.venv/bin/python tools/linktest.py -j 8 > work/linktest.log 2>&1; echo "linktest exit=$?"; tail -3 work/linktest.log | head -2
O=build/standalone/src_boundary_netz_stubs.cpp.obj
.venv/bin/python tools/netzstub.py --check --object $O 2>&1 | tail -1
.venv/bin/python src/boundary/netz/test_offline.py $O 2>&1 | tail -1
cmake --build build --target csf_standalone -j 8 > work/standalone_build.log 2>&1; echo "standalone build exit=$?"
cmake --build build --target csf_hook -j 8 > work/hook_build.log 2>&1; echo "hook build exit=$?"
.venv/bin/python tools/initreach.py --root 0x509250 | head -1
.venv/bin/python tools/dryrun.py
.venv/bin/python tests/wmrest_offline.py 2>&1 | tail -1
.venv/bin/python tests/rw_particles_original.py 2>&1 | tail -1
.venv/bin/python tests/audio1_offline.py 2>&1 | tail -1
.venv/bin/python tests/test_datagen_text.py 2>&1 | tail -1
[ "$1" = --no-walk ] || .venv/bin/python tools/dryrun.py --walk 2>&1 | tail -20
