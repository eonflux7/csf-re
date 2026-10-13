#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, S_004713e0_ReleaseOwnedStorage, 0x0069B3B0, (S_004713e0* self, void*));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static C_0085eac0* __fastcall C_0085eac0_DestroyWithOptionalFree(C_0085eac0* self, void* /*edx*/, char a2)
{
    // Most-derived vtable first, then release the four owned sub-objects.
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0085EAC0);

    S_004713e0_ReleaseOwnedStorage(
        reinterpret_cast<S_004713e0*>(reinterpret_cast<uint8_t*>(self) + 0xD4), nullptr);
    S_004713e0_ReleaseOwnedStorage(
        reinterpret_cast<S_004713e0*>(reinterpret_cast<uint8_t*>(self) + 0x90), nullptr);
    S_004713e0_ReleaseOwnedStorage(
        reinterpret_cast<S_004713e0*>(reinterpret_cast<uint8_t*>(self) + 0x4C), nullptr);
    S_004713e0_ReleaseOwnedStorage(
        reinterpret_cast<S_004713e0*>(reinterpret_cast<uint8_t*>(self) + 0x08), nullptr);

    // Base-class vtable, then optionally free the block itself.
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0085EA70);

    if ((a2 & 1) != 0)
        CoreMemory_Free(static_cast<void*>(self));

    return self;
}

HOOK(0x69E2F0, C_0085eac0_DestroyWithOptionalFree, "auto");
