#include <cmath>
#include <cstdint>

#include "globals.h"
#include "hook.h"

// Vector_GetLength: returns the Euclidean length of a 3-float vector.
// Original is __thiscall (ecx = this, no stack args); ecx is preserved by the caller.
static long double __fastcall Vector_GetLength(float* self, void* /*edx*/) {
    const float x = self[0];
    const float y = self[1];
    const float z = self[2];
    return std::sqrt(x * x + y * y + z * z);
}

HOOK_KEEP_ECX(0x401FA0, Vector_GetLength, 0, "game");
