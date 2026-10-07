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

// 0x00718650: returns the byte stored at offset 0xC8 of the object.
static char __fastcall C_00864288_GetField00c8(C_00864288* self, void* /*edx*/)
{
    return static_cast<char>(CSF_AT(uint8_t, self, 0xc8));
}

HOOK(0x718650, C_00864288_GetField00c8, "auto");
