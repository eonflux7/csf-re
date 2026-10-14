#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_771B80, 0x00771B80, (void* self, void* edx, int a2));

static int __fastcall GetMappedValueForKey(void* self, void* /*edx*/, S_00771b80* key) {
    // Code-address annotation: function value, not a vtable.
    [[maybe_unused]] auto* keyVtbl = reinterpret_cast<void**>(CSF_ADDR_FN(0x00771B80));

    // Owner -> table owner at +0x08 -> table holder at +0x2C
    uint32_t ownerRaw = CSF_AT(uint32_t, self, 0x08);
    S_00771b80* tableOwner = reinterpret_cast<S_00771b80*>(
        static_cast<uintptr_t>(CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(ownerRaw)), 0x2C)));

    // Key index lookup in the sub-object embedded at +0xF0
    void* indexTable = reinterpret_cast<uint8_t*>(tableOwner) + 0xF0;
    int index = sub_771B80(indexTable, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(key)));
    if (index == -1) {
        return -1;
    }

    // Value array pointer at +0xF4, 12-byte entries, value stored at entry start
    uint8_t* valueArray = reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(CSF_AT(uint32_t, tableOwner, 0xF4)));
    return CSF_AT(int, valueArray, 12 * index);
}

HOOK(0x004A6CF0, GetMappedValueForKey, "auto");
