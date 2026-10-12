#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class init routine (__thiscall, no stack arguments, ret 0).
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

// Game heap free (__cdecl, caller cleans the argument).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Sets the vtable pointers of C_00861c08 (primary at +0x00, secondary field at +0x04),
// runs the C_00860170 base init, and frees the block when bit 0 of the stack flag is set.
// Returns this in eax.
static C_00861c08* __fastcall C_00861c08_InitWithOptionalFree(C_00861c08* self, void* /*edx*/, char freeFlag)
{
    CSF_AT(uint32_t, self, 0x00) = CSF_ADDR_VT(0x00861C08);
    CSF_AT(uint32_t, self, 0x04) = CSF_ADDR_DATA(0x00861C00);
    CSF_AT(uint32_t, self, 0x04) = CSF_ADDR_VT(0x0082D410);

    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);

    if ((freeFlag & 1) != 0)
    {
        Mem_Free(self);
    }
    return self;
}

HOOK(0x006E5470, C_00861c08_InitWithOptionalFree, "auto");

