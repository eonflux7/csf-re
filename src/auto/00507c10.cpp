#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Byte-by-byte equality of a NUL-terminated key against a table entry name.
// Mirrors the inline compare loop in the original (no CRT call).
static bool KeyStringsEqual(const char* key, const char* entryName) {
    const uint8_t* a = reinterpret_cast<const uint8_t*>(key);
    const uint8_t* b = reinterpret_cast<const uint8_t*>(entryName);
    while (true) {
        uint8_t ca = *a;
        if (ca != *b) {
            return false;
        }
        if (ca == 0) {
            return true;
        }
        ++a;
        ++b;
    }
}

// Walks the table at self->+0x44 (count at self->+0x4C, 36-byte entries).
// Each entry: +0x04 name (inline if its length at +0x18 is < 0x10, otherwise a pointer),
// +0x1C value. Returns the value of the last matching entry scanned, stopping at the
// first non-zero value.
static int __fastcall S_005080e0_LookupValueByName(S_005080e0* self, void* /*edx*/, const char* name) {
    int result = 0;
    int lastMatchedValue = 0;
    const uint32_t entryCount = CSF_AT(uint32_t, self, 0x4C);
    uint32_t entryIndex = 0;
    uint32_t entryByteOffset = 0;

    while (entryIndex < entryCount) {
        uint8_t* tableBase = CSF_AT(uint8_t*, self, 0x44);
        uint8_t* entry = tableBase + entryByteOffset;

        uint32_t nameLength = CSF_AT(uint32_t, entry, 0x18);
        const char* entryName = (nameLength < 0x10)
            ? reinterpret_cast<const char*>(entry + 4)
            : CSF_AT(const char*, entry, 4);

        if (KeyStringsEqual(name, entryName)) {
            lastMatchedValue = CSF_AT(int, entry, 0x1C);
        }
        result = lastMatchedValue;

        ++entryIndex;
        entryByteOffset += 36;
        if (result != 0) {
            break;
        }
    }
    return result;
}

HOOK(0x00507C10, S_005080e0_LookupValueByName, "auto");
