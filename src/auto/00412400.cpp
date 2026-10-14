#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ValueScale, 0x0082D4EC);
CSF_GLOBAL(float, g_ScaleY_0_6, 0x0082E038);

static int __fastcall C_0082dfd0_TryTransferNearbyObject(
    C_0082dfd0* self, void* /*edx*/, void* candidate,
    void* previousObject, char force)
{
    void* boundsObject = reinterpret_cast<void*>(
        C_0082dd78_DispatchReturnedObject(
            CSF_AT(C_0082dd78*, candidate, 0x4C), nullptr));

    float* dimensions =
        reinterpret_cast<float* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, boundsObject, 0)[0x30 / 4])(
                boundsObject, nullptr);

    const float width = dimensions[0];
    const float scaledDepth = dimensions[2] * g_ValueScale;
    const float scaledWidth = width * g_ValueScale;
    const float radius =
        (scaledWidth < scaledDepth ? scaledDepth : scaledWidth) * g_ScaleY_0_6;

    const float* candidatePosition = reinterpret_cast<const float*>(
        GetObjectPositionVector(candidate, nullptr));
    const float candidateX = candidatePosition[0];
    const float candidateZ = candidatePosition[2];

    const float* ownerPosition = reinterpret_cast<const float*>(
        GetObjectPositionVector(
            reinterpret_cast<void*>(self->field_0004), nullptr));
    const float ownerX = ownerPosition[0];
    const float ownerZ = ownerPosition[2];

    if (!force)
    {
        const double dx = static_cast<double>(candidateX) - ownerX;
        const double dz = static_cast<double>(candidateZ) - ownerZ;
        const double distanceSquared =
            dz * dz + static_cast<double>(g_fZero) * g_fZero + dx * dx;
        if (!(radius * radius > distanceSquared))
            return 0;
    }

    if ((self->field_0148 & 0x80000000) == 0)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, previousObject, 0)[0x6C / 4])(
                previousObject, nullptr, 1);
    }

    self->field_0144 = CSF_AT(uint32_t, candidate, 0x20);
    self->field_0148 = CSF_AT(uint32_t, candidate, 0x24);

    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, candidate, 0)[0x6C / 4])(
            candidate, nullptr, 0);

    return reinterpret_cast<int>(candidate);
}

HOOK(0x00412400, C_0082dfd0_TryTransferNearbyObject, "auto");
