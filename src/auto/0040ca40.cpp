#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_00526d40_TransformVector3, 0x004069F0, (S_00526d40* self, void* , uint32_t* out, int* in));
CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* lowerBound, float* upperBound));
CSF_FUNC(int, __fastcall, RotateSubEntryAroundAxisY, 0x00407FB0, (uint8_t* self, void* , int a2, float a3));
CSF_FUNC(int, __fastcall, RotateTransformEntryZ, 0x004081A0, (uint32_t* self, void* , int entryIndex, float angleRadians));
CSF_FUNC(int, __fastcall, GetTableEntryDwordByIndex, 0x004084C0, (void* self, void* , int index));
CSF_FUNC(int, __fastcall, IndexedChildForwardValue, 0x004084E0, (void* self, void* , int value, int index));
CSF_FUNC(C_0082daf4*, __fastcall, C_0082daf4_FindOrCreateByKey, 0x004087D0, (void* self, void* , int key));
CSF_FUNC(int, __fastcall, InitRatioVec4, 0x0040A5E0, (float* self, void* , float a, float b));
CSF_FUNC(float*, __fastcall, C_0082daf4_ApplyRatio, 0x0040B550, (C_0082daf4* self, void* , int a2, float* a3));
CSF_FUNC(float*, __fastcall, C_0082daf4_ApplyVec4, 0x0040B570, (C_0082daf4* self, void* , int a2, float* a3));
CSF_FUNC(double, __cdecl, GfxMatrix_ComputeArcCosine, 0x0040C980, (float a1));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (S_006adb40* self, void* , float** a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_VehiclePitchLowerLimit, 0x0082DBC4);
CSF_GLOBAL(float, g_VehiclePitchUpperLimit, 0x0082DBC8);
CSF_GLOBAL(float, g_VehicleAimLowerLimit, 0x0082DBCC);
CSF_GLOBAL(float, g_VehicleYawUpperLimit, 0x0082DBD0);

