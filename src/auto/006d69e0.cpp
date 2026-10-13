#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Looks up the entry whose first dword equals `key` in the object's table of
// entries, then pops the last value from that entry's value array (count at +0x0C,
// value array pointer at +0x04). Returns 0 if the key is not found or the entry is empty.
static int __fastcall S_006D69E0_PopItemByKey(S_006d69e0* self, void* /*edx*/, int key) {
    uint32_t entryCount = self->field_006c;
    if (entryCount == 0) {
        return 0;
    }

    uint32_t* entries = reinterpret_cast<uint32_t*>(self->field_0064);
    uint32_t matchIndex = 0;
    for (;;) {
        uint8_t* entry = reinterpret_cast<uint8_t*>(entries[matchIndex]);
        if (CSF_AT(int, entry, 0x00) == key) {
            break;
        }
        ++matchIndex;
        if (matchIndex >= entryCount) {
            return 0;
        }
    }

    uint8_t* entry = reinterpret_cast<uint8_t*>(entries[matchIndex]);
    uint32_t valueCount = CSF_AT(uint32_t, entry, 0x0C);
    if (valueCount == 0) {
        return 0;
    }

    uint32_t* valueArray = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, entry, 0x04));
    int poppedValue = static_cast<int>(valueArray[valueCount - 1]);
    CSF_AT(uint32_t, entry, 0x0C) = valueCount - 1;
    return poppedValue;
}

HOOK(0x006D69E0, S_006D69E0_PopItemByKey, "auto");
