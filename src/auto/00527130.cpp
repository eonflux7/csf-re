#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, UpdateSoundInstance, 0x00526FA0,
         (void* self, void* edx, float a2));
CSF_FUNC(float*, __fastcall, ProjectPointOntoSegment, 0x006B1240,
         (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

CSF_GLOBAL(float, g_FloatMax, 0x00831090);

static void AudioSoundManager_ConsiderPosition(
    const float* position, const float* listener, uint32_t index,
    int& nearestIndex, int& secondIndex,
    float& nearestDistance, float& secondDistance)
{
    const float dx = position[0] - listener[0];
    const float dy = position[1] - listener[1];
    const float dz = position[2] - listener[2];
    const float distance = dz * dz + dy * dy + dx * dx;

    if (distance < nearestDistance)
    {
        if (nearestIndex != -1)
        {
            secondIndex = nearestIndex;
            secondDistance = nearestDistance;
        }
        nearestDistance = distance;
        nearestIndex = static_cast<int>(index);
    }
    else if (distance < secondDistance)
    {
        secondIndex = static_cast<int>(index);
        secondDistance = distance;
    }
}

static void __fastcall C_00841128_UpdatePosition(
    C_00841128* self, void* /*edx*/, float elapsedTime)
{
    UpdateSoundInstance(self, nullptr, elapsedTime);

    const uint32_t positionCount = self->field_0078;
    if (!positionCount)
        return;

    if (positionCount == 1)
    {
        if (self->m_voice)
        {
            void* voice = reinterpret_cast<void*>(self->m_voice);
            reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
                CSF_AT(void**, voice, 0)[0x1C / 4])(
                    voice, nullptr,
                    reinterpret_cast<float*>(self->field_0070));
        }
        return;
    }

    // Reserve the complete room available to the original output object.
    float listener[0x1C / sizeof(float)] = {};
    void* listenerProvider = reinterpret_cast<void*>(
        static_cast<uintptr_t>(GetGlobalValue_70DDE0()));
    reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, listenerProvider, 0)[0x44 / 4])(
            listenerProvider, nullptr, listener);

    float secondDistance = g_FloatMax;
    float nearestDistance = 3.4028235e38f;
    int nearestIndex = -1;
    int secondIndex = -1;
    uint32_t index = 0;

    if (static_cast<int32_t>(positionCount) >= 4)
    {
        const float* positions =
            reinterpret_cast<const float*>(self->field_0070);
        do
        {
            for (uint32_t offset = 0; offset < 4; ++offset)
            {
                AudioSoundManager_ConsiderPosition(
                    positions + offset * 3, listener, index + offset,
                    nearestIndex, secondIndex,
                    nearestDistance, secondDistance);
            }
            positions += 12;
            index += 4;
        } while (index < positionCount - 3);
    }

    if (index < positionCount)
    {
        const float* position =
            reinterpret_cast<const float*>(self->field_0070 + 12 * index);
        do
        {
            AudioSoundManager_ConsiderPosition(
                position, listener, index, nearestIndex, secondIndex,
                nearestDistance, secondDistance);
            ++index;
            position += 3;
        } while (index < positionCount);
    }

    const float* nearest = reinterpret_cast<const float*>(
        self->field_0070 + 12 * static_cast<uint32_t>(nearestIndex));
    const float* second = reinterpret_cast<const float*>(
        self->field_0070 + 12 * static_cast<uint32_t>(secondIndex));

    float segment[0x18 / sizeof(float)];
    segment[0] = nearest[0];
    segment[1] = nearest[1];
    segment[2] = nearest[2];
    segment[3] = second[0];
    segment[4] = second[1];
    segment[5] = second[2];

    float projected[0xC / sizeof(float)] = {};
    float* position =
        ProjectPointOntoSegment(segment, nullptr, projected, listener);

    void* voice = reinterpret_cast<void*>(self->m_voice);
    if (voice)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, voice, 0)[0x1C / 4])(
                voice, nullptr, position);
    }
}

HOOK(0x00527130, C_00841128_UpdatePosition, "auto");
