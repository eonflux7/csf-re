#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, CalculateArcCosine, 0x0040C980, (float a1));
CSF_FUNC(double, __cdecl, RwV3dLength, 0x00623360, (float* a1));

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_VectorLengthEpsilon, 0x0086005C);

static double __fastcall S_00526d40_GetVectorAngleRadians(
    S_00526d40* self, void* /*edx*/)
{
    float vector[4];
    std::memcpy(&vector[0], &self->field_0000, sizeof(float));
    vector[1] = 0.0f;
    vector[2] = CSF_AT(float, self, 0x08);
    vector[3] = 0.0f;

    const double length = RwV3dLength(vector);
    vector[3] = static_cast<float>(length);

    // x87: the quotient and the products stay in the register (not rounded to float until stored), and the
    // clamp compares that unrounded value
    double cosine = vector[2];
    if (length > g_VectorLengthEpsilon && vector[3] != g_fZero)
    {
        const double inverseLength = static_cast<double>(g_TransitionOne) / static_cast<double>(vector[3]);
        vector[0] = static_cast<float>(static_cast<double>(vector[0]) * inverseLength);
        cosine = inverseLength * static_cast<double>(vector[2]);
    }
    vector[3] = static_cast<float>(cosine);
    if (cosine < g_MissingMetricValue)
        vector[3] = -1.0f;
    else if (cosine > g_TransitionOne)
        vector[3] = 1.0f;

    const double angle = CalculateArcCosine(vector[3]);
    return vector[0] < g_fZero ? -angle : angle;
}

HOOK(0x006AB930, S_00526d40_GetVectorAngleRadians, "auto");

