#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(double, __cdecl, RwV3dLength, 0x00623360, (float* a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall S_00526d40_FindAvailableSoundInstance(
    S_00526d40* self, void* /*edx*/, void* request)
{
    if (!self->field_0000)
        return 0;

    float scratch[3] = {};
    float difference[3] = {};
    float listenerPosition[8] = {};

    void* listener = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
    ((void(__fastcall*)(void*, void*, float*))
        CSF_AT(void**, listener, 0)[0x44 / 4])(
            listener, nullptr, listenerPosition);

    // The negative-handle path leaves the original argument bits as the metric.
    float requestDistance;
    std::memcpy(&requestDistance, &request, sizeof(requestDistance));

    switch (CSF_AT(uint32_t, request, 0))
    {
    case 0:
    case 4:
        requestDistance = 0.0f;
        break;

    case 1:
        difference[0] = CSF_AT(float, request, 0x18) - listenerPosition[0];
        difference[1] = CSF_AT(float, request, 0x1C) - listenerPosition[1];
        difference[2] = CSF_AT(float, request, 0x20) - listenerPosition[2];
        requestDistance = static_cast<float>(RwV3dLength(difference));
        break;

    case 2:
        if (CSF_AT(int, request, 0x2C) >= 0)
        {
            int resolved = CHandleTable_Resolve(
                g_pCurrentScene, nullptr,
                static_cast<int>(reinterpret_cast<intptr_t>(
                    static_cast<uint8_t*>(request) + 0x24)));
            if (!resolved)
            {
                requestDistance = -1.0f;
                break;
            }

            int position = GetObjectPositionVector(
                reinterpret_cast<void*>(resolved), nullptr);
            float* displacement = BuildFacingVectorBetween(
                reinterpret_cast<void*>(position), nullptr,
                scratch, listenerPosition);
            requestDistance = static_cast<float>(RwV3dLength(displacement));
        }
        break;

    default:
        requestDistance = -1.0f;
        break;
    }

    const int requestedPriority = CSF_AT(int, request, 0x4);
    if (requestDistance == g_MissingMetricValue)
        return 0;

    int index = CSF_AT(int, self, 0x10);
    uint32_t offset = 112 * static_cast<uint32_t>(index);
    void* instance = reinterpret_cast<void*>(self->field_0000 + offset);

    int replacementIndex = -1;
    float replacementDistance = 0.0f;
    int replacementPriority = 4;
    bool exhausted = false;

    while (CSF_AT(uint8_t, instance, 0x3C))
    {
        int priority = ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, instance, 0)[0x58 / 4])(instance, nullptr);

        if (priority < requestedPriority && priority <= replacementPriority)
        {
            instance = reinterpret_cast<void*>(self->field_0000 + offset);
            float* position = ((float*(__fastcall*)(void*, void*, float*))
                CSF_AT(void**, instance, 0)[0x54 / 4])(
                    instance, nullptr, scratch);

            difference[2] = position[2] - listenerPosition[2];
            difference[1] = position[1] - listenerPosition[1];
            difference[0] = position[0] - listenerPosition[0];
            double distance = RwV3dLength(difference);

            if (priority < replacementPriority ||
                (priority == replacementPriority &&
                 distance > replacementDistance))
            {
                replacementDistance = static_cast<float>(distance);
                replacementIndex = index;
                replacementPriority = priority;
            }
        }

        ++index;
        if (index == CSF_AT(int, self, 0xC))
        {
            if (!CSF_AT(int, self, 0x10))
            {
                exhausted = true;
                break;
            }
            index = 0;
        }
        if (index == CSF_AT(int, self, 0x10))
        {
            exhausted = true;
            break;
        }

        offset = 112 * static_cast<uint32_t>(index);
        instance = reinterpret_cast<void*>(self->field_0000 + offset);
    }

    if (!exhausted && index != -1)
        return static_cast<int>(
            self->field_0000 + 112 * static_cast<uint32_t>(index));

    if (replacementDistance > requestDistance)
    {
        void* observer = CSF_AT(void*, self, 0x14);
        index = replacementIndex;
        if (observer)
        {
            void* replacement = reinterpret_cast<void*>(
                self->field_0000 + 112 * static_cast<uint32_t>(index));
            ((void(__fastcall*)(void*, void*, void*))
                CSF_AT(void**, observer, 0)[0x4 / 4])(
                    observer, nullptr, replacement);
        }

        offset = 112 * static_cast<uint32_t>(replacementIndex);
        instance = reinterpret_cast<void*>(self->field_0000 + offset);
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, instance, 0)[0x2C / 4])(instance, nullptr);

        instance = reinterpret_cast<void*>(self->field_0000 + offset);
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, instance, 0)[0x24 / 4])(instance, nullptr);

        if (replacementIndex != -1)
            return static_cast<int>(
                self->field_0000 + 112 * static_cast<uint32_t>(index));
    }

    return 0;
}

HOOK(0x00526D40, S_00526d40_FindAvailableSoundInstance, "auto");
