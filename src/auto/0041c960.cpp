#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Vehicle_RotateEntryZAndNotify, 0x004081A0, (void* self, void* edx, int a2, float a3));
CSF_FUNC(int, __fastcall, Vehicle_RotateTransformEntryZ, 0x00408200, (void* self, void* edx, int a2, float a3));
CSF_FUNC(int, __fastcall, Vehicle_GetTableEntryDwordByIndex, 0x004084C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Vehicle_IndexedChildForwardValue, 0x004084E0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(void*, __fastcall, Vehicle_FindOrCreateByKey, 0x004087D0, (void* self, void* edx, int a3));
CSF_FUNC(int, __fastcall, Vehicle_InitRatioVec4, 0x0040A5E0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(float*, __fastcall, Vehicle_SetRotationTransition, 0x0040B550, (void* self, void* edx, int a2, float* a3));
CSF_FUNC(double, __cdecl, CalculateArcCosine, 0x0040C980, (float a1));
CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (S_006adb40* self, void* , float** a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_VehiclePitchUpperLimit, 0x0082D7BC);
CSF_GLOBAL(float, g_VehicleAimLowerLimit, 0x0082DBCC);
CSF_GLOBAL(float, g_VehicleYawUpperLimit, 0x0082DBD0);

static bool __fastcall Vehicle_AimAtPosition(void* self, void* /*edx*/,
                                           float* position, char interpolate)
{
    CSF_AT(float, self, 0x90) = position[0];
    CSF_AT(float, self, 0x94) = position[1];
    CSF_AT(float, self, 0x98) = position[2];

    float origin[3];
    Vehicle_IndexedChildForwardValue(
        CSF_AT(void*, self, 0x30), nullptr,
        reinterpret_cast<int>(origin), 25);

    void* entry = reinterpret_cast<void*>(
        Vehicle_GetTableEntryDwordByIndex(CSF_AT(void*, self, 0x30), nullptr, 25));

    struct MatrixStorage {
        S_006adb40 matrix;
        std::uint8_t ownsInverse;
        std::uint8_t padding[3];
    } matrices{};

    ((void(__fastcall*)(void*, void*, S_006adb40*))
        CSF_AT(void**, entry, 0)[0x3C / 4])(entry, nullptr, &matrices.matrix);

    float displacement[3] = {
        position[0] - origin[0],
        position[1] - origin[1],
        position[2] - origin[2]
    };

    std::uint32_t* inverse = RwMatrixCreate();
    std::memcpy(&matrices.matrix.field_0008, &inverse, sizeof(inverse));
    matrices.ownsInverse = 1;

    // The packet also identifies this address as a vtable reference.
    (void)CSF_VTBL(0x006ADB40);
    Matrix_InvertWithStoredTarget(
        &matrices.matrix, nullptr,
        reinterpret_cast<float**>(&matrices.matrix.field_0008));

    float localDirection[3] = {0.0f, 0.0f, 0.0f};
    std::memcpy(&inverse, &matrices.matrix.field_0008, sizeof(inverse));
    RwV3dTransformVectors(
        reinterpret_cast<int>(localDirection),
        reinterpret_cast<int>(displacement), 1,
        reinterpret_cast<int>(inverse));

    float horizontal[3] = {localDirection[0], 0.0f, localDirection[2]};
    NormalizeVector3(horizontal, nullptr);
    float yawDelta = static_cast<float>(CalculateArcCosine(horizontal[2]));
    if (horizontal[0] < g_fZero)
        yawDelta = -yawDelta;

    float vertical[3] = {0.0f, localDirection[1], localDirection[2]};
    NormalizeVector3(vertical, nullptr);
    double pitchDelta = CalculateArcCosine(vertical[2]);
    if (vertical[1] > g_fZero)
        pitchDelta = -pitchDelta;

    float yaw = yawDelta + CSF_AT(float, self, 0x9C);
    float pitch = static_cast<float>(pitchDelta + CSF_AT(float, self, 0xA0));

    bool yawInRange = true;
    if (yaw > g_VehicleYawUpperLimit)
        yawInRange = false;
    else if (yaw < g_VehicleAimLowerLimit)
        yawInRange = false;

    bool pitchInRange = true;
    if (pitch > g_VehiclePitchUpperLimit)
        pitchInRange = false;
    else if (pitch < g_VehicleAimLowerLimit)
        pitchInRange = false;

    if (yaw < g_VehicleAimLowerLimit)
        yaw = -0.87266463f;
    else if (yaw > g_VehicleYawUpperLimit)
        yaw = 0.87266463f;

    if (pitch < g_VehicleAimLowerLimit)
        pitch = -0.87266463f;
    else if (pitch > g_VehiclePitchUpperLimit)
        pitch = 0.17453292f;

    if (interpolate)
    {
        void* transition = Vehicle_FindOrCreateByKey(
            CSF_AT(void*, self, 0x30), nullptr, 25);

        float yawChange = yaw - CSF_AT(float, self, 0x9C);
        float ratio[4] = {0.0f, 0.0f, 0.0f, -1.0f};
        Vehicle_InitRatioVec4(ratio, nullptr, yawChange, 0.30000001f);
        Vehicle_SetRotationTransition(transition, nullptr, 1, ratio);

        float pitchChange = pitch - CSF_AT(float, self, 0xA0);
        Vehicle_InitRatioVec4(ratio, nullptr, pitchChange, 0.30000001f);
        Vehicle_SetRotationTransition(transition, nullptr, 0, ratio);
        CSF_AT(int, self, 0x8C) = 1;
    }
    else
    {
        float yawChange = yaw - CSF_AT(float, self, 0x9C);
        Vehicle_RotateTransformEntryZ(
            CSF_AT(void*, self, 0x30), nullptr, 25, yawChange);

        float pitchChange = pitch - CSF_AT(float, self, 0xA0);
        Vehicle_RotateEntryZAndNotify(
            CSF_AT(void*, self, 0x30), nullptr, 25, pitchChange);
        CSF_AT(int, self, 0x8C) = 0;
    }

    CSF_AT(float, self, 0xA0) = pitch;
    CSF_AT(float, self, 0x9C) = yaw;
    bool inRange = yawInRange && pitchInRange;

    if (matrices.ownsInverse)
    {
        std::memcpy(&inverse, &matrices.matrix.field_0008, sizeof(inverse));
        RwMatrixDestroy(inverse);
    }
    if (matrices.matrix.pad_0004[0])
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrices.matrix.field_0000));

    return inRange;
}

HOOK(0x0041C960, Vehicle_AimAtPosition, "auto");
