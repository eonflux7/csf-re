#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: MenuMpAccounts_CompareNames (0x005158F0), __fastcall with 4 stack args (ret 16).
CSF_FUNC(int, __fastcall, MenuMpAccounts_CompareNames, 0x005158F0,
         (void* self, void* edx, unsigned int offset, unsigned int length,
          const uint16_t* text, unsigned int textLength));

// Scans the 80-byte entries between field_0008 and field_000c for a name matching
// nameQuery[5]. Returns the index of the first match, or -1.
static int __fastcall C_00840af8_FindEntryIndexByName(C_00840af8* self, void* /*edx*/, unsigned int* nameQuery)
{
    uint32_t entriesBegin = self->field_0008;
    unsigned int entryCount = 0;
    if (entriesBegin) {
        entryCount = static_cast<unsigned int>(static_cast<int>(self->field_000c - entriesBegin) / 80);
    }
    if (entryCount == 0) {
        return -1;
    }

    unsigned int matchIndex = 0;
    for (uint32_t entryOffset = 0; ; entryOffset += 80) {
        uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0008) + entryOffset);
        uint32_t entryAddr = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(entry));
        uint32_t nameStorageLength = CSF_AT(uint32_t, entry, 0x18);
        uint32_t textLength = CSF_AT(uint32_t, entry, 0x14);

        // Short names are stored inline at +4; longer ones are referenced by pointer stored at +4.
        uint32_t textAddress = (nameStorageLength < 8) ? entryAddr + 4 : CSF_AT(uint32_t, entry, 4);

        int nameMatches = MenuMpAccounts_CompareNames(
            nameQuery,
            reinterpret_cast<void*>(static_cast<uintptr_t>(nameQuery[5])),
            0u,
            nameQuery[5],
            reinterpret_cast<const uint16_t*>(static_cast<uintptr_t>(textAddress)),
            textLength);

        if (!nameMatches) {
            return static_cast<int>(matchIndex);
        }
        if (++matchIndex >= entryCount) {
            return -1;
        }
    }
}

HOOK(0x515FC0, C_00840af8_FindEntryIndexByName, "auto");
