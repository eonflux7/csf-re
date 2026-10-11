#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0,
         (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430,
         (void* self, void* edx));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_WeaponPlacementTraceDepth, 0x0082E11C);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_AxisY_Y, 0x008A7D7C);
CSF_GLOBAL(float, g_AxisY_Z, 0x008A7D80);

static int __fastcall C_0083e7d8_ApplyInverseSquareVerticalForce(
    C_0083e7d8* self, void* /*edx*/)
{
    void* physicalObject = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_000c), 0x4C);

    void* dispatchedObject =
        C_0082dd78_DispatchReturnedObject(physicalObject, nullptr);
    void* dimensions =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, dispatchedObject, 0)[0x30 / 4])(
                dispatchedObject, nullptr);

    const float halfHeight =
        CSF_AT(float, dimensions, 0x04) * g_HalfDurationScale;
    float* position = GetObjectPositionVector(
        reinterpret_cast<void*>(self->field_000c), nullptr);

    float originX;
    std::memcpy(&originX, &self->field_002c, sizeof(originX));

    const float adjustedY = halfHeight + position[1];
    const float deltaX = position[0] - originX;
    const float deltaY = adjustedY - self->field_0030;
    const float deltaZ = position[2] - CSF_AT(float, self, 0x34);
    const float distance =
        std::sqrt(deltaZ * deltaZ + deltaY * deltaY + deltaX * deltaX);

    float axisX;
    float axisY;
    float axisZ;
    if (adjustedY < self->field_0030) {
        axisX = -g_AxisY;
        axisY = -g_AxisY_Y;
        axisZ = -g_AxisY_Z;
    } else {
        axisX = g_AxisY;
        axisY = g_AxisY_Y;
        axisZ = g_AxisY_Z;
    }

    const float strength =
        g_WeaponPlacementTraceDepth / (distance * distance);

    alignas(8) uint8_t forceBytes[0x1C];
    float* force = reinterpret_cast<float*>(forceBytes);
    force[0] = axisX * strength;
    force[1] = axisY * strength;
    force[2] = axisZ * strength;

    return reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*, float*)>(
        CSF_AT(void**, physicalObject, 0)[0x54 / 4])(
            physicalObject, nullptr, &self->field_002c, force);
}

HOOK(0x004E4540, C_0083e7d8_ApplyInverseSquareVerticalForce, "auto");
