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

// 0x004D6DD0 (6 bytes): mov eax, 0x83C8E8 ; ret
// Returns the address of the constant string "TYendoA" stored in the game image.
static const char* __cdecl GetTYendoA()
{
    return CSF_STR(0x0083C8E8, "TYendoA");
}

HOOK(0x004D6DD0, GetTYendoA, "auto");
