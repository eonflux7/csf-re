#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());

CSF_GLOBAL(float, g_MenuWidthScale, 0x0083F24C);
CSF_GLOBAL(float, g_ParticleFadeDistance, 0x008627C8);
CSF_GLOBAL(float, g_ParticleRangeNumerator, 0x008627CC);

static bool __cdecl GfxPrtStdParticles_ComputeDistanceFade(
    float* position, float* fade)
{
    *fade = 1.0f;

    auto* globalState = reinterpret_cast<std::uint8_t*>(
        GetGlobalValue_6C5760());
    auto* referencePosition = globalState + 0x124;

    const float dx = CSF_AT(float, referencePosition, 0) - position[0];
    const float dy = CSF_AT(float, referencePosition, 4) - position[1];
    const float dz = CSF_AT(float, referencePosition, 8) - position[2];
    const float distanceSquared = dz * dz + dx * dx + dy * dy;

    globalState = reinterpret_cast<std::uint8_t*>(GetGlobalValue_6C5760());
    const float range =
        g_ParticleRangeNumerator / CSF_AT(float, globalState, 0x130);

    const bool inRange = range * range >= distanceSquared;
    if (!inRange)
        return false;

    const float remainingDistance = range - std::sqrt(distanceSquared);
    if (remainingDistance < g_ParticleFadeDistance)
        *fade = remainingDistance * g_MenuWidthScale;

    return inRange;
}

HOOK(0x006F3D10, GfxPrtStdParticles_ComputeDistanceFade, "auto");
