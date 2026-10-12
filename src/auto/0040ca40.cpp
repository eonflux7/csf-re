#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_00526d40_TransformVector3, 0x004069F0, (S_00526d40* self, void* , uint32_t* out, int* in));
CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* lowerBound, float* upperBound));
CSF_FUNC(int, __fastcall, RotateSubEntryAroundAxisY, 0x00407FB0, (uint8_t* self, void* , int a2, float a3));
CSF_FUNC(int, __fastcall, RotateTransformEntryZ, 0x004081A0, (uint32_t* self, void* , int entryIndex, float angleRadians));
CSF_FUNC(int, __fastcall, GetTableEntryDwordByIndex, 0x004084C0, (void* self, void* , int index));
CSF_FUNC(int, __fastcall, IndexedChildForwardValue, 0x004084E0, (void* self, void* , int value, int index));
CSF_FUNC(C_0082daf4*, __fastcall, C_0082daf4_FindOrCreateByKey, 0x004087D0, (void* self, void* , int key));
CSF_FUNC(int, __fastcall, InitRatioVec4, 0x0040A5E0, (float* self, void* , float a, float b));
CSF_FUNC(float*, __fastcall, Wheel_ApplyRatio, 0x0040B550, (void* self, void* edx, int a2, float* a3));
CSF_FUNC(float*, __fastcall, VehicleWheel_ApplyVec4, 0x0040B570, (void* self, void* edx, int a2, float* a3));
CSF_FUNC(double, __cdecl, CalculateArcCosine, 0x0040C980, (float a1));
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
    void* yawEntry = reinterpret_cast<void*>(GetTableEntryDwordByIndex(
        CSF_AT(void*, self, 0x8C), nullptr, 1));
    void* pitchEntry = reinterpret_cast<void*>(GetTableEntryDwordByIndex(
        CSF_AT(void*, self, 0x8C), nullptr, 0));

    if (!yawEntry || !pitchEntry)
        return false;

    CSF_AT(float, self, 0x94) = position[0];
    CSF_AT(float, self, 0x98) = position[1];
    CSF_AT(float, self, 0x9C) = position[2];

    // The first four floats also serve as the ratio vector later.
    alignas(8) uint8_t ratioBytes[0x10];
    float* ratio = reinterpret_cast<float*>(ratioBytes);
    float* transformed = ratio + 1;

    alignas(8) uint8_t originBytes[0xC];
    float* origin = reinterpret_cast<float*>(originBytes);
    IndexedChildForwardValue(
        CSF_AT(void*, self, 0x8C), nullptr,
        reinterpret_cast<int>(origin), 0);

    alignas(8) uint8_t displacementBytes[0xC];
    float* displacement = reinterpret_cast<float*>(displacementBytes);
    displacement[0] = position[0] - origin[0];
    displacement[1] = position[1] - origin[1];
    displacement[2] = position[2] - origin[2];

    alignas(8) uint8_t yawMatrixBytes[0x8];
    alignas(8) uint8_t pitchMatrixBytes[0x8];
    alignas(8) uint8_t inverseYawBytes[0x8];
    alignas(8) uint8_t inversePitchBytes[0x8];
    S_006adb40& yawMatrix =
        *reinterpret_cast<S_006adb40*>(yawMatrixBytes);
    S_006adb40& pitchMatrix =
        *reinterpret_cast<S_006adb40*>(pitchMatrixBytes);

    ((void(__fastcall*)(void*, void*, S_006adb40*))
        CSF_AT(void**, yawEntry, 0)[0x3C / 4])(
            yawEntry, nullptr, &yawMatrix);

    CSF_AT(float*, inverseYawBytes, 0) =
        reinterpret_cast<float*>(RwMatrixCreate());
    CSF_AT(uint8_t, inverseYawBytes, 4) = 1;
    (void)CSF_VTBL(0x006ADB40);
    Matrix_InvertWithStoredTarget(
        &yawMatrix, nullptr,
        reinterpret_cast<float**>(inverseYawBytes));

    S_00526d40_TransformVector3(
        reinterpret_cast<S_00526d40*>(displacementBytes), nullptr,
        reinterpret_cast<uint32_t*>(transformed),
        reinterpret_cast<int*>(inverseYawBytes));

    alignas(8) uint8_t yawDirectionBytes[0xC];
    float* yawDirection = reinterpret_cast<float*>(yawDirectionBytes);
    yawDirection[0] = transformed[0];
    yawDirection[1] = 0.0f;
    yawDirection[2] = transformed[2];
    NormalizeVector3(
        reinterpret_cast<C_00833b88*>(yawDirectionBytes), nullptr);

    float yaw = static_cast<float>(CalculateArcCosine(yawDirection[2]));
    if (yawDirection[0] < g_fZero)
        yaw = -yaw;

    ((void(__fastcall*)(void*, void*, S_006adb40*))
        CSF_AT(void**, pitchEntry, 0)[0x3C / 4])(
            pitchEntry, nullptr, &pitchMatrix);

    CSF_AT(float*, inversePitchBytes, 0) =
        reinterpret_cast<float*>(RwMatrixCreate());
    CSF_AT(uint8_t, inversePitchBytes, 4) = 1;
    (void)CSF_VTBL(0x006ADB40);
    Matrix_InvertWithStoredTarget(
        &pitchMatrix, nullptr,
        reinterpret_cast<float**>(inversePitchBytes));

    S_00526d40_TransformVector3(
        reinterpret_cast<S_00526d40*>(displacementBytes), nullptr,
        reinterpret_cast<uint32_t*>(transformed),
        reinterpret_cast<int*>(inversePitchBytes));

    alignas(8) uint8_t pitchDirectionBytes[0xC];
    float* pitchDirection = reinterpret_cast<float*>(pitchDirectionBytes);
    pitchDirection[0] = 0.0f;
    pitchDirection[1] = transformed[1];
    pitchDirection[2] = transformed[2];
    NormalizeVector3(
        reinterpret_cast<C_00833b88*>(pitchDirectionBytes), nullptr);

    double pitchAngle = CalculateArcCosine(pitchDirection[2]);
    if (pitchDirection[1] > g_fZero)
        pitchAngle = -pitchAngle;
    float pitch =
        static_cast<float>(pitchAngle + CSF_AT(float, self, 0xA4));

    float yawLower = -0.87266463f;
    float yawUpper = 0.87266463f;
    float pitchLower = -0.08726646f;
    float pitchUpper = 0.08726646f;

    bool yawInRange = !(yaw > g_VehicleYawUpperLimit) &&
                      !(yaw < g_VehicleAimLowerLimit);
    bool pitchInRange = !(pitch > g_VehiclePitchUpperLimit) &&
                        !(pitch < g_VehiclePitchLowerLimit);

    ClampFloatToBounds(&yaw, &yawLower, &yawUpper);
    ClampFloatToBounds(&pitch, &pitchLower, &pitchUpper);

    if (smooth)
    {
        C_0082daf4* wheel = C_0082daf4_FindOrCreateByKey(
            CSF_AT(void*, self, 0x8C), nullptr, 0);
        float yawDelta = yaw - CSF_AT(float, self, 0xA0);
        ratio[0] = 0.0f;
        ratio[1] = 0.0f;
        ratio[2] = 0.0f;
        ratio[3] = -1.0f;
        InitRatioVec4(ratio, nullptr, yawDelta, 0.30000001f);
        VehicleWheel_ApplyVec4(wheel, nullptr, 1, ratio);

        float pitchDelta = pitch - CSF_AT(float, self, 0xA4);
        InitRatioVec4(ratio, nullptr, pitchDelta, 0.30000001f);
        Wheel_ApplyRatio(wheel, nullptr, 0, ratio);
        CSF_AT(int, self, 0x90) = 1;
    }
    else
    {
        float yawDelta = yaw - CSF_AT(float, self, 0xA0);
        RotateSubEntryAroundAxisY(
            CSF_AT(uint8_t*, self, 0x8C), nullptr, 0, yawDelta);

        float pitchDelta = pitch - CSF_AT(float, self, 0xA4);
        RotateTransformEntryZ(
            CSF_AT(uint32_t*, self, 0x8C), nullptr, 0, pitchDelta);
        CSF_AT(int, self, 0x90) = 0;
    }

    CSF_AT(float, self, 0xA0) = yaw;
    CSF_AT(float, self, 0xA4) = pitch;
    bool inRange = yawInRange && pitchInRange;

    if (CSF_AT(uint8_t, inversePitchBytes, 4))
        RwMatrixDestroy(CSF_AT(uint32_t*, inversePitchBytes, 0));
    if (pitchMatrix.pad_0004[0])
        RwMatrixDestroy(
            reinterpret_cast<uint32_t*>(pitchMatrix.field_0000));
    if (CSF_AT(uint8_t, inverseYawBytes, 4))
        RwMatrixDestroy(CSF_AT(uint32_t*, inverseYawBytes, 0));
    if (yawMatrix.pad_0004[0])
        RwMatrixDestroy(
            reinterpret_cast<uint32_t*>(yawMatrix.field_0000));

    return inRange;
}

HOOK(0x0040CA40, AiSearchOrders_AimControllerAtPosition, "auto");
