#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void*, uint32_t* output));

CSF_GLOBAL(float, g_GravityScale, 0x0082E1D4);
CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_RotationWrapStep, 0x0082E1E4);

static double __stdcall Vehicle_ComputeWheelTorque(float* origin, float* pointB, float* pointC, float duration)
{
    uint32_t converted[6];
    float normal[3];
    float edges[6];

    edges[0] = pointB[0] - origin[0];
    edges[1] = pointB[1] - origin[1];
    edges[2] = pointB[2] - origin[2];
    edges[3] = pointC[0] - origin[0];
    edges[4] = pointC[1] - origin[1];
    edges[5] = pointC[2] - origin[2];

    float cz = edges[1] * edges[3] - edges[4] * edges[0];
    normal[0] = edges[4] * edges[2] - edges[5] * edges[1];
    normal[2] = cz;
    normal[1] = edges[5] * edges[0] - edges[2] * edges[3];

    NormalizeVector3(normal, nullptr);

    float scaledY = normal[1] * g_GravityScale;

    float* dir = reinterpret_cast<float*>(
        S_00526d40_ConvertDirection(reinterpret_cast<S_00526d40*>(edges), nullptr, converted));

    double c2 = (double)edges[3] * dir[1] - (double)edges[4] * dir[0];
    double c0 = (double)edges[4] * dir[2] - (double)edges[5] * dir[1];
    double sumSq = c2 * c2 + c0 * c0;
    double c1 = (double)edges[5] * dir[0] - (double)edges[3] * dir[2];

    double len = std::sqrt(sumSq + c1 * c1);

    double dur = duration;
    double t = (dur * g_HalfDurationScale + len) / dur;
    double u = (double)g_TransitionOne - t;

    return len * scaledY * g_RotationWrapStep /
           ((u * u * u + t * t * t) * dur * dur);
}

HOOK(0x00418830, Vehicle_ComputeWheelTorque, "auto");
