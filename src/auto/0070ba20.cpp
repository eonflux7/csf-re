#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x82D444);

// Copies a 3-float vector into the object's float fields at +0x14/+0x18/+0x1C,
// scaling each component by g_HalfDurationScale. Returns the source pointer.
static float* __fastcall C_0086379c_StoreHalfScaledVector(C_0086379c* self, void* /*edx*/, float* sourceVector)
{
    self->field_0014 = sourceVector[0] * g_HalfDurationScale;
    self->field_0018 = sourceVector[1] * g_HalfDurationScale;
    self->field_001c = sourceVector[2] * g_HalfDurationScale;
    return sourceVector;
}

HOOK(0x70BA20, C_0086379c_StoreHalfScaledVector, "auto");
