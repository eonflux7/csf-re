#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Computes the offset from the anchor position returned by the virtual getter
// (slot 0x18) to the float triple a2, then forwards that offset vector to the
// virtual dispatcher at slot 0x58. Returns that dispatcher's result.
static int __fastcall C_00860700_DispatchDeltaFromAnchor(C_00860700* self, void* /*edx*/, float* a2)
{
    float* anchor = reinterpret_cast<float*>(static_cast<uintptr_t>(self->vGetField000cPlus64()));

    float deltaVector[3];
    deltaVector[2] = a2[2] - anchor[2];
    deltaVector[1] = a2[1] - anchor[1];
    deltaVector[0] = a2[0] - anchor[0];

    return self->vSlot58(deltaVector);
}

HOOK(0x006BF4E0, C_00860700_DispatchDeltaFromAnchor, "auto");
