#include <cstdint>
#include "globals.h"
#include "hook.h"

// Copies the value at self+0x58 into the target's field at +4 when self+0x28 == 4;
// otherwise writes 0. Pops 3 stack args (ret 12).
static int __fastcall CopyActiveValueToOutput(void* self, void* /*edx*/, void* outSlot, int /*a3*/, int /*a4*/)
{
    int32_t result = 0;

    if (*CSF_AT(int32_t*, self, 0x28) == 4)
    {
        result = *CSF_AT(int32_t*, self, 0x58);
        uint8_t* target = *CSF_AT(uint8_t**, outSlot, 0x4);
        *CSF_AT(int32_t*, target, 0x4) = result;
    }
    else
    {
        uint8_t* target = *CSF_AT(uint8_t**, outSlot, 0x4);
        *CSF_AT(int32_t*, target, 0x4) = 0;
    }

    return result;
}

HOOK(0x45C6F0, CopyActiveValueToOutput, "auto");
