#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(bool, __fastcall, MissionScript_MatchesEntry,
    0x004725F0, (void* self, void* edx, int a2));

static bool __fastcall S_004725f0_MatchesEntryData(
    S_004725f0* self, void* /*edx*/, int entry)
{
    const auto* data = reinterpret_cast<const void*>(
        CSF_AT(uint32_t, reinterpret_cast<const void*>(entry), 0x04));

    if (CSF_AT(uint8_t, data, 0x00) == 4)
    {
        const auto* ownData =
            reinterpret_cast<const void*>(self->field_0004);
        return CSF_AT(uint32_t, ownData, 0x10) ==
               CSF_AT(uint32_t, data, 0x10);
    }

    return MissionScript_MatchesEntry(self, nullptr, entry);
}

HOOK(0x00472780, S_004725f0_MatchesEntryData, "auto");
