#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, Water_GrowEntries, 0x007003B0, (void* self, void* edx, int a2));

static int __fastcall C_008630e0_AppendEnabledPlayerEntry(
    C_008630e0* self, void* /*edx*/,
    const uint32_t* firstVector, const uint32_t* secondVector, int value)
{
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(self));
    void* player = reinterpret_cast<void*>(self->field_0004);

    if (!CSF_AT(void*, player, 0x24))
        return result;

    const uint16_t playerIndex = CSF_AT(uint16_t, player, 0x20);
    void* team = CSF_AT(void*, player, 0x24);
    const uint32_t* enabledPlayers = CSF_AT(uint32_t*, team, 0x64);

    if ((enabledPlayers[playerIndex >> 5] &
         (1u << (playerIndex & 0x1F))) == 0)
        return result;

    uint32_t entry[8];
    entry[0] = firstVector[0];
    entry[1] = firstVector[1];
    entry[2] = firstVector[2];
    entry[3] = secondVector[0];
    entry[4] = secondVector[1];
    entry[5] = secondVector[2];
    entry[6] = 0; // Flag is zero; unspecified padding is initialized to zero.
    entry[7] = static_cast<uint32_t>(value);

    if (CSF_AT(uint32_t, self, 0x1C) >= CSF_AT(uint32_t, self, 0x18))
    {
        Water_GrowEntries(
            reinterpret_cast<uint8_t*>(self) + 0x14,
            nullptr, static_cast<int>(self->field_0020));
    }

    const uint32_t count = CSF_AT(uint32_t, self, 0x1C);
    void* destination = reinterpret_cast<void*>(self->field_0014 + 32u * count);
    const uint32_t newCount = count + 1;
    CSF_AT(uint32_t, self, 0x1C) = newCount;
    std::memcpy(destination, entry, 0x20u);

    return static_cast<int>(newCount);
}

HOOK(0x00700930, C_008630e0_AppendEnabledPlayerEntry, "auto");
