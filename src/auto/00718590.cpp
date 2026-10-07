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

// 0x00718590: returns the byte at offset 0x114 of C_00864288.
// The offset lies inside the base class' pad_0114[] array, so it is accessed
// via CSF_AT instead of a declared field.
static char __fastcall C_00864288_GetField_0114(C_00864288* self, void* /*edx*/)
{
    return CSF_AT(char, self, 0x114);
}

HOOK(0x718590, C_00864288_GetField_0114, "auto");