static bool __fastcall AiSearchOrders_AimControllerAtPosition(
    void* self, void* /*edx*/, float* position, char smooth)
{
    void* yawController = reinterpret_cast<void*>(
        GetTableEntryDwordByIndex(
            reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x8C)),
            nullptr, 1));
    void* pitchController = reinterpret_cast<void*>(
        GetTableEntryDwordByIndex(
            reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x8C)),
            nullptr, 0));
    if (!yawController || !pitchController)
        return false;

    CSF_AT(float, self, 0x94) = position[0];
    CSF_AT(float, self, 0x98) = position[1];
    CSF_AT(float, self, 0x9C) = position[2];

    // Keep the full storage shared by the original vector and matrix locals.
    // In particular, the ratio vector reuses the transformed-vector storage.
    alignas(8) uint8_t workspace[0x78];
    auto* origin = workspace + 0x10;
    auto* direction = reinterpret_cast<S_00526d40*>(workspace + 0x1C);
    auto* transformed = workspace + 0x04;
    auto* yawVector = workspace + 0x34;
    auto* pitchVector = workspace + 0x28;
    auto* yawMatrix = reinterpret_cast<S_006adb40*>(workspace + 0x40);
    auto* pitchMatrix = reinterpret_cast<S_006adb40*>(workspace + 0x48);
    auto* inverseYaw = reinterpret_cast<S_006adb40*>(workspace + 0x50);
    auto* inversePitch = reinterpret_cast<S_006adb40*>(workspace + 0x58);

    IndexedChildForwardValue(
        reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x8C)),
        nullptr, reinterpret_cast<int>(origin), 0);

    CSF_AT(float, &direction->field_0000, 0) =
        position[0] - CSF_AT(float, origin, 0);
    CSF_AT(float, direction, 0x4) =
        position[1] - CSF_AT(float, origin, 0x4);
    CSF_AT(float, direction, 0x8) =
        position[2] - CSF_AT(float, origin, 0x8);

    reinterpret_cast<void (__fastcall*)(void*, void*, S_006adb40*)>(
        CSF_AT(void**, yawController, 0)[0x3C / 4])(
            yawController, nullptr, yawMatrix);

    inverseYaw->field_0000 = reinterpret_cast<uint32_t>(RwMatrixCreate());
    inverseYaw->pad_0004[0] = 1;
    Matrix_InvertWithStoredTarget(
        yawMatrix, nullptr, reinterpret_cast<float**>(&inverseYaw->field_0000));
    S_00526d40_TransformVector3(
        direction, nullptr, reinterpret_cast<uint32_t*>(transformed),
        reinterpret_cast<int*>(inverseYaw));

    CSF_AT(float, yawVector, 0) = CSF_AT(float, transformed, 0);
    CSF_AT(float, yawVector, 0x4) = 0.0;
    CSF_AT(float, yawVector, 0x8) = CSF_AT(float, transformed, 0x8);
    NormalizeVector3(reinterpret_cast<C_00833b88*>(yawVector), nullptr);

    float yaw = static_cast<float>(
        GfxMatrix_ComputeArcCosine(CSF_AT(float, yawVector, 0x8)));
    if (CSF_AT(float, yawVector, 0) < g_fZero)
        yaw = -yaw;

    reinterpret_cast<void (__fastcall*)(void*, void*, S_006adb40*)>(
        CSF_AT(void**, pitchController, 0)[0x3C / 4])(
            pitchController, nullptr, pitchMatrix);

    inversePitch->field_0000 = reinterpret_cast<uint32_t>(RwMatrixCreate());
    inversePitch->pad_0004[0] = 1;
    Matrix_InvertWithStoredTarget(
        pitchMatrix, nullptr, reinterpret_cast<float**>(&inversePitch->field_0000));
    S_00526d40_TransformVector3(
        direction, nullptr, reinterpret_cast<uint32_t*>(transformed),
        reinterpret_cast<int*>(inversePitch));

    CSF_AT(float, pitchVector, 0) = 0.0;
    CSF_AT(float, pitchVector, 0x4) = CSF_AT(float, transformed, 0x4);
    CSF_AT(float, pitchVector, 0x8) = CSF_AT(float, transformed, 0x8);
    NormalizeVector3(reinterpret_cast<C_00833b88*>(pitchVector), nullptr);

    double pitchAngle =
        GfxMatrix_ComputeArcCosine(CSF_AT(float, pitchVector, 0x8));
    if (CSF_AT(float, pitchVector, 0x4) > g_fZero)
        pitchAngle = -pitchAngle;
    CSF_AT(float, workspace, 0x74) =
        static_cast<float>(pitchAngle + CSF_AT(float, self, 0xA4));
    CSF_AT(float, workspace, 0x70) = yaw;

    CSF_AT(float, workspace, 0x68) = -0.87266463f;
    CSF_AT(float, workspace, 0x6C) = 0.87266463f;
    CSF_AT(float, workspace, 0x60) = -0.08726646f;
    CSF_AT(float, workspace, 0x64) = 0.08726646f;

    const bool yawWithinLimits =
        !(yaw > g_VehicleYawUpperLimit) &&
        !(yaw < g_VehicleAimLowerLimit);
    const bool pitchWithinLimits =
        !(CSF_AT(float, workspace, 0x74) > g_VehiclePitchUpperLimit) &&
        !(CSF_AT(float, workspace, 0x74) < g_VehiclePitchLowerLimit);

    ClampFloatToBounds(
        &CSF_AT(float, workspace, 0x70),
        &CSF_AT(float, workspace, 0x68),
        &CSF_AT(float, workspace, 0x6C));
    ClampFloatToBounds(
        &CSF_AT(float, workspace, 0x74),
        &CSF_AT(float, workspace, 0x60),
        &CSF_AT(float, workspace, 0x64));

    if (smooth)
    {
        C_0082daf4* controller = C_0082daf4_FindOrCreateByKey(
            reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x8C)),
            nullptr, 0);
        float yawDelta =
            CSF_AT(float, workspace, 0x70) - CSF_AT(float, self, 0xA0);
        CSF_AT(float, workspace, 0) = 0.0;
        CSF_AT(float, workspace, 0x4) = 0.0;
        CSF_AT(float, workspace, 0x8) = 0.0;
        CSF_AT(float, workspace, 0xC) = -1.0;
        InitRatioVec4(
            reinterpret_cast<float*>(workspace), nullptr, yawDelta, 0.30000001f);
        C_0082daf4_ApplyVec4(
            controller, nullptr, 1, reinterpret_cast<float*>(workspace));

        float pitchDelta =
            CSF_AT(float, workspace, 0x74) - CSF_AT(float, self, 0xA4);
        InitRatioVec4(
            reinterpret_cast<float*>(workspace), nullptr, pitchDelta, 0.30000001f);
        C_0082daf4_ApplyRatio(
            controller, nullptr, 0, reinterpret_cast<float*>(workspace));
        CSF_AT(uint32_t, self, 0x90) = 1;
    }
    else
    {
        float yawDelta =
            CSF_AT(float, workspace, 0x70) - CSF_AT(float, self, 0xA0);
        RotateSubEntryAroundAxisY(
            reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x8C)),
            nullptr, 0, yawDelta);
        float pitchDelta =
            CSF_AT(float, workspace, 0x74) - CSF_AT(float, self, 0xA4);
        RotateTransformEntryZ(
            reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x8C)),
            nullptr, 0, pitchDelta);
        CSF_AT(uint32_t, self, 0x90) = 0;
    }

    CSF_AT(float, self, 0xA0) = CSF_AT(float, workspace, 0x70);
    CSF_AT(float, self, 0xA4) = CSF_AT(float, workspace, 0x74);
    const bool withinLimits = yawWithinLimits && pitchWithinLimits;

    if (inversePitch->pad_0004[0])
        RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(inversePitch->field_0000));
    if (pitchMatrix->pad_0004[0])
        RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(pitchMatrix->field_0000));
    if (inverseYaw->pad_0004[0])
        RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(inverseYaw->field_0000));
    if (yawMatrix->pad_0004[0])
        RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(yawMatrix->field_0000));
    return withinLimits;
}

HOOK(0x0040CA40, AiSearchOrders_AimControllerAtPosition, "auto");
