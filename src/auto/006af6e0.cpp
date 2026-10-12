#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, sub_6AEF40, 0x006AEF40, (void* self, void* edx, float a2, float* a3));
CSF_FUNC(int, __fastcall, sub_6AF250, 0x006AF250, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_6AF520, 0x006AF520, (uint32_t* a1));

CSF_GLOBAL(float, g_DistanceSampleStep, 0x008A7DFC);

static int __fastcall S_006aef40_BuildDistanceTable(
    S_006aef40* self, void* /*edx*/, int durationBits)
{
    std::memcpy(&self->field_0044, &durationBits, sizeof(durationBits));
    sub_6AF250(self, nullptr);
    sub_6AF520(reinterpret_cast<uint32_t*>(self));

    auto* distanceArray = reinterpret_cast<S_0041e5f0*>(
        reinterpret_cast<uint8_t*>(self) + 0x34);

    if (CSF_AT(uint32_t, self, 0x3C) >= CSF_AT(uint32_t, self, 0x38))
    {
        S_0041e5f0_GrowArrayCapacity(
            distanceArray, nullptr, CSF_AT(int, self, 0x40));
    }

    uint32_t count = CSF_AT(uint32_t, self, 0x3C);
    float* destination =
        reinterpret_cast<float*>(self->field_0034) + count;
    CSF_AT(uint32_t, self, 0x3C) = count + 1;
    *destination = 0.0f;

    int sampleIndex = 1;
    do
    {
        alignas(8) uint8_t currentPositionBytes[0xC] = {};
        alignas(8) uint8_t previousPositionBytes[0xC] = {};
        float* currentPosition =
            reinterpret_cast<float*>(currentPositionBytes);
        float* previousPosition =
            reinterpret_cast<float*>(previousPositionBytes);

        sub_6AEF40(self, nullptr,
                  static_cast<float>(sampleIndex) * g_DistanceSampleStep,
                  currentPosition);
        sub_6AEF40(self, nullptr,
                  static_cast<float>(sampleIndex - 1) * g_DistanceSampleStep,
                  previousPosition);

        const float dx = currentPosition[0] - previousPosition[0];
        const float dy = currentPosition[1] - previousPosition[1];
        const float dz = currentPosition[2] - previousPosition[2];
        const float accumulatedDistance =
            std::sqrt(dz * dz + dy * dy + dx * dx) +
            reinterpret_cast<float*>(self->field_0034)[sampleIndex - 1];

        if (CSF_AT(uint32_t, self, 0x3C) >=
            CSF_AT(uint32_t, self, 0x38))
        {
            S_0041e5f0_GrowArrayCapacity(
                distanceArray, nullptr, CSF_AT(int, self, 0x40));
        }

        count = CSF_AT(uint32_t, self, 0x3C);
        destination = reinterpret_cast<float*>(self->field_0034) + count;
        CSF_AT(uint32_t, self, 0x3C) = count + 1;
        ++sampleIndex;
        *destination = accumulatedDistance;
    }
    while (sampleIndex <= 100);

    const float totalDistance =
        CSF_AT(float, reinterpret_cast<void*>(self->field_0034), 400);
    void* initialPosition = reinterpret_cast<void*>(self->field_0004);
    const float averageSpeed = totalDistance / self->field_0044;
    self->field_0054 = 0;
    self->field_0048 = averageSpeed;

    CSF_AT(uint32_t, self, 0x58) =
        CSF_AT(uint32_t, initialPosition, 0);
    CSF_AT(uint32_t, self, 0x5C) =
        CSF_AT(uint32_t, initialPosition, 4);
    const int result = CSF_AT(int, initialPosition, 8);
    CSF_AT(uint32_t, self, 0x60) = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x006AF6E0, S_006aef40_BuildDistanceTable, "auto");
