#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwCameraSetViewWindow, 0x00629000, (int a1, uint32_t* a2));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_CharacterWidthScale, 0x0082E060);

static void __fastcall GfxCamera_UpdateFieldOfView(void* self, void* /*edx*/, float deltaTime)
{
    if (CSF_AT(float, self, 0x50) != CSF_AT(float, self, 0x5C))
    {
        if (CSF_AT(float, self, 0x50) < CSF_AT(float, self, 0x5C))
        {
            const float nextAngle =
                deltaTime * CSF_AT(float, self, 0x60) + CSF_AT(float, self, 0x50);
            CSF_AT(float, self, 0x50) =
                nextAngle < CSF_AT(float, self, 0x5C)
                    ? nextAngle
                    : CSF_AT(float, self, 0x5C);
        }
        else if (CSF_AT(float, self, 0x50) > CSF_AT(float, self, 0x5C))
        {
            const float nextAngle =
                CSF_AT(float, self, 0x50) - deltaTime * CSF_AT(float, self, 0x60);
            CSF_AT(float, self, 0x50) =
                nextAngle < CSF_AT(float, self, 0x5C)
                    ? CSF_AT(float, self, 0x5C)
                    : nextAngle;
        }

        const float halfAngle = CSF_AT(float, self, 0x50) * g_HalfDurationScale;
        const int camera = CSF_AT(int, self, 0x0C);
        const float sine = std::sin(halfAngle);
        const float cosine = std::cos(halfAngle);
        const float horizontalExtent = sine / cosine;

        alignas(8) uint8_t viewWindowBytes[0x8];
        float* viewWindow = reinterpret_cast<float*>(viewWindowBytes);
        viewWindow[0] = horizontalExtent;
        viewWindow[1] = horizontalExtent * g_CharacterWidthScale;
        RwCameraSetViewWindow(camera, reinterpret_cast<uint32_t*>(viewWindowBytes));
    }
}

HOOK(0x006D2F60, GfxCamera_UpdateFieldOfView, "auto");
