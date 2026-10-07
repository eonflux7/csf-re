#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x0074B150: sets the byte flag at offset 0x08 to 1 and returns 1.
static char __fastcall C_74b150_SetFlag8(void* self, void* /*edx*/)
{
  CSF_AT(unsigned char, self, 0x08) = 1;
  return 1;
}

HOOK(0x0074B150, C_74b150_SetFlag8, "auto");
