#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns bit 2 of the dword at +0x34 of the object referenced by this->field_0004.
static int __fastcall C_00863dd0_GetFlagBit2(C_00863dd0* self, void* /*edx*/)
{
    uint32_t* inner = reinterpret_cast<uint32_t*>(self->field_0004);
    return (CSF_AT(uint32_t, inner, 0x34) >> 2) & 1;
}

HOOK(0x007122C0, C_00863dd0_GetFlagBit2, "auto");
