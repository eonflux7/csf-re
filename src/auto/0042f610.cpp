#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Screen scale helper: fills float[2] with the (x, y) scale factors.
CSF_FUNC(unsigned int, __cdecl, Hud_ComputeScreenScaleFactors, 0x0042DE60, (float* a1));

// Widget virtual slots (byte offset / 4):
//   2 = Refresh, 7 = SetPosition, 8 = GetPosition, 9 = SetSize, 10 = GetSize.

static int* HudWidget_GetPosition(int* widget)
{
    auto getPosition = reinterpret_cast<int* (__fastcall*)(void*, void*)>(CSF_AT(void**, widget, 0)[8]);
    return getPosition(widget, nullptr);
}

static int* HudWidget_GetSize(int* widget)
{
    auto getSize = reinterpret_cast<int* (__fastcall*)(void*, void*)>(CSF_AT(void**, widget, 0)[10]);
    return getSize(widget, nullptr);
}

static void HudWidget_SetPosition(int* widget, int* values)
{
    auto setPosition = reinterpret_cast<void (__fastcall*)(void*, void*, int*)>(CSF_AT(void**, widget, 0)[7]);
    setPosition(widget, nullptr, values);
}

static void HudWidget_SetSize(int* widget, int* values)
{
    auto setSize = reinterpret_cast<void (__fastcall*)(void*, void*, int*)>(CSF_AT(void**, widget, 0)[9]);
    setSize(widget, nullptr, values);
}

static void HudWidget_Refresh(int* widget)
{
    auto refresh = reinterpret_cast<void (__fastcall*)(void*, void*)>(CSF_AT(void**, widget, 0)[2]);
    refresh(widget, nullptr);
}

// Scales an integer coordinate by a float factor, truncating toward zero through a 64-bit intermediate.
static int Hud_ScaleComponent(int value, float factor)
{
    double scaled = static_cast<double>(value) * static_cast<double>(factor);
    return static_cast<int>(static_cast<int64_t>(scaled));
}

// Temporarily applies screen-scaled position and size to a HUD widget, refreshes it,
// then restores the original position and size.
static void __cdecl Hud_DrawWidgetScaled(int* widget)
{
    if (!widget)
        return;

    int* position = HudWidget_GetPosition(widget);
    int originalPos[2] = { position[0], position[1] };

    int* size = HudWidget_GetSize(widget);
    int originalSize[2] = { size[0], size[1] };

    int scaledPos[2] = { originalPos[0], originalPos[1] };
    int scaledSize[2] = { originalSize[0], originalSize[1] };

    float scale[2] = { 0.0f, 0.0f };
    Hud_ComputeScreenScaleFactors(scale);
    scaledPos[0] = Hud_ScaleComponent(scaledPos[0], scale[0]);
    scaledPos[1] = Hud_ScaleComponent(scaledPos[1], scale[1]);

    scale[0] = 0.0f;
    scale[1] = 0.0f;
    Hud_ComputeScreenScaleFactors(scale);
    scaledSize[0] = Hud_ScaleComponent(scaledSize[0], scale[0]);
    scaledSize[1] = Hud_ScaleComponent(scaledSize[1], scale[1]);

    HudWidget_SetPosition(widget, scaledPos);
    HudWidget_SetSize(widget, scaledSize);
    HudWidget_Refresh(widget);
    HudWidget_SetPosition(widget, originalPos);
    HudWidget_SetSize(widget, originalSize);
}

HOOK(0x0042F610, Hud_DrawWidgetScaled, "auto");
