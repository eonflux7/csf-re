#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, Water_GrowEntries, 0x007003B0, (void* self, void* edx, int a2));

static int __fastcall C_008630e0_AppendEntryIfPlayerEnabled(
    C_008630e0* self, void* /*edx*/,
    const uint32_t* first, const uint32_t* second, int value)
{
    uint32_t result = self->field_0004;
    void* player = reinterpret_cast<void*>(result);

    if (CSF_AT(uint32_t, player, 0x24) != 0)
    {
        const uint32_t playerIndex = CSF_AT(uint16_t, player, 0x20);
        void* team = reinterpret_cast<void*>(CSF_AT(uint32_t, player, 0x24));
        result = CSF_AT(uint32_t, team, 0x64);

        const uint32_t* enabledPlayers = reinterpret_cast<const uint32_t*>(result);
        if ((enabledPlayers[playerIndex >> 5] &
             (uint32_t{1} << (playerIndex & 0x1F))) != 0)
        {
            uint32_t entry[8] = {};
            entry[0] = first[0];
            entry[1] = first[1];
            entry[2] = first[2];
            entry[3] = second[0];
            entry[4] = second[1];
            entry[5] = second[2];
            entry[6] = 1;
            entry[7] = static_cast<uint32_t>(value);

            if (CSF_AT(uint32_t, self, 0x1C) >=
                CSF_AT(uint32_t, self, 0x18))
            {
                Water_GrowEntries(
                    reinterpret_cast<uint8_t*>(self) + 0x14,
                    nullptr, static_cast<int>(self->field_0020));
            }

            const uint32_t count = CSF_AT(uint32_t, self, 0x1C);
            void* destination = reinterpret_cast<void*>(
                self->field_0014 + 32 * count);
            result = count + 1;
            CSF_AT(uint32_t, self, 0x1C) = result;
            std::memcpy(destination, entry, 0x20);
        }
    }

    return static_cast<int>(result);
}

HOOK(0x007009E0, C_008630e0_AppendEntryIfPlayerEnabled, "auto");
