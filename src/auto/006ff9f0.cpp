#include <cstdint>
#include <cstddef>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));
CSF_FUNC(int, __cdecl, RpTeamPlayerSetActive, 0x007FEE10, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, _rpTeamPlayerSelect, 0x007FEE60, (uint32_t* a1, int a2));
CSF_FUNC(uint32_t*, __cdecl, GfxTeam_ApplyPlayerMovement, 0x00802410, (uint32_t* a1, float* a2, float* a3));

CSF_GLOBAL(float, g_SpeedUnitScale, 0x0082DD6C);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);

static bool GfxTeam_IsPlayerSelected(uint8_t* player, uint8_t* team)
{
    if (!team)
        return false;

    const uint16_t index = CSF_AT(uint16_t, player, 0x20);
    auto* selectionBits = CSF_AT(uint32_t*, team, 0x60);
    return (selectionBits[index >> 5] & (1u << (index & 0x1F))) != 0;
}

static char __fastcall C_008630e0_UpdateBase(
    C_008630e0* self, void* /*edx*/, float elapsedTime)
{
    char hasRemainingMovement = 0;

    if (self->field_0012)
    {
        auto* player = reinterpret_cast<uint8_t*>(self->field_0004);
        auto* team = CSF_AT(uint8_t*, player, 0x24);
        _rpTeamPlayerSelect(reinterpret_cast<uint32_t*>(team),
                            static_cast<int>(self->field_0004));

        player = reinterpret_cast<uint8_t*>(self->field_0004);
        team = CSF_AT(uint8_t*, player, 0x24);
        if (!GfxTeam_IsPlayerSelected(player, team))
            return 0;

        RpTeamPlayerSetActive(reinterpret_cast<int>(team),
                              reinterpret_cast<int>(player), 1);
        self->field_0012 = 0;
    }

    if (!self->field_001c)
        return hasRemainingMovement;

    auto* player = reinterpret_cast<uint8_t*>(self->field_0004);
    auto* team = CSF_AT(uint8_t*, player, 0x24);
    if (!GfxTeam_IsPlayerSelected(player, team))
    {
        _rpTeamPlayerSelect(reinterpret_cast<uint32_t*>(team),
                            reinterpret_cast<int>(player));

        player = reinterpret_cast<uint8_t*>(self->field_0004);
        team = CSF_AT(uint8_t*, player, 0x24);
        if (!GfxTeam_IsPlayerSelected(player, team))
            return 0;

        RpTeamPlayerSetActive(reinterpret_cast<int>(team),
                              reinterpret_cast<int>(player), 1);
    }

    int32_t index = static_cast<int32_t>(self->field_001c - 1);
    if (index < 0)
        return hasRemainingMovement;

    alignas(8) uint8_t stepBytes[0x1C];
    alignas(8) uint8_t scaledStepBytes[0xC];
    alignas(8) uint8_t scaledRemainderBytes[0xC];
    auto* step = reinterpret_cast<float*>(stepBytes);
    auto* scaledStep = reinterpret_cast<float*>(scaledStepBytes);
    auto* scaledRemainder = reinterpret_cast<float*>(scaledRemainderBytes);

    uint32_t byteOffset = 32u * static_cast<uint32_t>(index);
    for (;;)
    {
        auto* record =
            reinterpret_cast<uint8_t*>(self->field_0014) + byteOffset;
        player = reinterpret_cast<uint8_t*>(self->field_0004);
        const uint32_t elementIndex = CSF_AT(uint32_t, record, 0x1C);
        auto* elements = CSF_AT(uint8_t*, player, 0x10);
        auto* element = CSF_AT(uint32_t*, elements + 12u * elementIndex, 0x4);

        bool removeRecord = true;
        if (element)
        {
            const bool singleElement = CSF_AT(uint16_t, player, 0xC) <= 1u;
            const float length = std::sqrt(
                CSF_AT(float, record, 0x0) * CSF_AT(float, record, 0x0) +
                CSF_AT(float, record, 0x4) * CSF_AT(float, record, 0x4) +
                CSF_AT(float, record, 0x8) * CSF_AT(float, record, 0x8));

            if (!singleElement && length > g_SpeedUnitScale)
            {
                const float fraction = g_SpeedUnitScale / length;
                step[2] = fraction * CSF_AT(float, record, 0x8);
                step[1] = fraction * CSF_AT(float, record, 0x4);
                step[0] = fraction * CSF_AT(float, record, 0x0);

                auto* position = reinterpret_cast<float*>(record + 0xC);
                if (CSF_AT(uint8_t, record, 0x18))
                {
                    const float timeScale = g_ProgressScale / elapsedTime;
                    scaledStep[0] = step[0] * timeScale;
                    scaledStep[1] = step[1] * timeScale;
                    scaledStep[2] = step[2] * timeScale;
                    GfxTeam_ApplyPlayerMovement(element, scaledStep, position);
                }
                else
                {
                    GfxTeam_ApplyPlayerMovement(element, step, position);
                }

                record = reinterpret_cast<uint8_t*>(self->field_0014) + byteOffset;
                hasRemainingMovement = 1;
                CSF_AT(float, record, 0x0) -= step[0];
                CSF_AT(float, record, 0x4) -= step[1];
                CSF_AT(float, record, 0x8) -= step[2];

                auto* owner = CSF_AT(uint8_t*, element, 0x0);
                auto* matrixTable = CSF_AT(uint8_t*, owner, 0x10);
                auto* matrices = CSF_AT(uint8_t*, matrixTable, 0x0);
                const uint32_t matrixOffset =
                    CSF_AT(uint32_t, element, 0x4) << 6;
                auto* currentPosition = matrices + matrixOffset + 0x30;
                auto* storedPosition =
                    reinterpret_cast<uint8_t*>(self->field_0014) + byteOffset + 0xC;
                CSF_AT(uint32_t, storedPosition, 0x0) =
                    CSF_AT(uint32_t, currentPosition, 0x0);
                CSF_AT(uint32_t, storedPosition, 0x4) =
                    CSF_AT(uint32_t, currentPosition, 0x4);
                CSF_AT(uint32_t, storedPosition, 0x8) =
                    CSF_AT(uint32_t, currentPosition, 0x8);
                removeRecord = false;
            }
            else if (CSF_AT(uint8_t, record, 0x18))
            {
                const float timeScale = g_ProgressScale / elapsedTime;
                scaledRemainder[2] = timeScale * CSF_AT(float, record, 0x8);
                scaledRemainder[1] = timeScale * CSF_AT(float, record, 0x4);
                scaledRemainder[0] = timeScale * CSF_AT(float, record, 0x0);
                GfxTeam_ApplyPlayerMovement(
                    element, scaledRemainder,
                    reinterpret_cast<float*>(record + 0xC));
            }
            else
            {
                GfxTeam_ApplyPlayerMovement(
                    element, reinterpret_cast<float*>(record),
                    reinterpret_cast<float*>(record + 0xC));
            }
        }

        if (removeRecord)
        {
            const uint32_t remainingCount = self->field_001c - 1;
            self->field_001c = remainingCount;
            if (static_cast<uint32_t>(index) != remainingCount)
            {
                auto* records = reinterpret_cast<uint8_t*>(self->field_0014);
                _memcpy(records + byteOffset, records + byteOffset + 32,
                        32u * (remainingCount - static_cast<uint32_t>(index)));
            }
        }

        if (--index < 0)
            return hasRemainingMovement;
        byteOffset -= 32;
    }
}

HOOK(0x006FF9F0, C_008630e0_UpdateBase, "auto");
