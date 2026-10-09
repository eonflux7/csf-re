#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, CalculateArcCosine, 0x0040C980, (float a1));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, EulerAnglesToDirection, 0x006AE640, (void* self, void* edx, float* a2));
CSF_FUNC(float*, __fastcall, NormalizeVector2, 0x006AEE00, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_RadiansToDegrees, 0x0083126C);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);

static double __fastcall GetCachedSignedAngleToSceneSelection(void* self, void* /*edx*/)
{
    if (!CSF_AT(uint8_t, self, 0xD))
    {
        const uint32_t index = CSF_AT(uint32_t, g_pCurrentScene, 0x70);
        if (index != UINT32_MAX &&
            index < CSF_AT(uint32_t, g_pCurrentScene, 0x68))
        {
            auto* selected =
                CSF_AT(C_008342e8**, g_pCurrentScene, 0x60)[index];
            if (selected)
            {
                const float* selectedPosition = reinterpret_cast<float*>(
                    GetObjectPositionVector(selected, nullptr));
                const float* ownerPosition = reinterpret_cast<float*>(
                    GetObjectPositionVector(CSF_AT(void*, self, 0x0), nullptr));

                float displacement[3];
                displacement[2] = ownerPosition[2] - selectedPosition[2];
                displacement[1] = ownerPosition[1] - selectedPosition[1];
                displacement[0] = ownerPosition[0] - selectedPosition[0];

                float angles[2] = {};
                float facing[3] = {};
                void* returnedAngles =
                    reinterpret_cast<void* (__fastcall*)(void*, void*, float*)>(
                        CSF_AT(void**, selected, 0x0)[0xAC / 4])(
                            selected, nullptr, angles);
                EulerAnglesToDirection(returnedAngles, nullptr, facing);

                float horizontalFacing[2] = {facing[0], facing[2]};
                NormalizeVector2(horizontalFacing, nullptr);

                float horizontalDisplacement[2] = {
                    displacement[0], displacement[2]
                };
                NormalizeVector2(horizontalDisplacement, nullptr);

                const float dot =
                    horizontalDisplacement[1] * horizontalFacing[1] +
                    horizontalDisplacement[0] * horizontalFacing[0];
                CSF_AT(float, self, 0x8) =
                    static_cast<float>(CalculateArcCosine(dot) * g_RadiansToDegrees);

                NormalizeVector3(facing, nullptr);
                NormalizeVector3(displacement, nullptr);

                if (displacement[0] * facing[2] -
                    displacement[2] * facing[0] < g_fZero)
                {
                    CSF_AT(float, self, 0x8) =
                        CSF_AT(float, self, 0x8) * g_MissingMetricValue;
                }
            }
        }
        CSF_AT(uint8_t, self, 0xD) = 1;
    }
    return CSF_AT(float, self, 0x8);
}

HOOK(0x00450AD0, GetCachedSignedAngleToSceneSelection, "auto");
