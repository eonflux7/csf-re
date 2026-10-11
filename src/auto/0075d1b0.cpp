#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, LookupGraphEntryByKey, 0x0075CF20, (void* self, void* edx, uint32_t* a2));

static bool __fastcall C_0086c288_MatchesLookupResultId(C_0086c288* self, void* /*edx*/, uint32_t* a2)
{
    int entry = LookupGraphEntryByKey(self, nullptr, a2);
    uint8_t* entryBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(entry)));
    uint32_t entryId = CSF_AT(uint32_t, entryBase, 0x18);
    return entryId == self->field_001c;
}

HOOK(0x0075D1B0, C_0086c288_MatchesLookupResultId, "auto");
