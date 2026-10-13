#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* lowerBound, float* upperBound));
CSF_FUNC(double, __cdecl, RwV3dNormalize, 0x00623250, (float* a1, float* a2));
CSF_FUNC(char, __cdecl, GfxSkin_ComputeDistanceFade, 0x006C5310, (float* fade, float* position, float* referencePosition, float distanceScale, float maximumDistance, float minimumDistance, float fadeWidth));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_DegreesToRadians, 0x00862618);

static char __fastcall C_00862640_UpdateVisibilityFade(
    C_00862640* self, void* /*edx*/)
{
    alignas(8) uint8_t sphereBytes[0x10];
    alignas(8) uint8_t directionBytes[0xC];
    alignas(8) uint8_t upperBoundBytes[0x4];
    alignas(8) uint8_t distanceParametersBytes[0xC];

    void* source = reinterpret_cast<void*>(self->field_0020);
    void* transform =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, source, 0)[0x14 / 4])(source, nullptr);
    float* position =
        reinterpret_cast<float* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, transform, 0)[0x10 / 4])(transform, nullptr);

    auto* context = reinterpret_cast<uint8_t*>(GetGlobalValue_6C5760());
    float* referencePosition = reinterpret_cast<float*>(context + 0x124);

    source = reinterpret_cast<void*>(self->field_0020);
    transform =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, source, 0)[0x14 / 4])(source, nullptr);

    void* definition = reinterpret_cast<void*>(self->field_0018);
    float* radius = CSF_AT(float, definition, 0x50) <
                            CSF_AT(float, definition, 0x4C)
                        ? &CSF_AT(float, definition, 0x4C)
                        : &CSF_AT(float, definition, 0x50);

    void* spherePosition =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, transform, 0)[0x10 / 4])(transform, nullptr);

    CSF_AT(uint32_t, sphereBytes, 0) = CSF_AT(uint32_t, spherePosition, 0);
    CSF_AT(float, sphereBytes, 0xC) = *radius * g_HalfDurationScale;
    CSF_AT(uint32_t, sphereBytes, 4) = CSF_AT(uint32_t, spherePosition, 4);
    CSF_AT(uint32_t, sphereBytes, 8) = CSF_AT(uint32_t, spherePosition, 8);

    definition = reinterpret_cast<void*>(self->field_0018);
    uint32_t queryFlag = CSF_AT(uint8_t, definition, 0x41) != 0;
    context = reinterpret_cast<uint8_t*>(GetGlobalValue_6C5760());
    void* visibilitySystem = CSF_AT(void*, context, 0xF0);

    char visible =
        reinterpret_cast<char (__fastcall*)(void*, void*, float*, uint32_t*, uint32_t)>(
            CSF_AT(void**, visibilitySystem, 0)[0x6C / 4])(
                visibilitySystem, nullptr,
                reinterpret_cast<float*>(sphereBytes), &self->field_004c, queryFlag);
    if (!visible)
        return visible;

    definition = reinterpret_cast<void*>(self->field_0018);
    CSF_AT(uint32_t, distanceParametersBytes, 0) =
        CSF_AT(uint32_t, definition, 0x64);
    CSF_AT(uint32_t, distanceParametersBytes, 4) =
        CSF_AT(uint32_t, definition, 0x68);
    CSF_AT(uint32_t, distanceParametersBytes, 8) =
        CSF_AT(uint32_t, definition, 0x6C);

    context = reinterpret_cast<uint8_t*>(GetGlobalValue_6C5760());
    float distanceScale = CSF_AT(float, context, 0x130);
    char distanceVisible = GfxSkin_ComputeDistanceFade(
        &self->field_0038, position, referencePosition, distanceScale,
        CSF_AT(float, distanceParametersBytes, 0),
        CSF_AT(float, distanceParametersBytes, 4),
        CSF_AT(float, distanceParametersBytes, 8));

    if (!distanceVisible)
        return distanceVisible;
    definition = reinterpret_cast<void*>(self->field_0018);
    if (CSF_AT(float, definition, 0x60) == g_fZero)
        return distanceVisible;

    float* direction = reinterpret_cast<float*>(directionBytes);
    direction[0] = referencePosition[0] - position[0];
    direction[1] = referencePosition[1] - position[1];
    direction[2] = referencePosition[2] - position[2];
    RwV3dNormalize(direction, direction);

    definition = reinterpret_cast<void*>(self->field_0018);
    CSF_AT(float, upperBoundBytes, 0) = 1.0f;
    CSF_AT(float, distanceParametersBytes, 0) = 0.0f;
    float outerCosine =
        std::cos(CSF_AT(float, definition, 0x60) * g_DegreesToRadians);

    self->field_0034 =
        (direction[1] * self->field_002c +
         direction[0] * self->field_0028 +
         direction[2] * self->field_0030 - outerCosine) /
        (std::cos(CSF_AT(float, definition, 0x5C) * g_DegreesToRadians) -
         outerCosine);

    ClampFloatToBounds(
        &self->field_0034,
        reinterpret_cast<float*>(distanceParametersBytes),
        reinterpret_cast<float*>(upperBoundBytes));
    return self->field_0034 > g_fZero;
}

HOOK(0x006F30B0, C_00862640_UpdateVisibilityFade, "auto");
