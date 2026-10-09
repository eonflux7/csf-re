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

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

// Copies the zero vector into the sub-object at owner->+4 +4 and sets two
// int slots (+0x10 and +0x14) of the owner->+4 object to -1. Returns owner->+4.
static int __stdcall ResetVectorAndInvalidateIndices(int a1, int a2, int a3)
{
    uint8_t* owner = reinterpret_cast<uint8_t*>(a1);
    uint8_t* target = CSF_AT(uint8_t*, owner, 4);

    float* zeroedVector = reinterpret_cast<float*>(target + 4);
    zeroedVector[0] = g_ZeroVectorX;
    zeroedVector[1] = g_ZeroVectorY;
    zeroedVector[2] = g_ZeroVectorZ;

    CSF_AT(int32_t, CSF_AT(uint8_t*, owner, 4), 0x10) = -1;

    uint8_t* result = CSF_AT(uint8_t*, owner, 4);
    CSF_AT(int32_t, result, 0x14) = -1;

    return static_cast<int>(reinterpret_cast<uintptr_t>(result));
}

HOOK(0x0045AD60, ResetVectorAndInvalidateIndices, "auto");
