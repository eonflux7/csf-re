#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-level cleanup routine (__thiscall, no stack arguments, ret 0).
CSF_FUNC(int, __fastcall, C_00843720_ReleaseSubobjects, 0x00537130, (void* self, void* edx));

// Game heap free (__cdecl, caller cleans the argument).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Resets the vtable pointers of C_00843720 (primary at +0x00, secondary at +0x04,
// tertiary at +0x10), runs the base cleanup, and frees the block when bit 0 of the
// stack flag is set. Returns this in eax.
static C_00843720* __fastcall C_00843720_DestructWithOptionalFree(C_00843720* self, void* /*edx*/, char freeFlag)
{
    CSF_AT(uint32_t, self, 0x00) = CSF_ADDR_VT(0x00843720);
    CSF_AT(uint32_t, self, 0x04) = CSF_ADDR_VT(0x008436D0);
    CSF_AT(uint32_t, self, 0x10) = CSF_ADDR_VT(0x008436CC);

    C_00843720_ReleaseSubobjects(self, nullptr);

    if ((freeFlag & 1) != 0)
    {
        Mem_Free(self);
    }
    return self;
}

HOOK(0x00543C00, C_00843720_DestructWithOptionalFree, "auto");
