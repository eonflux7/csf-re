#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static float* __fastcall GfxBboxMath_SetBoundsCenter(
    float* self, void* /*edx*/, float* center)
{
    const double extentX =
        static_cast<double>(CSF_AT(float, self, 0x00)) -
        CSF_AT(float, self, 0x0C);
    const double extentY =
        static_cast<double>(CSF_AT(float, self, 0x04)) -
        CSF_AT(float, self, 0x10);
    const float extentZ =
        CSF_AT(float, self, 0x08) - CSF_AT(float, self, 0x14);

    const float halfX = static_cast<float>(extentX * g_HalfDurationScale);
    const double halfY = extentY * g_HalfDurationScale;
    const double halfZ = static_cast<double>(extentZ) * g_HalfDurationScale;

    const float upperX = halfX + CSF_AT(float, center, 0x00);
    const float upperY =
        static_cast<float>(halfY + CSF_AT(float, center, 0x04));
    const float upperZ =
        static_cast<float>(halfZ + CSF_AT(float, center, 0x08));

    CSF_AT(float, self, 0x00) = upperX;
    CSF_AT(float, self, 0x04) = upperY;
    CSF_AT(float, self, 0x08) = upperZ;

    const float lowerX = CSF_AT(float, center, 0x00) - halfX;
    const float lowerY =
        static_cast<float>(CSF_AT(float, center, 0x04) - halfY);
    const float lowerZ =
        static_cast<float>(CSF_AT(float, center, 0x08) - halfZ);

    CSF_AT(float, self, 0x0C) = lowerX;
    CSF_AT(float, self, 0x10) = lowerY;
    CSF_AT(float, self, 0x14) = lowerZ;

    return self;
}

HOOK(0x006AB5C0, GfxBboxMath_SetBoundsCenter, "auto");
