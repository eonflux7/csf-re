#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));
CSF_FUNC(double, __fastcall, S_00526d40_NormalizeCopyAndReportY, 0x006AB9E0, (S_00526d40* self, void*));
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (S_006adb40* self, void* , float** a2));

CSF_GLOBAL(float, g_LocalDirectionAngleScale, 0x0082D568);

static float __cdecl Game_ComputeLocalDirectionAngleDegrees(
    void* object, const float* targetPosition, int angleMode)
{
    struct MatrixHandle {
        float* matrix;
        std::uint8_t owned;
        std::uint8_t padding[3];
    };

    float position[3] = {};
    reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, object, 0)[0x38 / 4])(object, nullptr, position);

    float direction[3] = {
        targetPosition[0] - position[0],
        targetPosition[1] - position[1],
        targetPosition[2] - position[2]
    };

    MatrixHandle inverse;
    inverse.matrix = reinterpret_cast<float*>(RwMatrixCreate());
    inverse.owned = 1;

    MatrixHandle temporary = {};
    auto* source = reinterpret_cast<S_006adb40*(__fastcall*)(
        void*, void*, MatrixHandle*)>(
        CSF_AT(void**, object, 0)[0x3C / 4])(object, nullptr, &temporary);

    // The packet also classifies this callee address as a vtable reference.
    (void)CSF_VTBL(0x006ADB40);
    Matrix_InvertWithStoredTarget(source, nullptr, &inverse.matrix);

    if (temporary.owned) {
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(temporary.matrix));
    }

    float localDirection[3] = {};
    RwV3dTransformVectors(
        reinterpret_cast<int>(localDirection),
        reinterpret_cast<int>(direction),
        1,
        reinterpret_cast<int>(inverse.matrix));

    std::memcpy(direction, localDirection, sizeof(direction));

    double angle;
    if (angleMode == 0) {
        angle = S_00526d40_NormalizeCopyAndReportY(
            reinterpret_cast<S_00526d40*>(direction), nullptr);
    } else if (angleMode == 1) {
        angle = S_00526d40_GetVectorAngleRadians(
            reinterpret_cast<S_00526d40*>(direction), nullptr);
    } else {
        // The original reads the first argument's bits as a float here.
        float argumentBits;
        std::memcpy(&argumentBits, &object, sizeof(argumentBits));
        angle = argumentBits;
    }

    const float result = static_cast<float>(angle * g_LocalDirectionAngleScale);
    if (inverse.owned) {
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(inverse.matrix));
    }
    return result;
}

HOOK(0x00405720, Game_ComputeLocalDirectionAngleDegrees, "auto");
