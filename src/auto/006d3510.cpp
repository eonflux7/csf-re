#include <cstdint>
#include <cstddef>
#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_FUNC(double, __cdecl, sub_6D29B0, 0x006D29B0, (float a1));
CSF_FUNC(int, __cdecl, RwCameraSetViewWindow, 0x00629000, (int a1, uint32_t* a2));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_CharacterWidthScale, 0x0082E060);

// Stores the angle-derived scale in the camera, then sets the RW view window
// from tan(angle/2) horizontally and 0.75 of that vertically.
static int __fastcall CCamera_SetViewWindowFromFov(void* self, void* /*edx*/, float a1)
{
    double halfAngle = sub_6D29B0(a1);

    CSF_AT(float, self, 0x50) = (float)halfAngle;
    CSF_AT(float, self, 0x5C) = (float)halfAngle;

    double halfRadians = halfAngle * (double)g_HalfDurationScale;
    int rwCamera = CSF_AT(int, self, 0x0C);
    CSF_AT(int, self, 0x60) = 0;

    double tanHalf = std::sin(halfRadians) / std::cos(halfRadians);
    float viewWindow[2];
    viewWindow[0] = (float)tanHalf;
    viewWindow[1] = (float)(tanHalf * (double)g_CharacterWidthScale);

    return RwCameraSetViewWindow(rwCamera, reinterpret_cast<uint32_t*>(viewWindow));
}

HOOK(0x006D3510, CCamera_SetViewWindowFromFov, "auto");
