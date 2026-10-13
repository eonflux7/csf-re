#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original heap allocator used by this function (game::Mem_Alloc).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Shared global: pointer to the lazily created HUD portrait state block (stored as int32_t).
CSF_GLOBAL(int32_t, g_GlobalValue, 0x008AFD9C);

// Returns the shared 44-byte state block, creating it on first use.
// Returns nullptr (and leaves the global at 0) if the allocation fails.
static uint32_t* __cdecl HudPortraits_GetOrCreateSharedState() {
    uint32_t* result = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(g_GlobalValue));
    if (g_GlobalValue != 0) {
        return result;
    }

    void* block = Mem_Alloc(0x2C);
    if (block != nullptr) {
        CSF_AT(void*, block, 0x00) = CSF_VTBL(0x00831394);
        CSF_AT(uint32_t, block, 0x08) = 0xFFFFFFFFu;
        CSF_AT(uint32_t, block, 0x0C) = 0xFFFFFFFFu;
        CSF_AT(void*, block, 0x04) = CSF_VTBL(0x0082D448);
        CSF_AT(void*, block, 0x10) = CSF_VTBL(0x0082D448);
        CSF_AT(uint32_t, block, 0x14) = 0xFFFFFFFFu;
        CSF_AT(uint32_t, block, 0x18) = 0xFFFFFFFFu;
        CSF_AT(uint32_t, block, 0x08) = 0xFFFFFFFFu;
        CSF_AT(uint32_t, block, 0x0C) = 0xFFFFFFFFu;
        CSF_AT(uint32_t, block, 0x14) = 0xFFFFFFFFu;
        CSF_AT(uint32_t, block, 0x18) = 0xFFFFFFFFu;
        CSF_AT(uint32_t, block, 0x1C) = 0;
        CSF_AT(uint32_t, block, 0x20) = 0;
        CSF_AT(uint32_t, block, 0x24) = 0;
        CSF_AT(uint32_t, block, 0x28) = 0;

        result = reinterpret_cast<uint32_t*>(block);
        g_GlobalValue = static_cast<int32_t>(reinterpret_cast<uintptr_t>(result));
    } else {
        g_GlobalValue = 0;
        result = nullptr;
    }
    return result;
}

HOOK(0x00457FD0, HudPortraits_GetOrCreateSharedState, "auto");
