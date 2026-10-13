#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_004dc8f0_FindEntryIndex, 0x004DC8F0, (void* self, void* edx, uint32_t* a2));

static bool __fastcall S_004dc8f0_IsLookupEntryUnflagged(S_004dc8f0* self, void* /*edx*/, uint32_t* a2)
{
    int entryIndex = S_004dc8f0_FindEntryIndex(self, nullptr, a2);
    if (entryIndex == -1)
        return false;

    uint8_t* table = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0068));
    uint32_t entryPtr = CSF_AT(uint32_t, table, 16 * entryIndex + 8);
    if (entryPtr == 0)
        return false;

    uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(entryPtr));
    uint8_t flags = CSF_AT(uint8_t, entry, 4);
    return (flags & 1) == 0;
}

HOOK(0x004DCDC0, S_004dc8f0_IsLookupEntryUnflagged, "auto");
