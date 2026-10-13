#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, MenuMpAccounts_CompareNames, 0x005158F0,
         (void* self, void* edx, unsigned int offset, unsigned int length, const uint16_t* text, unsigned int textLength));

// Scans the 30 account slots (48 bytes each, starting at +8) and returns the index of the
// first slot whose stored name matches the given string, or -1 if none matches.
static int __fastcall C_00867900_FindEntryByPlayerName(uint8_t* self, void* /*edx*/, uint8_t* nameString)
{
    int result = -1;
    int matchIndex = -1;
    int slotIndex = 0;
    uint8_t* slot = self + 8;

    do
    {
        if (result != -1)
            break;

        // Inline buffer when capacity < 8, otherwise heap pointer stored at +4.
        const uint16_t* text = CSF_AT(uint32_t, nameString, 0x18) < 8u
            ? reinterpret_cast<const uint16_t*>(nameString + 4)
            : reinterpret_cast<const uint16_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, nameString, 0x4)));

        if (MenuMpAccounts_CompareNames(slot, nullptr, 0,
                                        CSF_AT(uint32_t, slot, 0x14),
                                        text,
                                        CSF_AT(uint32_t, nameString, 0x14)) == 0)
        {
            matchIndex = slotIndex;
        }

        result = matchIndex;
        ++slotIndex;
        slot += 48;
    }
    while (slotIndex < 30);

    return result;
}

HOOK(0x0074A4C0, C_00867900_FindEntryByPlayerName, "auto");
