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

// Sets the vtable pointers of C_00860f10 (primary at +0x00, secondary at +0x04),
// runs the C_00860170 base constructor, and frees the block when bit 0 of the
// stack flag is set. Returns this in eax.
static C_00860f10* __fastcall C_00860f10_ConstructWithOptionalFree(C_00860f10* self, void* /*edx*/, char freeFlag)
{
    *reinterpret_cast<void**>(self) = CSF_VTBL(0x00860F10);
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x00860F04);
    self->field_0004 = (uint32_t)CSF_VTBL(0x0082D410);

    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);

    if ((freeFlag & 1) != 0)
    {
        Mem_Free(self);
    }
    return self;
}

HOOK(0x006D2910, C_00860f10_ConstructWithOptionalFree, "auto");
