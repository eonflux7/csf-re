#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxMatrix_InvertInPlace, 0x006ADB10, (void** self, void*));
CSF_FUNC(int, __cdecl, CharActionFsm_GetProviderFrameObject, 0x006C0500, (int a1));

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static char __fastcall CharActionFsm_SetControllerPosition(
    void* self, void* /*edx*/, float* position)
{
    void* controller = CSF_AT(void*, self, 0x8);
    void* object =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0x28 / 4])(controller, nullptr);
    if (!object)
        return 0;

    float facing[3];
    facing[0] = CSF_AT(float, self, 0x34) - CSF_AT(float, self, 0x28);
    facing[1] = 0.0f;
    facing[2] = CSF_AT(float, self, 0x3C) - CSF_AT(float, self, 0x30);

    float displacement[3];
    displacement[0] = position[0] - CSF_AT(float, self, 0x28);
    displacement[1] = position[1] - CSF_AT(float, self, 0x2C);
    displacement[2] = position[2] - CSF_AT(float, self, 0x30);

    NormalizeVector3(facing, nullptr);
    displacement[1] = 0.0f;
    NormalizeVector3(displacement, nullptr);

    const float cross = displacement[0] * facing[2]
                      - displacement[2] * facing[0];
    CSF_AT(float, self, 0x20) =
        cross < g_fZero ? g_MissingMetricValue : g_TransitionOne;

    void* provider = GetCachedObjectProvider(object, nullptr);
    void* frameObject = reinterpret_cast<void*>(
        CharActionFsm_GetProviderFrameObject(reinterpret_cast<int>(provider)));

    struct MatrixReference
    {
        void* matrix;
        std::uint8_t owned;
        std::uint8_t padding[3];
    };

    MatrixReference matrixReference;
    matrixReference.matrix =
        RwFrameGetLTM(CSF_AT(std::uint32_t, frameObject, 0x4));
    matrixReference.owned = 0;

    GfxMatrix_InvertInPlace(&matrixReference.matrix, nullptr);

    float transformed[3];
    RwV3dTransformPoints(
        reinterpret_cast<int>(transformed),
        reinterpret_cast<int>(facing),
        1,
        reinterpret_cast<int>(matrixReference.matrix));

    CSF_AT(float, self, 0x58) =
        transformed[2] * CSF_AT(float, self, 0x20);
    CSF_AT(float, self, 0x5C) = 0.0f;
    CSF_AT(float, self, 0x60) = transformed[0];

    GfxMatrix_InvertInPlace(&matrixReference.matrix, nullptr);
    RwV3dTransformPoints(
        reinterpret_cast<int>(transformed),
        reinterpret_cast<int>(static_cast<std::uint8_t*>(self) + 0x58),
        1,
        reinterpret_cast<int>(matrixReference.matrix));

    CSF_AT(float, self, 0x58) = transformed[0];
    CSF_AT(float, self, 0x5C) = transformed[1];
    CSF_AT(float, self, 0x60) = transformed[2];

    if (matrixReference.owned)
        return static_cast<char>(RwMatrixDestroy(
            static_cast<std::uint32_t*>(matrixReference.matrix)));
    return static_cast<char>(matrixReference.owned);
}

HOOK(0x004096D0, CharActionFsm_SetControllerPosition, "auto");
