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

// Resets the scalar option defaults: two floats at +4/+8 set to 1.0f,
// an int at +12 set to 1, and two bytes at +16/+17 cleared.
// Returns the bit pattern of 1.0f (0x3F800000) in eax.
static int __fastcall OptionsFile_SetDefaultScalars(void* self, void* /*edx*/)
{
    CSF_AT(float, self, 0x4) = 1.0f;
    CSF_AT(float, self, 0x8) = 1.0f;
    CSF_AT(int, self, 0xC) = 1;
    CSF_AT(uint8_t, self, 0x10) = 0;
    CSF_AT(uint8_t, self, 0x11) = 0;
    return 1065353216;
}

HOOK(0x50A960, OptionsFile_SetDefaultScalars, "auto");
