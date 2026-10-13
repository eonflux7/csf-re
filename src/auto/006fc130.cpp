#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class constructor of C_00862e88 chain (__thiscall, no stack arguments, ret 0).
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

// Game heap free (__cdecl, caller cleans the argument).
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Initializes the vtable pointers of C_00862df0 (primary at +0x00, secondary field at +0x04,
// then overwritten at +0x04 with C_0082d410's vtable), runs the C_00860170 base constructor,
// and frees the block when bit 0 of the stack flag is set. Returns this in eax.
static uint32_t* __fastcall C_00862df0_InitWithOptionalFree(C_00862df0* self, void* /*edx*/, char freeFlag)
{
    CSF_AT(void**, self, 0x00) = CSF_VTBL(0x00862DF0);
    self->field_0004 = CSF_ADDR_DATA(0x00862DE8);
    self->field_0004 = CSF_ADDR_VT(0x0082D410);

    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);

    if ((freeFlag & 1) != 0)
    {
        CoreMemory_Free(self);
    }
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x006FC130, C_00862df0_InitWithOptionalFree, "auto");
