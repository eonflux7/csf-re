#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GfxMatrix_AreVectorsDifferent, 0x0040FB20, (float* self, void* , float* other));
CSF_FUNC(int, __cdecl, RpTeamPlayerSetActive, 0x007FEE10, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, _rpTeamPlayerSelect, 0x007FEE60, (uint32_t* a1, int a2));
CSF_FUNC(int, __cdecl, _rpTeamPlayerDeselect, 0x007FEF10, (int a1, int a2));

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

static int __fastcall C_008630e0_SetPlayerVector(
    C_008630e0* self, void* /*edx*/, int vectorAddress)
{
    int result = static_cast<int>(self->field_0004);
    if (!result)
        return result;

    auto* player = reinterpret_cast<uint8_t*>(self->field_0004);
    if (!CSF_AT(void*, player, 0x24))
        return result;

    uint32_t playerIndex = CSF_AT(uint16_t, player, 0x20);
    auto* team = CSF_AT(uint8_t*, player, 0x24);
    auto* playerBits = CSF_AT(uint8_t*, team, 0x64);
    result = static_cast<int>(reinterpret_cast<uintptr_t>(playerBits));

    if ((CSF_AT(uint32_t, playerBits, 4 * (playerIndex >> 5)) &
         (uint32_t{1} << (playerIndex & 0x1F))) == 0)
        return result;

    auto* vector = reinterpret_cast<float*>(static_cast<uint32_t>(vectorAddress));
    if (static_cast<uint8_t>(
            GfxMatrix_AreVectorsDifferent(vector, nullptr, &g_ZeroVectorX)) == 0)
    {
        result = reinterpret_cast<int (__fastcall*)(C_008630e0*, void*)>(
            CSF_AT(void**, self, 0)[0x10 / 4])(self, nullptr);
        if (result != 1)
        {
            player = reinterpret_cast<uint8_t*>(self->field_0004);
            return _rpTeamPlayerDeselect(
                CSF_AT(int, player, 0x24),
                static_cast<int>(reinterpret_cast<uintptr_t>(player)));
        }
        return result;
    }

    player = reinterpret_cast<uint8_t*>(self->field_0004);
    playerIndex = CSF_AT(uint16_t, player, 0x20);
    team = CSF_AT(uint8_t*, player, 0x24);
    playerBits = CSF_AT(uint8_t*, team, 0x60);

    if ((CSF_AT(uint32_t, playerBits, 4 * (playerIndex >> 5)) &
         (uint32_t{1} << (playerIndex & 0x1F))) == 0)
    {
        player = reinterpret_cast<uint8_t*>(self->field_0004);
        _rpTeamPlayerSelect(
            CSF_AT(uint32_t*, player, 0x24),
            static_cast<int>(reinterpret_cast<uintptr_t>(player)));

        player = reinterpret_cast<uint8_t*>(self->field_0004);
        result = CSF_AT(int, player, 0x24);
        if (!result)
            return result;

        playerIndex = CSF_AT(uint16_t, player, 0x20);
        team = CSF_AT(uint8_t*, player, 0x24);
        playerBits = CSF_AT(uint8_t*, team, 0x60);
        result = static_cast<int>(reinterpret_cast<uintptr_t>(playerBits));
        if ((CSF_AT(uint32_t, playerBits, 4 * (playerIndex >> 5)) &
             (uint32_t{1} << (playerIndex & 0x1F))) == 0)
            return result;

        player = reinterpret_cast<uint8_t*>(self->field_0004);
        RpTeamPlayerSetActive(
            CSF_AT(int, player, 0x24),
            static_cast<int>(reinterpret_cast<uintptr_t>(player)), 1);
    }

    player = reinterpret_cast<uint8_t*>(self->field_0004);
    result = CSF_AT(uint16_t, player, 0x0C);
    if (!result)
        return result;

    int remaining = result;
    uint32_t entryOffset = 0;
    do
    {
        player = reinterpret_cast<uint8_t*>(self->field_0004);
        auto* entries = CSF_AT(uint8_t*, player, 0x10);
        auto* entry = CSF_AT(uint8_t*, entries, entryOffset + 4);
        result = static_cast<int>(reinterpret_cast<uintptr_t>(entry));
        if (entry)
        {
            auto* owner = CSF_AT(uint8_t*, entry, 0);
            uint32_t index = CSF_AT(uint32_t, entry, 4);
            auto* storage = CSF_AT(uint8_t*, owner, 0x10);
            auto* destination = CSF_AT(uint8_t*, storage, 8) + 32 * index;
            auto* source = reinterpret_cast<uint8_t*>(
                static_cast<uint32_t>(vectorAddress));

            CSF_AT(uint32_t, destination, 0) = CSF_AT(uint32_t, source, 0);
            CSF_AT(uint32_t, destination, 4) = CSF_AT(uint32_t, source, 4);
            result = CSF_AT(int, source, 8);
            CSF_AT(int, destination, 8) = result;
        }
        entryOffset += 12;
        --remaining;
    } while (remaining);

    return result;
}

HOOK(0x006FFCA0, C_008630e0_SetPlayerVector, "auto");
