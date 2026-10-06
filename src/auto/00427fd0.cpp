#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);
CSF_GLOBAL(float, g_FadeRed, 0x008A7D90);
CSF_GLOBAL(float, g_FadeGreen, 0x008A7D94);
CSF_GLOBAL(float, g_FadeBlue, 0x008A7D98);
CSF_GLOBAL(float, g_FadeAlpha, 0x008A7D9C);

static int __fastcall UpdateColorFade(
    float* self, void* /*edx*/, float deltaTime)
{
    const double duration = static_cast<double>(CSF_AT(float, self, 0x3C));
    void* colorTarget =
        reinterpret_cast<void*>(CSF_AT(std::uint32_t, self, 0x20));
    const double halfDuration =
        duration * static_cast<double>(g_HalfDurationScale);

    // The original also takes the ramp branch for an unordered comparison.
    const bool rampAlpha =
        !(halfDuration <= static_cast<double>(CSF_AT(float, self, 0x44)));

    float color[4];
    color[0] = static_cast<float>(
        static_cast<double>(g_FadeRed) *
        static_cast<double>(g_ColorChannelScale));
    color[1] = static_cast<float>(
        static_cast<double>(g_FadeGreen) *
        static_cast<double>(g_ColorChannelScale));
    color[2] = static_cast<float>(
        static_cast<double>(g_FadeBlue) *
        static_cast<double>(g_ColorChannelScale));

    if (rampAlpha)
    {
        color[3] = static_cast<float>(
            (static_cast<double>(CSF_AT(float, self, 0x44)) *
             static_cast<double>(g_ColorChannelScale)) /
            halfDuration);
    }
    else
    {
        color[3] = static_cast<float>(
            static_cast<double>(g_FadeAlpha) *
            static_cast<double>(g_ColorChannelScale));
    }

    const int result =
        ((int(__fastcall*)(void*, void*, float*))
             CSF_AT(void**, colorTarget, 0)[26])(
            colorTarget, nullptr, color);

    if (rampAlpha)
    {
        CSF_AT(float, self, 0x44) = static_cast<float>(
            static_cast<double>(deltaTime) +
            static_cast<double>(CSF_AT(float, self, 0x44)));
    }
    else
    {
        CSF_AT(std::uint32_t, self, 0) = 1;
    }

    return result;
}

HOOK(0x00427FD0, UpdateColorFade, "auto");
