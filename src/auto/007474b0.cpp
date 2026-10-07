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

// Returns the literal type/class name string "NetProyectil".
// Original: mov eax, 0x867EF0 ; ret   (0x00867EF0 -> 'NetProyectil')
static const char* __cdecl GetNetProyectil()
{
    return CSF_STR(0x00867EF0, "NetProyectil");
}

HOOK(0x007474B0, GetNetProyectil, "auto");
