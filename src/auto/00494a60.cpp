#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, ComputeAlertIconAnchorPosition, 0x004A1520, (float* self, void* , float* out));
CSF_FUNC(void, __fastcall, Rotation_ApplyYawDelta, 0x006AE6B0, (S_006ae570* self, void* , float yawDelta));

CSF_GLOBAL(float, g_MinimumRotation, 0x008305EC);
CSF_GLOBAL(float, g_MaximumRotation, 0x008305F0);

static float* __fastcall CPlayerCharacter_GetAlertIconRotation(
    CPlayerCharacter* self, void* /*edx*/, float* out)
{
    alignas(8) uint8_t rotationBytes[0x8];
    S_006ae570& rotation =
        *reinterpret_cast<S_006ae570*>(rotationBytes);

    rotation.field_0004 = CSF_AT(float, self, 0x1B0);
    rotation.field_0000 = CSF_AT(float, self, 0x1AC);

    void* component = reinterpret_cast<void*>(self->vGetField016c());
    int orientationAddress =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, component, 0)[0x18 / 4])(component, nullptr);

    void* orientation = reinterpret_cast<void*>(orientationAddress);
    float yawDelta = static_cast<float>(
        reinterpret_cast<double(__fastcall*)(void*, void*)>(
            CSF_AT(void**, orientation, 0)[0x28 / 4])(orientation, nullptr));

    Rotation_ApplyYawDelta(&rotation, nullptr, yawDelta);

    alignas(8) uint8_t anchorBytes[0x10];
    float* anchor = ComputeAlertIconAnchorPosition(
        reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(self) + 0x344),
        nullptr,
        reinterpret_cast<float*>(anchorBytes));

    float horizontal = rotation.field_0000 + anchor[0];
    float vertical = rotation.field_0004 + anchor[1];

    if (vertical > g_MaximumRotation)
        vertical = 89.0f;
    else if (vertical < g_MinimumRotation)
        vertical = -89.0f;

    out[0] = horizontal;
    out[1] = vertical;
    return out;
}

HOOK(0x00494A60, CPlayerCharacter_GetAlertIconRotation, "auto");
