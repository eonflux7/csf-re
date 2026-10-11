#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, MenuControls_IsPressed, 0x004285B0,
         (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, MenuControls_QueryFlag, 0x004285D0,
         (void* self, void* edx, int value));
CSF_FUNC(double, __fastcall, MenuControls_GetMagnitude, 0x00428610,
         (void* self, void* edx, int a2));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60,
         (void* self, void* edx));
CSF_FUNC(double, __cdecl, Cheats_Pow, 0x007AFA60,
         (double base, double exponent));

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVector2X, 0x008BB448);
CSF_GLOBAL(float, g_ZeroVector2Y, 0x008BB44C);

static float Cheats_FloatFromBits(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static uint32_t Cheats_FloatToBits(float value)
{
    uint32_t bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static void __fastcall C_00830578_ComputeLookDelta(
    C_00830578* self, void* /*edx*/, float* delta, float elapsed)
{
    delta[0] = 0.0f;
    delta[1] = 0.0f;

    if (MenuControls_IsPressed(reinterpret_cast<void*>(self->field_0014), nullptr, 5) ||
        MenuControls_QueryFlag(reinterpret_cast<void*>(self->field_0014), nullptr, 5))
    {
        delta[1] = static_cast<float>(
            MenuControls_GetMagnitude(reinterpret_cast<void*>(self->field_0014), nullptr, 5));
    }
    else if (MenuControls_IsPressed(reinterpret_cast<void*>(self->field_0014), nullptr, 4) ||
             MenuControls_QueryFlag(reinterpret_cast<void*>(self->field_0014), nullptr, 4))
    {
        delta[1] = static_cast<float>(
            -MenuControls_GetMagnitude(reinterpret_cast<void*>(self->field_0014), nullptr, 4));
    }

    if (MenuControls_IsPressed(reinterpret_cast<void*>(self->field_0014), nullptr, 6) ||
        MenuControls_QueryFlag(reinterpret_cast<void*>(self->field_0014), nullptr, 6))
    {
        delta[0] = static_cast<float>(
            MenuControls_GetMagnitude(reinterpret_cast<void*>(self->field_0014), nullptr, 6));
    }
    else if (MenuControls_IsPressed(reinterpret_cast<void*>(self->field_0014), nullptr, 7) ||
             MenuControls_QueryFlag(reinterpret_cast<void*>(self->field_0014), nullptr, 7))
    {
        delta[0] = static_cast<float>(
            -MenuControls_GetMagnitude(reinterpret_cast<void*>(self->field_0014), nullptr, 7));
    }

    if (self->field_003d)
        delta[1] *= g_MissingMetricValue;

    if (delta[0] == g_ZeroVector2X && delta[1] == g_ZeroVector2Y)
    {
        self->field_00cc = 0;
        self->field_00d0 = 0;
        self->field_00d4 = 0;
        self->field_00d8 = 0;
        return;
    }

    self->field_00d4 = Cheats_FloatToBits(delta[0]);
    self->field_00d8 = Cheats_FloatToBits(delta[1]);

    void* context = reinterpret_cast<void*>(
        GetSceneMenuContext(g_pCurrentScene, nullptr));
    void* curve = &self->field_0044;
    if (context && (CSF_AT(uint8_t, context, 0x2E0) & 0x40))
        curve = &self->field_0084;

    if (delta[0] != g_fZero)
    {
        const float magnitude = delta[0] < 0.0f ? -delta[0] : delta[0];
        const float sign = delta[0] / magnitude;
        delta[0] = magnitude;

        if (!(magnitude <= CSF_AT(float, curve, 0x20)))
        {
            const float span =
                CSF_AT(float, curve, 0x18) - CSF_AT(float, curve, 0x10);
            const float width = g_TransitionOne - CSF_AT(float, curve, 0x20);
            double response = g_fZero;
            if (width > g_fZero)
            {
                response = Cheats_Pow(
                    (magnitude - CSF_AT(float, curve, 0x20)) / width,
                    CSF_AT(float, curve, 0x38));
            }
            delta[0] = static_cast<float>(
                response * span + CSF_AT(float, curve, 0x10));

            if (sign * Cheats_FloatFromBits(self->field_00cc) > g_fZero)
            {
                self->field_00c4 = Cheats_FloatToBits(
                    elapsed + Cheats_FloatFromBits(self->field_00c4));
            }
            else
            {
                self->field_00c4 = 0;
            }
        }
        else
        {
            double response = g_fZero;
            if (CSF_AT(float, curve, 0x20) > g_fZero)
            {
                response = Cheats_Pow(
                    magnitude / CSF_AT(float, curve, 0x20),
                    CSF_AT(float, curve, 0x30));
            }
            delta[0] = static_cast<float>(response * CSF_AT(float, curve, 0x10));
            self->field_00c4 = 0;
        }

        delta[0] = CSF_AT(float, curve, 0x28) *
                   Cheats_FloatFromBits(self->field_00c4) + delta[0];
        if (delta[0] > CSF_AT(float, curve, 0x08))
            delta[0] = CSF_AT(float, curve, 0x08);
        delta[0] = sign * delta[0];
    }

    if (delta[1] != g_fZero)
    {
        const float magnitude = delta[1] < 0.0f ? -delta[1] : delta[1];
        const float sign = delta[1] / magnitude;
        delta[1] = magnitude;

        if (!(magnitude <= CSF_AT(float, curve, 0x24)))
        {
            const float span =
                CSF_AT(float, curve, 0x1C) - CSF_AT(float, curve, 0x14);
            const float width = g_TransitionOne - CSF_AT(float, curve, 0x24);
            double response = g_fZero;
            if (width > g_fZero)
            {
                response = Cheats_Pow(
                    (magnitude - CSF_AT(float, curve, 0x24)) / width,
                    CSF_AT(float, curve, 0x3C));
            }
            delta[1] = static_cast<float>(
                response * span + CSF_AT(float, curve, 0x14));

            if (sign * Cheats_FloatFromBits(self->field_00d0) > g_fZero)
            {
                self->field_00c8 = Cheats_FloatToBits(
                    elapsed + Cheats_FloatFromBits(self->field_00c8));
            }
            else
            {
                self->field_00c8 = 0;
            }
        }
        else
        {
            double response = g_fZero;
            if (CSF_AT(float, curve, 0x24) > g_fZero)
            {
                response = Cheats_Pow(
                    magnitude / CSF_AT(float, curve, 0x24),
                    CSF_AT(float, curve, 0x34));
            }
            delta[1] = static_cast<float>(response * CSF_AT(float, curve, 0x14));
            self->field_00c8 = 0;
        }

        delta[1] = CSF_AT(float, curve, 0x2C) *
                   Cheats_FloatFromBits(self->field_00c8) + delta[1];
        if (delta[1] > CSF_AT(float, curve, 0x0C))
            delta[1] = CSF_AT(float, curve, 0x0C);
        delta[1] = sign * delta[1];
    }

    if (context && CSF_AT(int32_t, context, 0x294) >= 0)
    {
        const float scale = CSF_AT(float, curve, 0x00);
        delta[0] = scale * delta[0];
        delta[1] = scale * delta[1];
    }

    self->field_00cc = Cheats_FloatToBits(delta[0]);
    self->field_00d0 = Cheats_FloatToBits(delta[1]);

    const float horizontal = elapsed * delta[0];
    delta[0] = elapsed * delta[1];
    delta[1] = horizontal;
    delta[0] = self->field_0034 * delta[0];
    delta[1] = self->field_0038 * delta[1];
}

HOOK(0x0042C000, C_00830578_ComputeLookDelta, "auto");
