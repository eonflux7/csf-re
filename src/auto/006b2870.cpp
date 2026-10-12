#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class constructor (__thiscall, no stack arguments, ret 0).
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void* edx));

// Game heap free (__cdecl, caller cleans the argument).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Stores the C_008601a8 vtable at +0x00, writes two values to the +0x04 slot (the
// first, data at 0x0086019C, is overwritten immediately by the 0x0082D410 vtable),
// runs the C_00860170 base constructor, and frees the block when bit 0 of the stack
// flag is set. Returns this in eax.
static C_008601a8* __fastcall C_008601a8_ConstructWithOptionalFree(C_008601a8* self, void* /*edx*/, char freeFlag)
{
    CSF_AT(uint32_t, self, 0x00) = CSF_ADDR_VT(0x008601A8);
    CSF_AT(uint32_t, self, 0x04) = CSF_ADDR_DATA(0x0086019C);
    CSF_AT(uint32_t, self, 0x04) = CSF_ADDR_VT(0x0082D410);

    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);

    if ((freeFlag & 1) != 0)
    {
        Mem_Free(self);
    }
    return self;
}

HOOK(0x006B2870, C_008601a8_ConstructWithOptionalFree, "auto");
