#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, RenderHandle_UnregisteredFallback, 0x0081CAA0, (int a1));

[[maybe_unused]] static const auto kListVtable = CSF_VTBL(0x0041E5F0);

// Removes/records a handle in the per-owner list whose key matches a2->key (a2 + 4).
// Appends a2 to the matching entry's item array, growing it when full.
// Returns the new item count for that entry, or the fallback result when no entry matches.
static uint32_t __fastcall RenderHandle_Release(uint32_t* self, void* /*edx*/, int a2) {
    uint32_t* handle = reinterpret_cast<uint32_t*>(a2);
    uint32_t entryCount = CSF_AT(uint32_t, self, 0x6C);
    uint32_t index = 0;

    if (entryCount == 0) {
        return static_cast<uint32_t>(RenderHandle_UnregisteredFallback(a2));
    }

    uint32_t** entries = reinterpret_cast<uint32_t**>(CSF_AT(uint32_t, self, 0x64));
    const uint32_t wantedKey = handle[1];

    uint32_t** cursor = entries;
    for (; **cursor != wantedKey; ++cursor) {
        if (++index >= entryCount) {
            return static_cast<uint32_t>(RenderHandle_UnregisteredFallback(a2));
        }
    }

    uint32_t* entry = reinterpret_cast<uint32_t*>(entries[index]);
    S_0041e5f0* list = reinterpret_cast<S_0041e5f0*>(entry + 1);

    uint32_t usedCount = entry[3];
    uint32_t capacity = entry[2];
    if (usedCount >= capacity) {
        S_0041e5f0_GrowArrayCapacity(list, nullptr, static_cast<int>(list->field_000c));
    }

    uint32_t slotIndex = list->field_0008;
    uint32_t* items = reinterpret_cast<uint32_t*>(list->field_0000);
    uint32_t result = slotIndex + 1;
    list->field_0008 = result;
    items[slotIndex] = static_cast<uint32_t>(a2);
    return result;
}

HOOK(0x006D6E60, RenderHandle_Release, "auto");
