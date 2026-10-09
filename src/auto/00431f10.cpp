#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Looks up `key` in CHud's pair vector at +0xA0..+0xA4 (8-byte entries:
// value at +0, key at +4). Returns the matching value, or 0 if not found.
// A key of -1 never matches and returns 0 immediately.
static int __fastcall CHud_LookupValueByKey(CHud* self, void* /*edx*/, int key) {
    if (key == -1) {
        return 0;
    }

    const uint32_t entriesBegin = self->field_00a0;
    unsigned int entryCount = 0;
    if (entriesBegin != 0) {
        entryCount = static_cast<unsigned int>(static_cast<int>(self->field_00a4 - entriesBegin) >> 3);
    }
    if (entryCount == 0) {
        return 0;
    }

    for (unsigned int i = 0; i < entryCount; ++i) {
        const int entryKey = CSF_AT(int, reinterpret_cast<void*>(entriesBegin), i * 8 + 4);
        if (key == entryKey) {
            return CSF_AT(int, reinterpret_cast<void*>(entriesBegin), i * 8);
        }
    }
    return 0;
}

HOOK(0x431F10, CHud_LookupValueByKey, "auto");
