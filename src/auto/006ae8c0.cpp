#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, Vector_ConvertDirection, 0x006ABA10, (void* self, void* edx, uint32_t* a2));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static float* __fastcall GfxMatrix_SetQuaternionFromAxisAngle(
    float* self, void* /*edx*/, S_00526d40* axis, float angle)
{
    if (reinterpret_cast<const float&>(axis[0].field_0000) == g_fZero &&
        reinterpret_cast<const float&>(axis[1].field_0000) == g_fZero &&
        reinterpret_cast<const float&>(axis[2].field_0000) == g_fZero)
    {
        self[0] = 0.0f;
        self[1] = 0.0f;
        self[2] = 0.0f;
        self[3] = 1.0f;
        return self;
    }

    const float halfAngle = angle * g_HalfDurationScale;
    const float sine = std::sin(halfAngle);
    uint32_t directionStorage[3];
    const float* direction = reinterpret_cast<const float*>(
        Vector_ConvertDirection(axis, nullptr, directionStorage));

    const float x = sine * direction[0];
    const float y = sine * direction[1];
    const float z = sine * direction[2];

    self[2] = z;
    self[0] = x;
    self[1] = y;
    self[3] = std::cos(halfAngle);
    return self;
}

HOOK(0x006AE8C0, GfxMatrix_SetQuaternionFromAxisAngle, "auto");
