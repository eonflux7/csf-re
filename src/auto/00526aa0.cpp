#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(double, __cdecl, RwV3dLength, 0x00623360, (float* a1));

static int __fastcall AudioSoundManager_UpdateDistanceThreshold(
    void* self, void* /*edx*/, float* position, float distanceStep)
{
    bool recalculate;
    if (CSF_AT(uint32_t, self, 0x60))
    {
        float distance = distanceStep + CSF_AT(float, self, 0x40);
        CSF_AT(float, self, 0x40) = distance;
        recalculate = distance > CSF_AT(float, self, 0x44);
    }
    else
    {
        float distance = CSF_AT(float, self, 0x40) - distanceStep;
        CSF_AT(float, self, 0x40) = distance;
        recalculate = distance < CSF_AT(float, self, 0x44);
    }

    if (recalculate)
    {
        float displacement[3];
        displacement[0] = CSF_AT(float, self, 0x1C) - position[0];
        displacement[1] = CSF_AT(float, self, 0x20) - position[1];
        displacement[2] = CSF_AT(float, self, 0x24) - position[2];
        CSF_AT(float, self, 0x40) =
            static_cast<float>(RwV3dLength(displacement));
    }

    return CSF_AT(float, self, 0x40) < CSF_AT(float, self, 0x44);
}

HOOK(0x00526AA0, AudioSoundManager_UpdateDistanceThreshold, "auto");
