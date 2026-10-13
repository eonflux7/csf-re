#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* lowerBound, float* upperBound));
CSF_FUNC(double, __cdecl, RwV3dNormalize, 0x00623250, (float* a1, float* a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_DegreesToRadians, 0x00862468);

static int __fastcall C_00862490_UpdateAngularWeight(
    C_00862490* self, void* /*edx*/, float* origin, float* target)
{
    alignas(8) uint8_t directionBytes[0xC];
    float* direction = reinterpret_cast<float*>(directionBytes);

    direction[0] = target[0] - origin[0];
    direction[1] = target[1] - origin[1];
    direction[2] = target[2] - origin[2];
    RwV3dNormalize(direction, direction);

    void* settings = reinterpret_cast<void*>(self->field_0018);
    const float outerCosine = static_cast<float>(std::cos(
        static_cast<double>(CSF_AT(float, settings, 0x60)) *
        g_DegreesToRadians));

    const double numerator =
        static_cast<double>(direction[1]) * self->field_0030 +
        static_cast<double>(direction[0]) * self->field_002c +
        static_cast<double>(direction[2]) * self->field_0034 -
        outerCosine;
    const double denominator = std::cos(
        static_cast<double>(CSF_AT(float, settings, 0x5C)) *
        g_DegreesToRadians) - outerCosine;

    float lowerBound = 0.0f;
    float upperBound = 1.0f;
    self->field_0038 = static_cast<float>(numerator / denominator);
    ClampFloatToBounds(&self->field_0038, &lowerBound, &upperBound);

    return self->field_0038 > g_fZero;
}

HOOK(0x006F1CB0, C_00862490_UpdateAngularWeight, "auto");
