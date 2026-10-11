#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup chain for C_0083b668 / C_0083b958 / TCharTask (__thiscall, no stack args).
CSF_FUNC(int, __fastcall, BaseChainCleanup_4D1140, 0x004D1140, (void* self, void* edx));

// Game heap free (__cdecl, caller cleans the argument).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Resets the vtable pointers of C_0083dd58 (primary at +0x00, secondary at +0x04,
// tertiary at +0x40), runs the base-chain cleanup, and frees the block when bit 0
// of the stack flag is set. Returns this in eax.
static C_0083dd58* __fastcall C_0083dd58_DestructWithOptionalFree(C_0083dd58* self, void* /*edx*/, char freeFlag)
{
    CSF_AT(uint32_t, self, 0x00) = CSF_ADDR_VT(0x0083DD58);
    CSF_AT(uint32_t, self, 0x04) = CSF_ADDR_VT(0x0083DD3C);
    CSF_AT(void*, self, 0x40) = CSF_VTBL(0x0082D410);

    BaseChainCleanup_4D1140(self, nullptr);

    if ((freeFlag & 1) != 0)
    {
        Mem_Free(self);
    }
    return self;
}

HOOK(0x004DAA00, C_0083dd58_DestructWithOptionalFree, "auto");
