#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* lowerBound, float* upperBound));

// Stores the raw dword in the object at +0x38, then clamps that value to [0.0f, 1.0f] in place.
static int __fastcall ParticleRender_SetValueClampedToUnit(void* self, void* /*edx*/, int rawValueBits)
{
    CSF_AT(int, self, 0x38) = rawValueBits;

    float lowerBound = 0.0f;
    float upperBound = 1.0f;
    ClampFloatToBounds(&CSF_AT(float, self, 0x38), &lowerBound, &upperBound);

    // The original returns whatever eax holds after the cdecl call; ClampFloatToBounds
    // is bound as void, so no meaningful value is available here.
    return 0;
}

HOOK(0x007052C0, ParticleRender_SetValueClampedToUnit, "auto");
