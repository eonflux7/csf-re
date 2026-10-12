#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, S_004e0de0_GetPosition, 0x004E0DE0,
         (void* self, void* edx, float* a2));
CSF_FUNC(double, __cdecl, RwV3dLength, 0x00623360, (float* a1));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0,
         (C_00833b88* self, void*));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_SoundActivityThreshold, 0x0082E114);

static void __fastcall S_004e0de0_MovePointTowardPosition(
    S_004e0de0* self, void* /*edx*/, float* point)
{
    // The packet also catalogs this direct-call address as a vtable.
    (void)CSF_VTBL(0x004E0DE0);

    alignas(8) uint8_t positionBytes[0x18];
    alignas(8) uint8_t differenceBytes[0xC];

    float* position = reinterpret_cast<float*>(positionBytes);
    float* direction = reinterpret_cast<float*>(positionBytes + 0xC);
    float* difference = reinterpret_cast<float*>(differenceBytes);

    S_004e0de0_GetPosition(self, nullptr, position);

    direction[0] = position[0] - point[0];
    direction[1] = position[1] - point[1];
    direction[2] = position[2] - point[2];
    NormalizeVector3(reinterpret_cast<C_00833b88*>(direction), nullptr);

    difference[0] = point[0] - position[0];
    difference[1] = point[1] - position[1];
    difference[2] = point[2] - position[2];

    const double distance = RwV3dLength(difference);
    float displacementX;
    float displacementY;
    float displacementZ;

    if (distance < g_SoundActivityThreshold)
    {
        const float scale = static_cast<float>(distance * g_HalfDurationScale);
        displacementX = direction[0] * scale;
        displacementY = direction[1] * scale;
        displacementZ = direction[2] * scale;
    }
    else
    {
        displacementX = direction[0] * g_SoundActivityThreshold;
        displacementY = direction[1] * g_SoundActivityThreshold;
        displacementZ = direction[2] * g_SoundActivityThreshold;
    }

    direction[2] = displacementZ;
    point[0] = displacementX + point[0];
    point[1] = displacementY + point[1];
    point[2] = direction[2] + point[2];
}

HOOK(0x004E0E90, S_004e0de0_MovePointTowardPosition, "auto");
