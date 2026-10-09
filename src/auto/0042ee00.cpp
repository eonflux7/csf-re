#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_HudNegativePi, 0x0082D75C);
CSF_GLOBAL(float, g_HudNegativeHalfPi, 0x0082FACC);
CSF_GLOBAL(float, g_TransitionHalfPi, 0x0082FAD0);
CSF_GLOBAL(float, g_HudPi, 0x00830668);
CSF_GLOBAL(float, g_HudTwoPi, 0x0083066C);
CSF_GLOBAL(unsigned int, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(unsigned int, g_ScreenHeight, 0x00899BA0);

CSF_FUNC(double, __cdecl, Hud_GetAngleDifference, 0x0042D620, (float a1, float a2));

static int __fastcall S_0042ee00_AlignWidgetToScreenEdge(
    S_0042ee00* self, void* /*edx*/, float angle,
    std::uint32_t* snappedAngle, unsigned int* position)
{
    void* widget = CSF_AT(void*, reinterpret_cast<void*>(self->field_0064), 0);
    auto* dimensions =
        reinterpret_cast<std::uint32_t* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, widget, 0)[15])(widget, nullptr);

    std::uint32_t size[2];
    size[0] = dimensions[0];
    const unsigned int halfWidth = g_ScreenWidth >> 1;
    const std::int32_t widgetHeight = static_cast<std::int32_t>(dimensions[1]);
    position[0] = halfWidth;
    const unsigned int screenHeight = g_ScreenHeight;
    const unsigned int screenWidth = g_ScreenWidth;
    const unsigned int halfHeight = screenHeight >> 1;
    size[1] = static_cast<std::uint32_t>(widgetHeight);

    bool verticalEdge;

    // The negated comparisons also preserve the original unordered cases.
    if (!(angle > g_TransitionHalfPi) && !(angle < g_fZero))
    {
        float fromZero = angle;
        if (fromZero > g_HudPi)
            fromZero -= g_HudTwoPi;
        else if (fromZero < g_HudNegativePi)
            fromZero += g_HudTwoPi;

        float fromHalfPi = g_TransitionHalfPi - angle;
        if (fromHalfPi > g_HudPi)
            fromHalfPi -= g_HudTwoPi;
        else if (fromHalfPi < g_HudNegativePi)
            fromHalfPi += g_HudTwoPi;

        if (!(fromZero > fromHalfPi))
        {
            *snappedAngle = 0;
            verticalEdge = false;
        }
        else
        {
            *snappedAngle = 1070141403;
            verticalEdge = true;
        }
    }
    else if (angle > g_HudPi || angle < g_TransitionHalfPi)
    {
        if (angle > g_HudNegativePi || angle < g_HudNegativeHalfPi)
        {
            const float fromZero =
                static_cast<float>(Hud_GetAngleDifference(angle, 0.0f));
            const double fromNegativeHalfPi =
                Hud_GetAngleDifference(-1.5707964f, angle);

            if (fromNegativeHalfPi < fromZero)
            {
                *snappedAngle = 0;
                verticalEdge = false;
            }
            else
            {
                *snappedAngle = static_cast<std::uint32_t>(-1077342245);
                verticalEdge = true;
            }
        }
        else
        {
            const float fromNegativeHalfPi =
                static_cast<float>(Hud_GetAngleDifference(angle, -1.5707964f));
            const double fromNegativePi =
                Hud_GetAngleDifference(-3.1415927f, angle);

            if (!(fromNegativePi < fromNegativeHalfPi))
            {
                *snappedAngle = static_cast<std::uint32_t>(-1068953637);
                verticalEdge = false;
            }
            else
            {
                *snappedAngle = static_cast<std::uint32_t>(-1077342245);
                verticalEdge = true;
            }
        }
    }
    else
    {
        float fromHalfPi = angle - g_TransitionHalfPi;
        if (fromHalfPi > g_HudPi)
            fromHalfPi -= g_HudTwoPi;
        else if (fromHalfPi < g_HudNegativePi)
            fromHalfPi += g_HudTwoPi;

        float fromPi = g_HudPi - angle;
        if (fromPi > g_HudPi)
            fromPi -= g_HudTwoPi;
        else if (fromPi < g_HudNegativePi)
            fromPi += g_HudTwoPi;

        if (!(fromHalfPi > fromPi))
        {
            *snappedAngle = 1070141403;
            verticalEdge = true;
        }
        else
        {
            *snappedAngle = 1078530011;
            verticalEdge = false;
        }
    }

    if (verticalEdge)
    {
        position[1] = halfHeight + widgetHeight / 2 - halfWidth;
        size[0] = screenHeight;
    }
    else
    {
        position[1] = widgetHeight / 2;
        size[0] = screenWidth;
    }

    widget = CSF_AT(void*, reinterpret_cast<void*>(self->field_0064), 0);
    return reinterpret_cast<int (__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, widget, 0)[14])(widget, nullptr, size);
}

HOOK(0x0042EE00, S_0042ee00_AlignWidgetToScreenEdge, "auto");
