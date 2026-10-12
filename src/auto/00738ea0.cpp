#include <cstdint>
#include "globals.h"
#include "hook.h"

using SweepCoefficients = float[8];
using SweepAxes = uint8_t[8];

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ScoreboardRefreshInterval, 0x0082DAF0);
CSF_GLOBAL(float, g_SweepEpsilon, 0x008673AC);
CSF_GLOBAL(SweepCoefficients, g_ForwardSweepCoefficients, 0x008A88DC);
CSF_GLOBAL(SweepCoefficients, g_ReverseSweepCoefficients, 0x008A88FC);
CSF_GLOBAL(SweepAxes, g_SweepAxes, 0x008A891C);

CSF_GLOBAL(float, g_CenterX, 0x008BBF38);
CSF_GLOBAL(float, g_CenterY, 0x008BBF3C);
CSF_GLOBAL(float, g_CenterU, 0x008BBF4C);
CSF_GLOBAL(float, g_CenterV, 0x008BBF50);
CSF_GLOBAL(float, g_FirstX, 0x008BBF54);
CSF_GLOBAL(float, g_FirstY, 0x008BBF58);
CSF_GLOBAL(float, g_FirstU, 0x008BBF68);
CSF_GLOBAL(float, g_FirstV, 0x008BBF6C);
CSF_GLOBAL(float, g_SecondX, 0x008BBF70);
CSF_GLOBAL(float, g_SecondY, 0x008BBF74);
CSF_GLOBAL(float, g_SecondU, 0x008BBF84);
CSF_GLOBAL(float, g_SecondV, 0x008BBF88);

static void __fastcall RenderRadialSweep(
    void* self, void* /*edx*/, float amount, int forward)
{
    float remaining = amount * g_ScoreboardRefreshInterval;
    float firstU = 0.5f;
    float firstV = 1.0f;
    float secondU = 0.5f;
    float secondV = 1.0f;

    for (int segment = 0; remaining > g_SweepEpsilon; ++segment)
    {
        if (segment >= 8)
            break;

        float step = g_HalfDurationScale;
        if (remaining <= g_HalfDurationScale)
        {
            step = remaining;
            remaining = 0.0f;
        }
        else
        {
            remaining -= g_HalfDurationScale;
        }

        const uint8_t axis = g_SweepAxes[segment];
        if (forward)
        {
            const float delta = step * g_ForwardSweepCoefficients[segment];
            if (axis)
                secondU = delta + firstU;
            else
                secondV = delta + firstV;
        }
        else
        {
            const float delta = step * g_ReverseSweepCoefficients[segment];
            if (axis)
                firstU = delta + secondU;
            else
                firstV = delta + secondV;
        }

        float textureHeight =
            CSF_AT(float, self, 0x28) - CSF_AT(float, self, 0x20);
        g_CenterU = (CSF_AT(float, self, 0x24) - CSF_AT(float, self, 0x1C))
            * g_HalfDurationScale + CSF_AT(float, self, 0x1C);
        g_CenterV = textureHeight * g_HalfDurationScale
            + CSF_AT(float, self, 0x20);
        g_CenterX = static_cast<float>(CSF_AT(int32_t, self, 0x0C));
        g_CenterY = static_cast<float>(CSF_AT(int32_t, self, 0x10)) + g_fZero;

        textureHeight =
            CSF_AT(float, self, 0x28) - CSF_AT(float, self, 0x20);
        g_FirstU = (CSF_AT(float, self, 0x24) - CSF_AT(float, self, 0x1C))
            * firstU + CSF_AT(float, self, 0x1C);
        g_FirstV = textureHeight * firstV + CSF_AT(float, self, 0x20);

        float height = static_cast<float>(CSF_AT(int32_t, self, 0x18));
        float verticalOffset = g_HalfDurationScale * height - height * firstV;
        float width = static_cast<float>(CSF_AT(int32_t, self, 0x14));
        g_FirstX = firstU * width - width * g_HalfDurationScale
            + static_cast<float>(CSF_AT(int32_t, self, 0x0C));
        g_FirstY = static_cast<float>(CSF_AT(int32_t, self, 0x10))
            + verticalOffset;

        textureHeight =
            CSF_AT(float, self, 0x28) - CSF_AT(float, self, 0x20);
        g_SecondU = (CSF_AT(float, self, 0x24) - CSF_AT(float, self, 0x1C))
            * secondU + CSF_AT(float, self, 0x1C);
        g_SecondV = textureHeight * secondV + CSF_AT(float, self, 0x20);

        height = static_cast<float>(CSF_AT(int32_t, self, 0x18));
        verticalOffset = g_HalfDurationScale * height - height * secondV;
        width = static_cast<float>(CSF_AT(int32_t, self, 0x14));
        g_SecondX = secondU * width - width * g_HalfDurationScale
            + static_cast<float>(CSF_AT(int32_t, self, 0x0C));
        g_SecondY = static_cast<float>(CSF_AT(int32_t, self, 0x10))
            + verticalOffset;

        void* renderer = CSF_AT(void*, self, 0x04);
        reinterpret_cast<void (__fastcall*)(void*, void*, int, float*, int)>(
            CSF_AT(void**, renderer, 0)[0x24 / 4])(
                renderer, nullptr, 3, &g_CenterX, 3);

        if (forward)
        {
            firstU = secondU;
            firstV = secondV;
        }
        else
        {
            secondU = firstU;
            secondV = firstV;
        }
    }
}

HOOK(0x00738EA0, RenderRadialSweep, "auto");
