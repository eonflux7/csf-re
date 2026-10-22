"""Machine-local game directories, all outside the repo. Set CSF_DATA_ROOT once, or each directory on its own:
  CSF_PLAY        the hook play copy of the game           (default $CSF_DATA_ROOT/csf-play)
  CSF_STANDALONE  the standalone exe's run directory       (default $CSF_DATA_ROOT/csf-standalone)
  CSF_PAKS        extracted .pak contents (test fixtures)  (default $CSF_DATA_ROOT/csf-paks)
  CSF_BUILDS      the original game builds (read-only)     (default $CSF_DATA_ROOT/csf-builds)
CSF_DATA_ROOT defaults to /mnt/e/dev (the original developer's WSL layout). Mirrored by tests/paths.h (C++) and the
${CSF_X:-${CSF_DATA_ROOT:-/mnt/e/dev}/...} defaults of the shell scripts."""
import os

DATA_ROOT = os.environ.get("CSF_DATA_ROOT", "/mnt/e/dev")


def _dir(var, name):
    return os.environ.get(var) or os.path.join(DATA_ROOT, name)


PLAY = _dir("CSF_PLAY", "csf-play")
STANDALONE = _dir("CSF_STANDALONE", "csf-standalone")
PAKS = _dir("CSF_PAKS", "csf-paks")
BUILDS = _dir("CSF_BUILDS", "csf-builds")
