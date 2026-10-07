#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the animation/state name literal "TGirandoHastaAngulo"
// ("turning until angle"), stored as constant data at 0x0083CA68.
static const char* GetTGirandoHastaAngulo()
{
  return CSF_STR(0x0083CA68, "TGirandoHastaAngulo");
}

HOOK(0x004D6DF0, GetTGirandoHastaAngulo, "auto");
