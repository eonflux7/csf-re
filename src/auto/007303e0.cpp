#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ColorMatchesSentinel, 0x0042D680,
         (float* self, void* edx, float* sentinel));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_CellTextColorSentinelRed, 0x008A7DA0);

static void __fastcall CGuiSwitch_ToggleStateAndUpdateColor(
    CGuiSwitch* self, void* /*edx*/)
{
    const uint32_t previousState = CSF_AT(uint32_t, self, 0x144);
    CSF_AT(uint32_t, self, 0x144) = previousState == 0 ? 1u : 0u;

    auto* selectedColor = reinterpret_cast<uint8_t*>(self) + 0x15C;
    if (previousState == 0)
    {
        auto* alternateColor = reinterpret_cast<uint8_t*>(self) + 0x16C;
        const int matches = ColorMatchesSentinel(
            reinterpret_cast<float*>(alternateColor), nullptr,
            &g_CellTextColorSentinelRed);
        if (static_cast<uint8_t>(matches) == 0)
            selectedColor = alternateColor;
    }

    float color[4];
    std::memcpy(&color[0], &CSF_AT(uint32_t, selectedColor, 0x00), 4);
    std::memcpy(&color[1], &CSF_AT(uint32_t, selectedColor, 0x04), 4);
    std::memcpy(&color[2], &CSF_AT(uint32_t, selectedColor, 0x08), 4);
    std::memcpy(&color[3], &CSF_AT(uint32_t, selectedColor, 0x0C), 4);

    const double halfAlpha =
        static_cast<double>(color[3]) * g_HalfDurationScale;
    std::memcpy(&CSF_AT(uint32_t, self, 0x158), &color[3], 4);
    CSF_AT(float, self, 0x154) = static_cast<float>(halfAlpha);

    float extent;
    std::memcpy(&extent, &self->field_0118, 4);
    const double halfExtent =
        static_cast<double>(extent) * g_HalfDurationScale;

    double distance;
    if (halfExtent > CSF_AT(float, self, 0x14C))
        distance = halfExtent - CSF_AT(float, self, 0x14C);
    else
        distance = CSF_AT(float, self, 0x14C) - halfExtent;

    void* colorRecipient =
        reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x148));

    const double fraction = distance / halfExtent;
    const double adjustedAlpha =
        halfAlpha + (static_cast<double>(color[3]) - halfAlpha) * fraction;

    CSF_AT(float, self, 0x150) = static_cast<float>(adjustedAlpha);
    color[3] = static_cast<float>(adjustedAlpha);

    if (colorRecipient)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, colorRecipient, 0)[0x60 / 4])(
                colorRecipient, nullptr, color);
    }
}

HOOK(0x007303E0, CGuiSwitch_ToggleStateAndUpdateColor, "auto");
