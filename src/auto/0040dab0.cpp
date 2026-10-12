#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, RotateTransformEntryZ, 0x004081A0, (uint32_t* self, void* , int entryIndex, float angleRadians));
CSF_FUNC(int, __fastcall, RotateEntryZAndNotify, 0x00408200, (uint32_t* self, void* , int entryIndex, float angleRadians));
CSF_FUNC(int, __fastcall, GetTableEntryDwordByIndex, 0x004084C0, (void* self, void* , int index));
CSF_FUNC(int, __fastcall, IndexedChildForwardValue, 0x004084E0, (void* self, void* , int value, int index));
CSF_FUNC(C_0082daf4*, __fastcall, C_0082daf4_FindOrCreateByKey, 0x004087D0, (void* self, void* , int key));
CSF_FUNC(int, __fastcall, InitRatioVec4, 0x0040A5E0, (float* self, void* , float a, float b));
CSF_FUNC(float*, __fastcall, Wheel_ApplyRatio, 0x0040B550, (void* self, void* edx, int a2, float* a3));
CSF_FUNC(double, __cdecl, CalculateArcCosine, 0x0040C980, (float a1));
CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (S_006adb40* self, void* , float** a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_DefaultAngleLimit, 0x0082D7BC);
CSF_GLOBAL(float, g_VehicleAimLowerLimit, 0x0082DBCC);
CSF_GLOBAL(float, g_VehicleYawUpperLimit, 0x0082DBD0);

static bool __fastcall Vehicle_AimPrimaryEntryAtPosition(
    void* self, void* /*edx*/, float* position, char interpolate)
{
    CSF_AT(float, self, 0x54) = position[0];
    CSF_AT(float, self, 0x58) = position[1];
    CSF_AT(float, self, 0x5C) = position[2];

    float origin[3];
    IndexedChildForwardValue(CSF_AT(void*, self, 0x30), nullptr,
                             reinterpret_cast<int>(origin), 1);

    void* entry = reinterpret_cast<void*>(
        GetTableEntryDwordByIndex(CSF_AT(void*, self, 0x30), nullptr, 1));

    alignas(8) std::uint8_t matrixBytes[0x18];
    S_006adb40& matrix = *reinterpret_cast<S_006adb40*>(matrixBytes);
    ((void(__fastcall*)(void*, void*, S_006adb40*))
        CSF_AT(void**, entry, 0)[0x3C / 4])(entry, nullptr, &matrix);

    float displacement[3] = {
        position[0] - origin[0],
        position[1] - origin[1],
        position[2] - origin[2]
    };

    std::uint32_t* inverse = RwMatrixCreate();
    std::memcpy(&matrix.field_0008, &inverse, sizeof(inverse));
    CSF_AT(std::uint8_t, matrixBytes, 0x0C) = 1;

    (void)CSF_VTBL(0x006ADB40);
    Matrix_InvertWithStoredTarget(
        &matrix, nullptr, reinterpret_cast<float**>(&matrix.field_0008));

    float localDirection[3] = {0.0f, 0.0f, 0.0f};
    std::memcpy(&inverse, &matrix.field_0008, sizeof(inverse));
    RwV3dTransformVectors(
        reinterpret_cast<int>(localDirection),
        reinterpret_cast<int>(displacement), 1,
        reinterpret_cast<int>(inverse));

    float savedY = localDirection[1];
    float savedZ = localDirection[2];
    float horizontal[3] = {localDirection[0], 0.0f, localDirection[2]};
    NormalizeVector3(reinterpret_cast<C_00833b88*>(horizontal), nullptr);
    float yawDelta = static_cast<float>(CalculateArcCosine(horizontal[2]));
    if (horizontal[0] < g_fZero)
        yawDelta = -yawDelta;

    float vertical[3] = {0.0f, savedY, savedZ};
    NormalizeVector3(reinterpret_cast<C_00833b88*>(vertical), nullptr);
    double pitchDelta = CalculateArcCosine(vertical[2]);
    if (vertical[1] > g_fZero)
        pitchDelta = -pitchDelta;

    float yaw = yawDelta + CSF_AT(float, self, 0x60);
    float pitch = static_cast<float>(pitchDelta + CSF_AT(float, self, 0x64));

    bool yawInRange = true;
    if (yaw > g_VehicleYawUpperLimit)
        yawInRange = false;
    else if (yaw < g_VehicleAimLowerLimit)
        yawInRange = false;

    bool pitchInRange = true;
    if (pitch > g_DefaultAngleLimit)
        pitchInRange = false;
    else if (pitch < g_VehicleAimLowerLimit)
        pitchInRange = false;

    if (yaw < g_VehicleAimLowerLimit)
        yaw = -0.87266463f;
    else if (yaw > g_VehicleYawUpperLimit)
        yaw = 0.87266463f;

    if (pitch < g_VehicleAimLowerLimit)
        pitch = -0.87266463f;
    else if (pitch > g_DefaultAngleLimit)
        pitch = 0.17453292f;

    if (interpolate)
    {
        C_0082daf4* transition = C_0082daf4_FindOrCreateByKey(
            CSF_AT(void*, self, 0x30), nullptr, 1);

        float yawChange = yaw - CSF_AT(float, self, 0x60);
        float ratio[4] = {0.0f, 0.0f, 0.0f, -1.0f};
        InitRatioVec4(ratio, nullptr, yawChange, 0.30000001f);
        Wheel_ApplyRatio(transition, nullptr, 1, ratio);

        float pitchChange = pitch - CSF_AT(float, self, 0x64);
        InitRatioVec4(ratio, nullptr, pitchChange, 0.30000001f);
        Wheel_ApplyRatio(transition, nullptr, 0, ratio);
        CSF_AT(int, self, 0x50) = 1;
    }
    else
    {
        float yawChange = yaw - CSF_AT(float, self, 0x60);
        RotateEntryZAndNotify(
            CSF_AT(uint32_t*, self, 0x30), nullptr, 1, yawChange);

        float pitchChange = pitch - CSF_AT(float, self, 0x64);
        RotateTransformEntryZ(
            CSF_AT(uint32_t*, self, 0x30), nullptr, 1, pitchChange);
        CSF_AT(int, self, 0x50) = 0;
    }

    CSF_AT(float, self, 0x64) = pitch;
    CSF_AT(float, self, 0x60) = yaw;
    bool inRange = yawInRange && pitchInRange;

    if (CSF_AT(std::uint8_t, matrixBytes, 0x0C))
    {
        std::memcpy(&inverse, &matrix.field_0008, sizeof(inverse));
        RwMatrixDestroy(inverse);
    }
    if (matrix.pad_0004[0])
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrix.field_0000));

    return inRange;
}

HOOK(0x0040DAB0, Vehicle_AimPrimaryEntryAtPosition, "auto");
