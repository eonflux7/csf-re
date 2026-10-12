#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, Vec3_NormalizeHelper, 0x006AB830, (void* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, Water_GrowEntries, 0x007003B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GfxTeam_GetElementPosition, 0x00700500, (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RpTeamPlayerSetActive, 0x007FEE10, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, _rpTeamPlayerSelect, 0x007FEE60, (uint32_t* a1, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(double, g_GfxTeamDistanceScale, 0x00863154);

static float GfxTeam_GetElementWeight(void* element)
{
    void* object = CSF_AT(void*, element, 0);
    void* geometry = CSF_AT(void*, object, 0x10);
    void* records = CSF_AT(void*, geometry, 0x20);
    uint32_t index = CSF_AT(uint32_t, element, 4);
    return CSF_AT(float, records, 48 * index + 12);
}

static unsigned int __fastcall C_008630e0_QueueRadialElementDisplacements(
    C_008630e0* self, void* /*edx*/, float strength, float* origin)
{
    uint32_t playerAddress = self->field_0004;
    void* player = reinterpret_cast<void*>(playerAddress);
    uint32_t teamAddress = CSF_AT(uint32_t, player, 0x24);
    if (!teamAddress)
        return playerAddress;

    uint32_t playerIndex = CSF_AT(uint16_t, player, 0x20);
    uint32_t maskAddress =
        CSF_AT(uint32_t, reinterpret_cast<void*>(teamAddress), 0x64);
    if (!(CSF_AT(uint32_t, reinterpret_cast<void*>(maskAddress),
                 4 * (playerIndex >> 5)) & (1u << (playerIndex & 0x1F))))
        return maskAddress;

    player = reinterpret_cast<void*>(self->field_0004);
    playerIndex = CSF_AT(uint16_t, player, 0x20);
    teamAddress = CSF_AT(uint32_t, player, 0x24);
    maskAddress = CSF_AT(uint32_t, reinterpret_cast<void*>(teamAddress), 0x60);

    if (!(CSF_AT(uint32_t, reinterpret_cast<void*>(maskAddress),
                 4 * (playerIndex >> 5)) & (1u << (playerIndex & 0x1F))))
    {
        playerAddress = self->field_0004;
        teamAddress =
            CSF_AT(uint32_t, reinterpret_cast<void*>(playerAddress), 0x24);
        _rpTeamPlayerSelect(reinterpret_cast<uint32_t*>(teamAddress),
                            static_cast<int>(playerAddress));

        player = reinterpret_cast<void*>(self->field_0004);
        teamAddress = CSF_AT(uint32_t, player, 0x24);
        if (!teamAddress)
            return teamAddress;

        playerIndex = CSF_AT(uint16_t, player, 0x20);
        maskAddress =
            CSF_AT(uint32_t, reinterpret_cast<void*>(teamAddress), 0x60);
        if (!(CSF_AT(uint32_t, reinterpret_cast<void*>(maskAddress),
                     4 * (playerIndex >> 5)) & (1u << (playerIndex & 0x1F))))
            return maskAddress;

        playerAddress = self->field_0004;
        teamAddress =
            CSF_AT(uint32_t, reinterpret_cast<void*>(playerAddress), 0x24);
        RpTeamPlayerSetActive(static_cast<int>(teamAddress),
                              static_cast<int>(playerAddress), 1);
    }

    player = reinterpret_cast<void*>(self->field_0004);
    const uint32_t elementCount = CSF_AT(uint16_t, player, 0x0C);
    float totalWeight = g_fZero;
    if (elementCount)
    {
        void* elements = CSF_AT(void*, player, 0x10);
        for (uint32_t index = 0; index < elementCount; ++index)
        {
            void* element = CSF_AT(void*, elements, 12 * index + 4);
            if (element)
                totalWeight += GfxTeam_GetElementWeight(element);
        }
    }

    for (uint32_t index = 0; index < elementCount; ++index)
    {
        player = reinterpret_cast<void*>(self->field_0004);
        void* elements = CSF_AT(void*, player, 0x10);
        void* element = CSF_AT(void*, elements, 12 * index + 4);
        if (!element)
            continue;

        alignas(8) uint8_t positionBytes[0x0C] = {};
        alignas(8) uint8_t directionBytes[0x24] = {};
        GfxTeam_GetElementPosition(
            self, nullptr, reinterpret_cast<int>(positionBytes),
            static_cast<int>(index), reinterpret_cast<int>(origin));

        CSF_AT(float, directionBytes, 0) =
            CSF_AT(float, positionBytes, 0) - origin[0];
        CSF_AT(float, directionBytes, 4) =
            CSF_AT(float, positionBytes, 4) - origin[1];
        CSF_AT(float, directionBytes, 8) =
            CSF_AT(float, positionBytes, 8) - origin[2];

        const float dx = CSF_AT(float, directionBytes, 0);
        const float dy = CSF_AT(float, directionBytes, 4);
        const float dz = CSF_AT(float, directionBytes, 8);
        float distance = std::sqrt(dz * dz + dy * dy + dx * dx);
        if (distance < g_TransitionOne)
            distance = 1.0f;

        Vec3_NormalizeHelper(directionBytes, nullptr, distance);

        const float weight = GfxTeam_GetElementWeight(element);
        const double scaledDistance = distance * g_GfxTeamDistanceScale;
        const double displacementScale =
            std::exp2(-(scaledDistance * scaledDistance) *
                      1.442695040888963407) *
            strength * (weight / totalWeight);

        alignas(8) uint8_t entryBytes[0x20] = {};
        CSF_AT(float, entryBytes, 0) =
            static_cast<float>(CSF_AT(float, directionBytes, 0) * displacementScale);
        CSF_AT(float, entryBytes, 4) =
            static_cast<float>(CSF_AT(float, directionBytes, 4) * displacementScale);
        CSF_AT(float, entryBytes, 8) =
            static_cast<float>(CSF_AT(float, directionBytes, 8) * displacementScale);
        std::memcpy(entryBytes + 12, positionBytes, 0x0C);
        CSF_AT(uint8_t, entryBytes, 0x18) = 1;
        CSF_AT(uint32_t, entryBytes, 0x1C) = index;

        if (CSF_AT(uint32_t, self, 0x1C) >= self->field_0018)
            Water_GrowEntries(reinterpret_cast<uint8_t*>(self) + 0x14,
                              nullptr, static_cast<int>(self->field_0020));

        uint8_t* destination =
            reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x14)) +
            32 * CSF_AT(uint32_t, self, 0x1C)++;
        std::memcpy(destination, entryBytes, 0x20);
    }

    return elementCount;
}

HOOK(0x00700A80, C_008630e0_QueueRadialElementDisplacements, "auto");
