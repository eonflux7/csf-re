#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Vehicle_GetIndexedChild, 0x004084C0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Vehicle_GetIndexedChildPosition, 0x004084E0,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(void*, __fastcall, Vehicle_FindOrCreateChildByKey, 0x004087D0,
         (void* self, void* edx, int a3));
CSF_FUNC(float*, __fastcall, Vehicle_SetChildParameter, 0x0040B550,
         (void* self, void* edx, int a2, float* a3));
CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0,
         (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10,
         (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930,
         (S_00526d40* self, void*));
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40,
         (S_006adb40* self, void*, float** a2));

static const auto g_MatrixInvertVtableReference = CSF_VTBL(0x006ADB40);

static std::uint32_t __fastcall Vehicle_SetWheelSteeringTowardPoint(
    void* self, void* /*edx*/, float* target)
{
    float childPosition[3] = {};
    Vehicle_GetIndexedChildPosition(
        CSF_AT(void*, self, 0x30), nullptr,
        reinterpret_cast<int>(childPosition), 28);

    void* child = reinterpret_cast<void*>(
        Vehicle_GetIndexedChild(CSF_AT(void*, self, 0x30), nullptr, 28));

    alignas(4) std::uint8_t matrixStorage[0x10];
    auto* matrix = reinterpret_cast<S_006adb40*>(matrixStorage);
    reinterpret_cast<void(__fastcall*)(void*, void*, S_006adb40*)>(
        CSF_AT(void**, child, 0)[0x3C / 4])(child, nullptr, matrix);

    float direction[3];
    direction[0] = target[0] - childPosition[0];
    direction[1] = target[1] - childPosition[1];
    direction[2] = target[2] - childPosition[2];

    std::uint32_t* inverse = RwMatrixCreate();
    std::memcpy(&matrix->field_0008, &inverse, sizeof(inverse));
    CSF_AT(std::uint8_t, matrixStorage, 0xC) = 1;
    Matrix_InvertWithStoredTarget(
        matrix, nullptr, reinterpret_cast<float**>(&matrix->field_0008));

    float localDirection[3] = {0.0f, 0.0f, 0.0f};
    std::memcpy(&inverse, &matrix->field_0008, sizeof(inverse));
    RwV3dTransformVectors(
        reinterpret_cast<int>(localDirection),
        reinterpret_cast<int>(direction), 1,
        reinterpret_cast<int>(inverse));

    alignas(4) std::uint8_t angleStorage[0xC];
    std::memcpy(angleStorage, localDirection, sizeof(localDirection));
    float angle = static_cast<float>(S_00526d40_GetVectorAngleRadians(
        reinterpret_cast<S_00526d40*>(angleStorage), nullptr));

    void* steeringChild = Vehicle_FindOrCreateChildByKey(
        CSF_AT(void*, self, 0x30), nullptr, 28);
    float parameters[4] = {angle, 1.0f, 0.0f, -1.0f};
    Vehicle_SetChildParameter(steeringChild, nullptr, 1, parameters);

    const bool ownsInverse = CSF_AT(std::uint8_t, matrixStorage, 0xC) != 0;
    CSF_AT(std::uint32_t, self, 0x90) = 1;
    if (ownsInverse) {
        std::memcpy(&inverse, &matrix->field_0008, sizeof(inverse));
        RwMatrixDestroy(inverse);
    }

    std::uint8_t result = CSF_AT(std::uint8_t, matrixStorage, 0x4);
    if (result) {
        std::uint32_t* sourceMatrix;
        std::memcpy(&sourceMatrix, &matrix->field_0000, sizeof(sourceMatrix));
        return static_cast<std::uint32_t>(RwMatrixDestroy(sourceMatrix));
    }
    return result;
}

HOOK(0x0041D040, Vehicle_SetWheelSteeringTowardPoint, "auto");
