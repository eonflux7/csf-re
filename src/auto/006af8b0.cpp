#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"

// Cheat binding table: 516-byte entries (0x204 stride), the entry array at +0x10, the entry count at +0x18.
// Sub_695680 copies a string-like field from the source entry to the destination entry.
CSF_FUNC(int, __cdecl, Sub_695680, 0x00695680, (char* a1, char* a2));

static int __fastcall Cheats_ApplyBindingChanges(void* self, void* /*edx*/, uint32_t* a2)
{
    uint32_t result = a2[2];

    // First pass: reset every destination entry, storing its index.
    uint32_t entryCount = CSF_AT(uint32_t, self, 0x18);
    uint32_t index = 0;
    if (entryCount) {
        uint32_t offset = 0;
        do {
            uint8_t* entry = CSF_AT(uint8_t*, self, 0x10) + offset;
            *reinterpret_cast<int32_t*>(entry) = -1;
            *reinterpret_cast<uint16_t*>(entry + 4) = 0;
            *reinterpret_cast<uint32_t*>(CSF_AT(uint8_t*, self, 0x10) + offset) = index++;
            offset += 0x204;
        } while (index < entryCount);
    }

    // Second pass: copy each source entry into the destination table.
    if (result) {
        uint32_t remaining = result;
        uint32_t offset = 0;
        do {
            *reinterpret_cast<uint32_t*>(CSF_AT(uint8_t*, self, 0x10) + offset) =
                *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, a2, 0)) + offset);

            Sub_695680(
                reinterpret_cast<char*>(CSF_AT(uint8_t*, self, 0x10) + offset + 4),
                reinterpret_cast<char*>(reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, a2, 0)) + offset + 4));

            offset += 0x204;
            remaining--;
            result = remaining;
        } while (remaining != 0);
    }

    return static_cast<int>(result);
}

HOOK(0x006AF8B0, Cheats_ApplyBindingChanges, "auto");
