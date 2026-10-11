#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, AiCore_IsCandidateEligible, 0x004BBAF0, (void* self, void* edx, int a1, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall AiCore_SelectNearestEligibleCandidate(
    void* self, void* /*edx*/, int source)
{
    CSF_AT(int, self, 0x18) = -1;
    CSF_AT(int, self, 0x1C) = -1;

    void* firstSettings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    void* secondSettings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    float nearestDistanceSquared =
        CSF_AT(float, secondSettings, 0xAB8) *
        CSF_AT(float, firstSettings, 0xAB8);

    const uint32_t candidateCount = CSF_AT(uint32_t, g_pCurrentScene, 0x28);
    for (uint32_t index = 0; index < candidateCount; ++index)
    {
        C_008342e8* candidate =
            CSF_AT(C_008342e8**, g_pCurrentScene, 0x20)[index];

        if (!candidate ||
            !AiCore_IsCandidateEligible(
                self, nullptr, source,
                reinterpret_cast<int>(&candidate->field_001c)))
        {
            continue;
        }

        const int linkedValue = GetLinkedObjectValue_4DB5C0(
            reinterpret_cast<void*>(source), nullptr);
        uint8_t* linkedObject = linkedValue
            ? reinterpret_cast<uint8_t*>(linkedValue) - 4
            : nullptr;

        float* sourcePosition = reinterpret_cast<float*>(
            GetObjectPositionVector(
                CSF_AT(void*, linkedObject, 0x2C), nullptr));
        float* candidatePosition = reinterpret_cast<float*>(
            GetObjectPositionVector(candidate, nullptr));

        const float dx = candidatePosition[0] - sourcePosition[0];
        const float dy = candidatePosition[1] - sourcePosition[1];
        const float dz = candidatePosition[2] - sourcePosition[2];
        const float distanceSquared = dz * dz + dy * dy + dx * dx;

        if (distanceSquared < nearestDistanceSquared)
        {
            nearestDistanceSquared = distanceSquared;
            CSF_AT(int, self, 0x18) = candidate->field_0020;
            CSF_AT(int, self, 0x1C) = candidate->field_0024;
        }
    }

    const int result = (CSF_AT(int, self, 0x1C) >= 0) + 2;
    CSF_AT(int, self, 0x04) = result;
    return result;
}

HOOK(0x004BF9C0, AiCore_SelectNearestEligibleCandidate, "auto");
