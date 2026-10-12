#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);
CSF_GLOBAL(float, g_MinimumMotionMagnitude, 0x0082E088);
CSF_GLOBAL(float, g_MotionScale, 0x0082E08C);

static int __fastcall C_0082e090_InitializeMovement(
    C_0082e090* self, void* /*edx*/, const uint32_t* initialVector)
{
    switch (CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 0x50))
    {
    case 8:
    case 9:
    case 0xE:
    case 0xF:
    case 0x10:
    case 0x14:
    case 0x15:
    {
        self->field_0078 = 1;

        const float signX =
            GetStateValue(reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr, 2) > 0
                ? 1.0f : -1.0f;
        const float signY =
            GetStateValue(reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr, 2) > 0
                ? 1.0f : -1.0f;
        const float signZ =
            GetStateValue(reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr, 2) > 0
                ? 1.0f : -1.0f;

        const int32_t magnitudeX = static_cast<int32_t>(
            static_cast<uint32_t>(GetStateValue(
                reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr, 10)) + 1);
        const float scaledX = static_cast<float>(magnitudeX) * g_flt_0082DC84;

        const int32_t magnitudeY = static_cast<int32_t>(
            static_cast<uint32_t>(GetStateValue(
                reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr, 10)) + 1);
        const float driftX = scaledX * signX;
        const float driftY =
            static_cast<float>(magnitudeY) * g_flt_0082DC84 * signY;

        const int32_t magnitudeZ = static_cast<int32_t>(
            static_cast<uint32_t>(GetStateValue(
                reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr, 10)) + 1);

        std::memcpy(&self->field_009c, &driftX, sizeof(float));
        CSF_AT(float, self, 0xA0) = driftY;
        CSF_AT(float, self, 0xA4) =
            static_cast<float>(magnitudeZ) * g_flt_0082DC84 * signZ;
        break;
    }
    case 0xB:
        self->field_0078 = 0;
        break;
    default:
        self->field_0078 = 1;
        break;
    }

    self->field_007c = 0;
    self->field_0080 = 0.0f;
    self->field_0075 = 1;
    self->field_0077 = 1;

    uint32_t componentBits = initialVector[0];
    std::memcpy(&self->field_0084, &componentBits, sizeof(componentBits));
    componentBits = initialVector[1];
    std::memcpy(&self->field_0088, &componentBits, sizeof(componentBits));

    const uint8_t motionEnabled = self->field_0078;
    componentBits = initialVector[2];
    std::memcpy(&CSF_AT(float, self, 0x8C), &componentBits, sizeof(componentBits));

    if (!motionEnabled)
    {
        self->field_00b8 = -0.050000001f;
    }
    else
    {
        const float motion = self->field_0088 * g_MotionScale;
        self->field_00b8 = motion;
        if (std::fabs(motion) < g_MinimumMotionMagnitude)
            self->field_00b8 = -0.050000001f;
    }

    void* owner = reinterpret_cast<void*>(self->field_0004);
    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, owner, 0)[0x6C / 4])(owner, nullptr, 1);
}

HOOK(0x00416000, C_0082e090_InitializeMovement, "auto");
