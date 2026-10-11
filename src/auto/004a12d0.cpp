#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ToggleTimer_IsActive, 0x004A1210, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GlobalStateValue_50DCF0, 0x0050DCF0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, Matrix_AddPitchAngle, 0x006AE690, (S_006ae570* self, void* , float pitchDelta));
CSF_FUNC(void, __fastcall, Rotation_ApplyYawDelta, 0x006AE6B0, (S_006ae570* self, void* , float yawDelta));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);

static void __fastcall S_004a1210_UpdateIconRotation(
    S_004a1210* self, void* /*edx*/, float deltaTime)
{
    if (!self->field_001c)
        return;

    if (static_cast<std::uint8_t>(ToggleTimer_IsActive(self, nullptr)) != 0)
    {
        self->field_0024 = 0;
        self->field_002c = 0;
        self->field_0030 = 0;
        self->field_0034 = 0;
        self->field_0038 = 0;
        return;
    }

    if (reinterpret_cast<float&>(self->field_0030) > g_fZero)
    {
        reinterpret_cast<float&>(self->field_0030) -= deltaTime;
        if (reinterpret_cast<float&>(self->field_0030) > g_fZero)
            return;
    }

    int state = GetGlobalState_50D190();
    float yawNoise = static_cast<float>(
        GlobalStateValue_50DCF0(
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(state)), nullptr) % 100)
        * CSF_AT(float, self, 0x20) * g_ProgressScale;

    state = GetGlobalState_50D190();
    float pitchNoise = static_cast<float>(
        GlobalStateValue_50DCF0(
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(state)), nullptr) % 100)
        * CSF_AT(float, self, 0x20) * g_ProgressScale;

    reinterpret_cast<float&>(self->field_002c) -= deltaTime;
    if (reinterpret_cast<float&>(self->field_002c) < g_fZero)
    {
        reinterpret_cast<float&>(self->field_002c) =
            g_TransitionOne / CSF_AT(float, self, 0x20) * g_HalfDurationScale;

        state = GetGlobalState_50D190();
        if (GlobalStateValue_50DCF0(
                reinterpret_cast<void*>(static_cast<std::uintptr_t>(state)), nullptr) % 4 > 0)
        {
            CSF_AT(int, self, 0x44) =
                !(reinterpret_cast<float&>(self->field_0034) > g_fZero) ? 1 : -1;
        }

        CSF_AT(int, self, 0x48) =
            !(reinterpret_cast<float&>(self->field_0038) > g_fZero) ? 1 : -1;
    }

    if (CSF_AT(int, self, 0x44) == -1)
        yawNoise *= g_MissingMetricValue;
    if (CSF_AT(int, self, 0x48) == -1)
        pitchNoise *= g_MissingMetricValue;

    state = GetGlobalState_50D190();
    if (GlobalStateValue_50DCF0(
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(state)), nullptr) % 2 == 0)
        yawNoise = 0.0f;

    state = GetGlobalState_50D190();
    if (GlobalStateValue_50DCF0(
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(state)), nullptr) % 2 == 0)
        pitchNoise = g_fZero;

    reinterpret_cast<float&>(self->field_0024) += deltaTime;
    if (reinterpret_cast<float&>(self->field_0024) > CSF_AT(float, self, 0x28))
        self->field_0024 = CSF_AT(std::uint32_t, self, 0x28);

    const float amplitude =
        (CSF_AT(float, self, 0x40) + CSF_AT(float, self, 0x3C))
        * g_HalfDurationScale
        * (reinterpret_cast<float&>(self->field_0024) / CSF_AT(float, self, 0x28));

    float yawDelta = yawNoise * amplitude * deltaTime;
    float pitchDelta = amplitude * pitchNoise * deltaTime;

    const float nextYaw = yawDelta + reinterpret_cast<float&>(self->field_0034);
    if (nextYaw > CSF_AT(float, self, 0x3C) ||
        nextYaw < -CSF_AT(float, self, 0x3C))
        yawDelta = -yawDelta;

    const float nextPitch = pitchDelta + reinterpret_cast<float&>(self->field_0038);
    if (nextPitch > CSF_AT(float, self, 0x40) ||
        nextPitch < -CSF_AT(float, self, 0x40))
        pitchDelta = -pitchDelta;

    auto* rotation = reinterpret_cast<S_006ae570*>(&self->field_0034);
    Rotation_ApplyYawDelta(rotation, nullptr, yawDelta);
    Matrix_AddPitchAngle(rotation, nullptr, pitchDelta);
}

HOOK(0x004A12D0, S_004a1210_UpdateIconRotation, "auto");
