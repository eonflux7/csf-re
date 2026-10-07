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

// 0x00751E00 - returns the constant 200.
static int __cdecl GetValue200()
{
  return 200;
}

HOOK(0x751E00, GetValue200, "auto");
