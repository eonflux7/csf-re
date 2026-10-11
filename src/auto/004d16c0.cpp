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

// Copies a 3-dword cover-attack state from a2 into this+4..+0xC and resets
// the flag byte at +0 and the two -1 markers at +0x14 / +0x18.
static int __fastcall AiCore_CopyCoverAttackState(void* self, void* /*edx*/, uint32_t* source)
{
    CSF_AT(uint8_t, self, 0x00) = 1;
    CSF_AT(int, self, 0x14) = -1;
    CSF_AT(int, self, 0x18) = -1;

    CSF_AT(uint32_t, self, 0x04) = source[0];
    CSF_AT(uint32_t, self, 0x08) = source[1];
    int lastValue = static_cast<int>(source[2]);
    CSF_AT(int, self, 0x0C) = lastValue;
    return lastValue;
}

HOOK(0x4D16C0, AiCore_CopyCoverAttackState, "auto");
