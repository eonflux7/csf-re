#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns bit 1 of the flags word at +0x34 of the linked object (field_0004),
// but only when the linked object's pointer at +0x50 is non-null; otherwise returns 0.
static char __fastcall C_00863dd0_GetSecondFlagBitIfLinked(C_00863dd0* self, void* /*edx*/)
{
    void* linkedObject = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0004));

    if (CSF_AT(uint32_t, linkedObject, 0x50) != 0)
    {
        uint32_t flagWord = CSF_AT(uint32_t, linkedObject, 0x34);
        return static_cast<char>((flagWord >> 1) & 1);
    }
    return 0;
}

HOOK(0x007123C0, C_00863dd0_GetSecondFlagBitIfLinked, "auto");
