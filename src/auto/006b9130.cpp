#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the index of the first 8-byte entry whose leading dword equals `key`,
// scanning the [begin, end) range stored in the object; -1 if not found.
static int __fastcall GfxObjectProps_FindEntryIndex(S_006b9130* self, void* /*edx*/, int key) {
    uint32_t entries = self->field_0004;
    int result = -1;
    uint32_t i = 0;

    while (entries != 0) {
        // The original re-reads the end pointer on every iteration.
        int count = static_cast<int>(self->field_0008 - entries) >> 3;
        if (i >= static_cast<uint32_t>(count)) {
            break;
        }
        const auto* element = reinterpret_cast<const int32_t*>(
            static_cast<uintptr_t>(entries) + 8u * i);
        if (*element == key) {
            result = static_cast<int>(i);
            break;
        }
        ++i;
    }
    return result;
}

HOOK(0x006B9130, GfxObjectProps_FindEntryIndex, "auto");
