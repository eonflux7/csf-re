#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860894_DestroyEmbeddedMember8, 0x006C4A50, (C_00860894* self, void*));
CSF_FUNC(int, __fastcall, GfxScene_DetachFromWorld, 0x006D5DF0, (void* self, void* edx));

// Releases the three embedded members and the heap blocks owned by this scene object.
// Returns the low byte of the last Mem_Free result, or 0 if the last owned block was null.
static char __fastcall C_0085e7b8_ReleaseResources(C_0085e7b8* self, void* /*edx*/)
{
    GfxScene_DetachFromWorld(self, nullptr);

    // Embedded members at +0x78, +0x64, +0x50 (lea ecx, [esi+N]).
    C_00860894_DestroyEmbeddedMember8(
        reinterpret_cast<C_00860894*>(reinterpret_cast<uint8_t*>(self) + 0x78), nullptr);
    C_00860894_DestroyEmbeddedMember8(
        reinterpret_cast<C_00860894*>(reinterpret_cast<uint8_t*>(self) + 0x64), nullptr);
    C_00860894_DestroyEmbeddedMember8(
        reinterpret_cast<C_00860894*>(reinterpret_cast<uint8_t*>(self) + 0x50), nullptr);

    uint32_t block38 = CSF_AT(uint32_t, self, 0x38);
    if (block38 != 0) {
        Mem_Free(reinterpret_cast<void*>(block38));
        CSF_AT(uint32_t, self, 0x38) = 0;
    }

    uint32_t block24 = CSF_AT(uint32_t, self, 0x24);
    if (block24 != 0) {
        Mem_Free(reinterpret_cast<void*>(block24));
        CSF_AT(uint32_t, self, 0x24) = 0;
    }

    uint32_t block14 = CSF_AT(uint32_t, self, 0x14);
    if (block14 != 0) {
        Mem_Free(reinterpret_cast<void*>(block14));
        CSF_AT(uint32_t, self, 0x14) = 0;
    }

    int freeResult = 0;
    if (self->field_0004 != 0) {
        freeResult = Mem_Free(reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
    }
    return static_cast<char>(freeResult);
}

HOOK(0x006CB0E0, C_0085e7b8_ReleaseResources, "auto");
