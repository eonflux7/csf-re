#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class init/cleanup routine (__thiscall, no stack arguments, ret 0).
CSF_FUNC(void, __fastcall, CharTaskBase_Init_4DB3C0, 0x004DB3C0, (void* self, void* edx));

// Game heap free (__cdecl, caller cleans the argument).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Resets the vtable pointers of C_0083b958 (primary at +0x00, secondary at +0x04,
// tertiary at +0x2C), runs the TCharTask base cleanup, and frees the block when bit 0
// of the stack flag is set. Returns this in eax.
static C_0083b958* __fastcall C_0083b958_DestructWithOptionalFree(C_0083b958* self, void* /*edx*/, char freeFlag)
{
    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x0083B958);
    CSF_AT(uint32_t, self, 0x04) = CSF_ADDR_VT(0x0083B938);
    CSF_AT(void*, self, 0x2C) = CSF_VTBL(0x0082D410);

    CharTaskBase_Init_4DB3C0(self, nullptr);

    if ((freeFlag & 1) != 0)
    {
        Mem_Free(self);
    }
    return self;
}

HOOK(0x004D1620, C_0083b958_DestructWithOptionalFree, "auto");
