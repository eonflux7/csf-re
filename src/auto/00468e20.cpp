#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Scans the 8-byte {key, value} entries of a vector-like table at this+8 (begin) / this+0xC (end)
// for the first entry whose key equals `key`. Returns that entry's value, or -1 if not found.
static int __fastcall SceneEntryIndexFromFloat(void* self, void* /*edx*/, int key) {
    uint32_t entryBegin = CSF_AT(uint32_t, self, 0x8);
    uint32_t entryCount = 0;
    if (entryBegin) {
        entryCount = (uint32_t)((int32_t)(CSF_AT(uint32_t, self, 0xC) - entryBegin) >> 3);
    }

    int result = -1;
    if (entryCount) {
        uint8_t* entries = reinterpret_cast<uint8_t*>(entryBegin);
        for (uint32_t index = 0; index < entryCount; ++index) {
            if (CSF_AT(int, entries, index * 8) == key) {
                return CSF_AT(int, entries, index * 8 + 4);
            }
        }
    }
    return result;
}

HOOK(0x00468E20, SceneEntryIndexFromFloat, "auto");
