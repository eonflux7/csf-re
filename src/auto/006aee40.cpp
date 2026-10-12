#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_FUNC(double, __cdecl, CalculateArcCosine, 0x0040C980, (float a1));

// Angle in radians between two 2D vectors: arccos(dot / (|a| * |b|)).
// The original leaves the callee's double result in st(0) and returns it
// unchanged, so this function returns double as well.
static double Vec2_ComputeAngleBetween(float* a1, float* a2)
{
    float dotProduct = a1[1] * a2[1] + a1[0] * a2[0];
    float magnitudeB = std::sqrt(a2[0] * a2[0] + a2[1] * a2[1]);
    float magnitudeA = std::sqrt(a1[0] * a1[0] + a1[1] * a1[1]);
    float cosine = dotProduct / (magnitudeB * magnitudeA);
    return CalculateArcCosine(cosine);
}

HOOK(0x006AEE40, Vec2_ComputeAngleBetween, "auto");
