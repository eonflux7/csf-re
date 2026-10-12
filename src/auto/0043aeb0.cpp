#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_RadiansToDegreesScale, 0x00830D18);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, GetContextTransform, 0x00495890, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GfxMatrix_GetEulerAngles, 0x006AE3A0, (float** self, void* , float* angles));
CSF_FUNC(double, __cdecl, MenuMpCharacterSelect_NormalizeAngle, 0x00406950, (float angleRadians));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(double, __cdecl, MinimapRadar_SmoothHeading, 0x0043AD70, (float a1, float a2, float* a3));

// Recomputes the minimap radar heading from the current scene's camera transform,
// smooths it against the stored heading (field_0048), and stores the result back.
static void __fastcall C_00830d90_UpdateRadarHeading(C_00830d90* self, void* /*edx*/)
{
    uint32_t sceneMenuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    uint32_t contextTransform = GetContextTransform(reinterpret_cast<void*>(sceneMenuContext), nullptr);

    // Virtual slot 0x8C / 4 = 35 on the transform object: fills a {matrix handle, flag} pair
    // and returns the matrix object used for the Euler-angle query.
    alignas(8) uint8_t matrixBytes[8];
    alignas(8) uint8_t angleBytes[12];
    void* transformObject = reinterpret_cast<void*>(contextTransform);
    void* matrixObject = reinterpret_cast<void* (__fastcall*)(void*, void*, uint8_t*)>(
        CSF_AT(void**, transformObject, 0)[0x8C / 4])(transformObject, nullptr, matrixBytes);

    float* eulerAngles = GfxMatrix_GetEulerAngles(reinterpret_cast<float**>(matrixObject), nullptr, reinterpret_cast<float*>(angleBytes));

    double normalizedAngle = MenuMpCharacterSelect_NormalizeAngle(eulerAngles[1]);
    float headingDegrees = static_cast<float>(normalizedAngle * g_RadiansToDegreesScale);

    uint8_t matrixFlag = matrixBytes[4];
    if (matrixFlag != 0)
    {
        uint32_t matrixHandle = *reinterpret_cast<uint32_t*>(matrixBytes);
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrixHandle));
    }

    self->field_0048 = static_cast<float>(
        MinimapRadar_SmoothHeading(self->field_0048, headingDegrees, &headingDegrees));
}

HOOK(0x0043AEB0, C_00830d90_UpdateRadarHeading, "auto");
