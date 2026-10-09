#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"

// Virtual getter signatures on the source accessor (a2). All are __thiscall,
// bound here as __fastcall so the source object goes in ecx and edx is unused.
using SourceIntGetter = int(__fastcall*)(void* self, void* edx);
using SourceDataGetter = void*(__fastcall*)(void* self, void* edx);
using SourceBufferGetter = void*(__fastcall*)(void* self, void* edx, void* buffer);

// Copies layout/state values from the source accessor into the HUD object at self
// and returns the value of the source's final query (also stored at self+0x308).
static int __fastcall Hud_CopySourceLayoutState(void* self, void* /*edx*/, int* source)
{
    // source->vtbl[0x48 / 4]: scalar stored at self+0x2D4.
    CSF_AT(int, self, 0x2D4) =
        reinterpret_cast<SourceIntGetter>(CSF_AT(void**, source, 0)[18])(source, nullptr);

    // source->vtbl[0x50 / 4]: pointer to four dwords stored at self+0x2D8..0x2E4.
    uint32_t* layoutRect = static_cast<uint32_t*>(
        reinterpret_cast<SourceDataGetter>(CSF_AT(void**, source, 0)[20])(source, nullptr));
    CSF_AT(uint32_t, self, 0x2D8) = layoutRect[0];
    CSF_AT(uint32_t, self, 0x2DC) = layoutRect[1];
    CSF_AT(uint32_t, self, 0x2E0) = layoutRect[2];
    CSF_AT(uint32_t, self, 0x2E4) = layoutRect[3];

    // source->vtbl[0x58 / 4]: fills a 16-byte scratch buffer, returns pointer to four dwords
    // stored at self+0x2E8..0x2F4.
    uint8_t scratchBuffer[16];
    uint32_t* stateRect = static_cast<uint32_t*>(
        reinterpret_cast<SourceBufferGetter>(CSF_AT(void**, source, 0)[22])(source, nullptr, scratchBuffer));
    CSF_AT(uint32_t, self, 0x2E8) = stateRect[0];
    CSF_AT(uint32_t, self, 0x2EC) = stateRect[1];
    CSF_AT(uint32_t, self, 0x2F0) = stateRect[2];
    CSF_AT(uint32_t, self, 0x2F4) = stateRect[3];

    // source->vtbl[0x64 / 4]: returns a pointer to a pointer; the 64-byte block it points to
    // is copied into the buffer whose pointer lives at self+0x30C. The destination pointer is
    // read after the call, as in the original.
    void* sourceBlockSlot =
        reinterpret_cast<SourceDataGetter>(CSF_AT(void**, source, 0)[25])(source, nullptr);
    const void* sourceBlock = *static_cast<void* const*>(sourceBlockSlot);
    void* destinationBlock = CSF_AT(void*, self, 0x30C);
    std::memcpy(destinationBlock, sourceBlock, 0x40);

    // source->vtbl[0x6C / 4]: pointer to four dwords stored at self+0x2F8..0x304.
    uint32_t* extraRect = static_cast<uint32_t*>(
        reinterpret_cast<SourceDataGetter>(CSF_AT(void**, source, 0)[27])(source, nullptr));
    CSF_AT(uint32_t, self, 0x2F8) = extraRect[0];
    CSF_AT(uint32_t, self, 0x2FC) = extraRect[1];
    CSF_AT(uint32_t, self, 0x300) = extraRect[2];
    CSF_AT(uint32_t, self, 0x304) = extraRect[3];

    // source->vtbl[0x74 / 4]: integer result, stored at self+0x308 and returned.
    int sourceResult =
        reinterpret_cast<SourceIntGetter>(CSF_AT(void**, source, 0)[29])(source, nullptr);
    CSF_AT(int, self, 0x308) = sourceResult;
    return sourceResult;
}

HOOK(0x0042E020, Hud_CopySourceLayoutState, "auto");
