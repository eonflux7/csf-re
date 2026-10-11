#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_6B9130, 0x006B9130, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, sub_6BA400, 0x006BA400, (void* self, void* edx));

CSF_GLOBAL(uint32_t, g_cachedEngineHandle, 0x008BB470);
CSF_GLOBAL(uint32_t, g_engineBase, 0x008BB474);
CSF_GLOBAL(uint32_t, g_pRwEngineInstance, 0x008B68C0);

[[maybe_unused]] static void* const kEngineSlotVtable = CSF_VTBL(0x006B9130);

// Returns the cached engine handle. On first use it creates it through sub_6BA400 and caches it;
// afterwards it bumps the reference count of the existing entry and returns the same handle.
static uint32_t* __cdecl GetCachedEngineHandle() {
    int cachedHandle = static_cast<int>(g_cachedEngineHandle);
    if (cachedHandle == 0) {
        auto* engine = *reinterpret_cast<S_006b9580**>(g_engineBase + g_pRwEngineInstance + 4);
        uint32_t* created = sub_6BA400(engine, nullptr);
        g_cachedEngineHandle = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(created));
        return created;
    }

    auto* engine = *reinterpret_cast<S_006b9130**>(g_engineBase + g_pRwEngineInstance + 4);
    int entryIndex = sub_6B9130(engine, nullptr, cachedHandle);
    if (entryIndex != -1) {
        uint8_t* table = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(engine->field_0004));
        CSF_AT(int, table, 8 * entryIndex + 4) += 1;
    }
    return reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(cachedHandle));
}

HOOK(0x006BA5B0, GetCachedEngineHandle, "auto");
