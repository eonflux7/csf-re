#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));

// Writes a two-float pair into `outPair`: [0] = resolved scalar + field_01a8, [1] = field_01ac.
static float* __fastcall C_008342e8_GetResolvedOffsetPair(C_008342e8* self, void* /*edx*/, float* outPair)
{
    double resolvedScalar = C_008342e8_ComputeResolvedObjectScalar(self, nullptr);
    float storedY = *reinterpret_cast<float*>(&self->field_01ac);
    outPair[0] = static_cast<float>(resolvedScalar) + *reinterpret_cast<float*>(&self->field_01a8);
    outPair[1] = storedY;
    return outPair;
}

HOOK(0x0049EC10, C_008342e8_GetResolvedOffsetPair, "auto");
