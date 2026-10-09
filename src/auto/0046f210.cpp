#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, MissionScript_MatchesEntryData, 0x00472780,
         (void* self, void* edx, int a2));

static int __fastcall S_0046f130_FindMatchingUnresolvedEntry(
    S_0046f130* self, void* /*edx*/, int entryAddress)
{
    const uint32_t begin = self->field_0004;
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_0008 - begin) >> 2)
        : 0;

    for (uint32_t index = 0; index < count; ++index)
    {
        void* entry = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_0004), index * 4);

        if (!entry || entry == reinterpret_cast<void*>(entryAddress))
            continue;

        if (CSF_AT(uint16_t, entry, 0x2) != 0xFFFF)
            continue;

        void* data = CSF_AT(void*, entry, 0x4);
        if (!data || CSF_AT(int, data, 0x8) != -1)
            continue;

        const int targetDataAddress =
            CSF_AT(int, reinterpret_cast<void*>(entryAddress), 0x4);
        void* targetData = reinterpret_cast<void*>(targetDataAddress);

        if (CSF_AT(uint8_t, CSF_AT(void*, data, 0x4), 0) ==
                CSF_AT(uint8_t, CSF_AT(void*, targetData, 0x4), 0) &&
            MissionScript_MatchesEntryData(data, nullptr, targetDataAddress))
        {
            return static_cast<int>(index);
        }
    }

    return -1;
}

HOOK(0x0046F210, S_0046f130_FindMatchingUnresolvedEntry, "auto");
