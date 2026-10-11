#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_GravityAccel, 0x0082E0EC);
CSF_GLOBAL(float, g_ProjectileSpeed, 0x008317C0);        // 1100.0
CSF_GLOBAL(float, g_InverseProjectileSpeed, 0x008317C4); // 0.00090909092

static float* __cdecl WeaponFirearms_ComputeBallisticVelocity(
    float* velocity, float* origin, float* target)
{
    float displacement[3];
    displacement[0] = target[0] - origin[0];
    displacement[1] = target[1] - origin[1];
    displacement[2] = target[2] - origin[2];

    const float duration = std::sqrt(
        displacement[0] * displacement[0] +
        displacement[2] * displacement[2] +
        displacement[1] * displacement[1]) * g_InverseProjectileSpeed;
    const float verticalDisplacement = displacement[1];

    NormalizeVector3(displacement, nullptr);

    displacement[0] *= g_ProjectileSpeed;
    velocity[0] = displacement[0];
    displacement[2] *= g_ProjectileSpeed;
    displacement[1] =
        duration * g_HalfDurationScale * g_GravityAccel +
        verticalDisplacement / duration;
    velocity[1] = displacement[1];
    velocity[2] = displacement[2];
    return velocity;
}

HOOK(0x00478930, WeaponFirearms_ComputeBallisticVelocity, "auto");
