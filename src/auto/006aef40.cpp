#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_RotationWrapStep, 0x0082E1E4);
CSF_GLOBAL(float, g_MessageBoxSizeScale, 0x00831430);

static int __fastcall S_006aef40_EvaluateHermite(
    S_006aef40* self, void* /*edx*/, float time, float* result)
{
    const int count = static_cast<int32_t>(self->field_000c);
    const float* times = reinterpret_cast<const float*>(self->field_0014);

    if (time >= times[count - 1])
    {
        const float* last =
            reinterpret_cast<const float*>(self->field_0004) + 3 * count - 3;
        result[0] = last[0];
        result[1] = last[1];
        result[2] = last[2];
        return count - 1;
    }

    int first = 0;
    int second = 0;
    for (int index = 0; index < count; ++index)
    {
        if (index + 1 >= count)
        {
            first = index;
            second = index;
        }
        else if (time <= times[index + 1])
        {
            first = index;
            second = index + 1;
            break;
        }
    }

    const float* positions =
        reinterpret_cast<const float*>(self->field_0004);
    const float* start = positions + 3 * first;
    const float startX = start[0];
    const float startY = start[1];
    const float startZ = start[2];
    const float* end = positions + 3 * second;
    const float endX = end[0];
    const float endY = end[1];
    const float endZ = end[2];

    if (first == second)
    {
        result[0] = startX;
        result[1] = startY;
        result[2] = startZ;
    }
    else
    {
        const float* tangents =
            reinterpret_cast<const float*>(CSF_AT(uint32_t, self, 0x24));
        const float* startTangent = tangents + 3 * first;
        const float startTangentX = startTangent[0];
        const float startTangentY = startTangent[1];
        const float startTangentZ = startTangent[2];
        const float* endTangent = tangents + 3 * second;
        const float endTangentX = endTangent[0];
        const float endTangentY = endTangent[1];
        const float endTangentZ = endTangent[2];

        times = reinterpret_cast<const float*>(self->field_0014);
        const float startTime = times[first];
        const float t = (time - startTime) / (times[second] - startTime);
        const float squared = t * t;
        const float cubed = squared * t;
        const float tripleSquared = squared * g_RotationWrapStep;

        const float startWeight =
            cubed + cubed - tripleSquared + g_TransitionOne;
        const float startTangentWeight = cubed - (squared + squared) + t;
        const float endWeight =
            cubed * g_MessageBoxSizeScale + tripleSquared;
        const float endTangentWeight = cubed - squared;

        result[0] = startX * startWeight
                  + startTangentX * startTangentWeight
                  + endX * endWeight
                  + endTangentX * endTangentWeight;
        result[1] = startY * startWeight
                  + startTangentY * startTangentWeight
                  + endY * endWeight
                  + endTangentY * endTangentWeight;
        result[2] = startZ * startWeight
                  + startTangentZ * startTangentWeight
                  + endZ * endWeight
                  + endTangentZ * endTangentWeight;
    }

    return first;
}

HOOK(0x006AEF40, S_006aef40_EvaluateHermite, "auto");
