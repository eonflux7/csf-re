#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, C_0082daf4_ConstructEntry, 0x0040C330, (void* self, void* edx, int a2, int a3));
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));

// Looks up the entry whose key (dword at +8) equals `key` in the owner's entry list.
// If none exists, allocates a 0x2C-byte entry, constructs it, appends it, and returns it.
static C_0082daf4* __fastcall C_0082daf4_FindOrCreateByKey(void* self, void* /*edx*/, int key) {
    const uint32_t entryCount = CSF_AT(uint32_t, self, 0x24);
    C_0082daf4* result = nullptr;

    if (entryCount != 0) {
        auto* entries = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x1C));
        for (uint32_t i = 0; i < entryCount; ++i) {
            auto* entry = reinterpret_cast<uint32_t*>(entries[i]);
            if (CSF_AT(int, entry, 8) == key)
                result = reinterpret_cast<C_0082daf4*>(entry);
        }
    }

    if (result)
        return result;

    C_0082daf4* created = nullptr;
    void* block = Mem_Alloc(0x2C);
    if (block) {
        created = reinterpret_cast<C_0082daf4*>(
            C_0082daf4_ConstructEntry(block, nullptr,
                                      static_cast<int>(reinterpret_cast<uintptr_t>(self)),
                                      key));
    }

    // Grow the entry array when count >= capacity (count/capacity re-read after the call).
    if (CSF_AT(uint32_t, self, 0x24) >= CSF_AT(uint32_t, self, 0x20)) {
        void* list = static_cast<uint8_t*>(self) + 0x1C;
        GrowButtonArray(list, nullptr, CSF_AT(int, self, 0x28));
    }

    const uint32_t index = CSF_AT(uint32_t, self, 0x24);
    auto** slots = reinterpret_cast<C_0082daf4**>(CSF_AT(uint32_t, self, 0x1C));
    CSF_AT(uint32_t, self, 0x24) = index + 1;
    slots[index] = created;
    return created;
}

HOOK(0x4087D0, C_0082daf4_FindOrCreateByKey, "auto");
