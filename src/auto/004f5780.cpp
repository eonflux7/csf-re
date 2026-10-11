#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fHudFadeStep, 0x0082E04C);

static int __stdcall CharacterModel_ComputeLimitedPosition(
    float* output, float* origin, float* target,
    float* direction, float* offset)
{
    const float dx = target[0] - origin[0];
    const float dy = target[1] - origin[1];
    const float dz = target[2] - origin[2];
    const float distance = std::sqrt(dz * dz + dx * dx + dy * dy);

    // Unordered comparisons take the limited-distance branch as well.
    if (!(distance < g_fHudFadeStep))
    {
        const float scaledX = direction[0] * g_fHudFadeStep;
        const float scaledY = direction[1] * g_fHudFadeStep;
        const float scaledZ = direction[2] * g_fHudFadeStep;
        const float x = scaledX + origin[0];
        const float y = scaledY + origin[1];
        const float z = scaledZ + origin[2];

        output[0] = x;
        output[1] = y;
        output[2] = z;

        int result;
        std::memcpy(&result, &z, sizeof(result));
        return result;
    }

    const float x = target[0] + offset[0];
    const float y = offset[1] + target[1];
    const float z = offset[2] + target[2];

    output[0] = x;
    output[1] = y;
    output[2] = z;
    return static_cast<int>(reinterpret_cast<std::intptr_t>(output));
}

HOOK(0x004F5780, CharacterModel_ComputeLimitedPosition, "auto");
