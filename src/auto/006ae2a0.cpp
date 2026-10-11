#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_6ADA10, 0x006ADA10, (void* self, void* edx, uint32_t* a2, uint32_t* a3, uint32_t* a4, uint32_t* a5));

CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

// Builds a rotation basis from angle a2 (radians) and forwards it to sub_6ADA10:
// first vector = (cos, 0, -sin), second vector = (sin, 0, cos).
static int __fastcall GfxMatrix_SetYRotation(S_006ada10* self, void* /*edx*/, float angle)
{
    float firstVector[3];
    float secondVector[3];

    firstVector[1] = 0.0f;
    secondVector[1] = 0.0f;

    float sinValue = std::sin(angle);
    firstVector[0] = std::cos(angle);
    firstVector[2] = -sinValue;
    secondVector[0] = sinValue;
    secondVector[2] = firstVector[0];

    return sub_6ADA10(self, nullptr,
        reinterpret_cast<uint32_t*>(firstVector),
        reinterpret_cast<uint32_t*>(&g_AxisY),
        reinterpret_cast<uint32_t*>(secondVector),
        reinterpret_cast<uint32_t*>(&g_ZeroVectorX));
}

HOOK(0x006AE2A0, GfxMatrix_SetYRotation, "auto");
