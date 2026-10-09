#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, LookupEntryIndex_46F390, 0x0046F390, (void* self, void* edx, int a2));

// Clears the "busy" byte at +0x81. If the flag byte reached through a4
// (*(*a4 + 4) + 1) is zero, re-resolves the entry index stored at +0x1C
// through LookupEntryIndex and writes the table entry's value back to +0x1C.
// Returns the low byte of the flag (or of the resolved value when re-resolved).
static char __fastcall RefreshActiveEntryIndex(void* self, void* /*edx*/,
                                               uint32_t a2, uint32_t a3, uint32_t a4)
{
    (void)a2;
    (void)a3;

    CSF_AT(uint8_t, self, 0x81) = 0;

    uintptr_t ownerPtr = *reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a4));
    uintptr_t infoPtr = *reinterpret_cast<uint32_t*>(ownerPtr + 4);
    uint8_t flagByte = *reinterpret_cast<uint8_t*>(infoPtr + 1);

    char result = static_cast<char>(flagByte);
    if (flagByte == 0) {
        int currentIndex = CSF_AT(int, self, 0x1C);
        void* tableOwner = CSF_AT(void*, self, 0x10);
        int entryIndex = LookupEntryIndex_46F390(tableOwner, nullptr, currentIndex);

        // Original reloads [esi+0x10] after the call; the callee may change state.
        uintptr_t tableBase = CSF_AT(uint32_t, CSF_AT(void*, self, 0x10), 0x60);
        uint32_t entryValue = *reinterpret_cast<uint32_t*>(
            tableBase + static_cast<uint32_t>(entryIndex) * 8u + 4u);

        CSF_AT(uint32_t, self, 0x1C) = entryValue;
        result = static_cast<char>(entryValue);
    }
    return result;
}

HOOK(0x00463600, RefreshActiveEntryIndex, "auto");
