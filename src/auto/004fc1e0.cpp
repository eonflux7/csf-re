#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Walks the entry table (pointer array at field_0024, count at field_002c) and returns
// the index of the first entry whose dword at +0x0C equals `key`, or -1 if none match.
static int __fastcall FindIndexOfEntryByKey(S_004fc1e0* self, void* /*edx*/, int key) {
    const uint32_t count = self->field_002c;
    uint32_t result = 0;
    if (count == 0) {
        return -1;
    }

    uint32_t cursor = self->field_0024;
    while (true) {
        uint8_t* entry = reinterpret_cast<uint8_t*>(*reinterpret_cast<uint32_t*>(cursor));
        if (CSF_AT(int, entry, 0x0C) == key) {
            return static_cast<int>(result);
        }
        ++result;
        if (result >= count) {
            return -1;
        }
        cursor += 4;
    }
}

HOOK(0x4FC1E0, FindIndexOfEntryByKey, "auto");
