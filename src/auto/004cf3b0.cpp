#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Obtains the position vector of an object (forwarder; __fastcall, no stack args).
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x48E430, (void* self, void* edx));

// Distance threshold (squared) stored in the game data segment.
CSF_GLOBAL(float, g_ProximityRadiusSquared, 0x83B2BC);

// Returns 1 when the squared distance between this object's linked
// position and the given object's position is NOT >= the threshold
// (unordered results also count as "near", matching the original x87 flags).
static int __fastcall C_0083b1f8_IsWithinProximityOfObject(C_0083b1f8* self, void* /*edx*/, C_008342e8* a2)
{
    float* ownerPosition = GetObjectPositionVector(reinterpret_cast<void*>(self->field_0014), nullptr);
    float* otherPosition = GetObjectPositionVector(a2, nullptr);

    float deltaX = otherPosition[0] - ownerPosition[0];
    float deltaY = otherPosition[1] - ownerPosition[1];
    float deltaZ = otherPosition[2] - ownerPosition[2];

    float squaredDistance = deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;

    return !(squaredDistance >= g_ProximityRadiusSquared) ? 1 : 0;
}

HOOK(0x004CF3B0, C_0083b1f8_IsWithinProximityOfObject, "auto");
