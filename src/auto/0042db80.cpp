#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ColorsMatch, 0x0042D680, (float* current, void* edx, float* target));
CSF_FUNC(void*, __fastcall, Hud_GetSceneState, 0x0044F8B0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_Unknown_GetByte1A8, 0x0044F8D0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __fastcall, CGuiControl_GetColorAnim, 0x004FE4B0, (CGuiControl* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);
CSF_GLOBAL(float, g_ColorWhite, 0x008A7D90);
CSF_GLOBAL(float, g_ColorWhite_G, 0x008A7D94);
CSF_GLOBAL(float, g_ColorWhite_B, 0x008A7D98);
CSF_GLOBAL(float, g_ColorWhite_A, 0x008A7D9C);
CSF_GLOBAL(float, g_ColorRed, 0x008A7DB0);
CSF_GLOBAL(float, g_DefaultColorGreen, 0x008A7DB4);
CSF_GLOBAL(float, g_DefaultColorBlue, 0x008A7DB8);
CSF_GLOBAL(float, g_DefaultColorAlpha, 0x008A7DBC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall Hud_UpdateAnimatedCounter(void* self, void* /*edx*/)
{
    const char enabled = CSF_AT(char, self, 0x2A2);
    if (!enabled)
        return enabled;

    if (!CSF_AT(void*, self, 0x2C8))
        return 0;

    const std::uintptr_t sceneState =
        reinterpret_cast<std::uintptr_t>(Hud_GetSceneState(g_pCurrentScene, nullptr));

    switch (sceneState)
    {
    case 0u:
    case 1u:
    case 2u:
    case 3u:
    case 4u:
        break;
    default:
        return static_cast<char>(sceneState);
    }

    char buffer[40];
    auto* control = reinterpret_cast<CGuiControl*>(GetGlobalValue_8B0568());
    void* colorAnimation =
        reinterpret_cast<void*>(CGuiControl_GetColorAnim(control, nullptr));
    _sprintf(buffer, CSF_STR(0x0083090C, "%d"),
             CSF_AT(int, colorAnimation, 0x8));

    float color[4];
    color[0] = g_ColorWhite;
    color[2] = g_ColorWhite_B;
    color[1] = g_ColorWhite_G;
    color[3] = g_ColorWhite_A;

    if (C_Unknown_GetByte1A8(g_pCurrentScene, nullptr))
    {
        void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
        const float elapsed =
            CSF_AT(float, globalState, 0x9C) +
            CSF_AT(float, globalState, 0x9C) +
            CSF_AT(float, self, 0x2C4);
        CSF_AT(float, self, 0x2C4) = elapsed;

        if (elapsed >= g_HalfDurationScale)
        {
            float* targetColor = &CSF_AT(float, self, 0x2B4);
            if (static_cast<std::uint8_t>(
                    ColorsMatch(targetColor, nullptr, &g_ColorRed)))
            {
                CSF_AT(float, self, 0x2A4) = g_ColorRed;
                CSF_AT(float, self, 0x2A8) = g_DefaultColorGreen;
                CSF_AT(float, self, 0x2AC) = g_DefaultColorBlue;
                CSF_AT(float, self, 0x2B0) = g_DefaultColorAlpha;
                CSF_AT(float, self, 0x2B4) = g_ColorWhite;
                CSF_AT(float, self, 0x2B8) = g_ColorWhite_G;
                CSF_AT(float, self, 0x2BC) = g_ColorWhite_B;
                CSF_AT(float, self, 0x2C0) = g_ColorWhite_A;
            }
            else if (static_cast<std::uint8_t>(
                         ColorsMatch(targetColor, nullptr, &g_ColorWhite)))
            {
                CSF_AT(float, self, 0x2A4) = g_ColorWhite;
                CSF_AT(float, self, 0x2A8) = g_ColorWhite_G;
                CSF_AT(float, self, 0x2AC) = g_ColorWhite_B;
                CSF_AT(float, self, 0x2B0) = g_ColorWhite_A;
                CSF_AT(float, self, 0x2B4) = g_ColorRed;
                CSF_AT(float, self, 0x2B8) = g_DefaultColorGreen;
                CSF_AT(float, self, 0x2BC) = g_DefaultColorBlue;
                CSF_AT(float, self, 0x2C0) = g_DefaultColorAlpha;
            }

            CSF_AT(float, self, 0x2C4) =
                CSF_AT(float, self, 0x2C4) - g_HalfDurationScale;
        }

        color[0] =
            (CSF_AT(float, self, 0x2B4) - CSF_AT(float, self, 0x2A4)) *
                CSF_AT(float, self, 0x2C4) +
            CSF_AT(float, self, 0x2A4);
        color[1] =
            (CSF_AT(float, self, 0x2B8) - CSF_AT(float, self, 0x2A8)) *
                CSF_AT(float, self, 0x2C4) +
            CSF_AT(float, self, 0x2A8);
        color[2] =
            (CSF_AT(float, self, 0x2BC) - CSF_AT(float, self, 0x2AC)) *
                CSF_AT(float, self, 0x2C4) +
            CSF_AT(float, self, 0x2AC);
    }

    void* widget = CSF_AT(void*, self, 0x2C8);
    color[3] = 255.0f;
    color[0] *= g_ColorChannelScale;
    color[1] *= g_ColorChannelScale;
    color[2] *= g_ColorChannelScale;

    reinterpret_cast<void(__fastcall*)(void*, void*, char*)>(
        CSF_AT(void**, widget, 0)[0x3C / 4])(widget, nullptr, buffer);

    widget = CSF_AT(void*, self, 0x2C8);
    return static_cast<char>(
        reinterpret_cast<int(__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, widget, 0)[0x60 / 4])(widget, nullptr, color));
}

HOOK(0x0042DB80, Hud_UpdateAnimatedCounter, "auto");
