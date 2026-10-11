#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup of TCharTask (__thiscall, no stack arguments, ret 0).
CSF_FUNC(int, __fastcall, C_0083b958_Destroy, 0x004D1140, (void* self, void* edx));

// Game heap free (__cdecl, caller cleans the argument).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Resets the vtable pointers of C_0083b4a0 (primary at +0x00, secondary at +0x04,
// tertiary at +0x48), runs the TCharTask base cleanup, and frees the block when bit 0
// of the stack flag is set. Returns this in eax.
static C_0083b4a0* __fastcall C_0083b4a0_DestructWithOptionalFree(C_0083b4a0* self, void* /*edx*/, char freeFlag)
{
    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x0083B4A0);
    CSF_AT(void*, self, 0x04) = CSF_VTBL(0x0083B484);
    CSF_AT(void*, self, 0x48) = CSF_VTBL(0x0082D410);

    C_0083b958_Destroy(self, nullptr);

    if ((freeFlag & 1) != 0)
    {
        Mem_Free(self);
    }
    return self;
}

HOOK(0x004CF8E0, C_0083b4a0_DestructWithOptionalFree, "auto");
