#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_Construct_71A010, 0x0071A010,
         (CGuiControl* self, void* edx, int argument1, int argument2));
CSF_FUNC(char, __fastcall, CGuiControl_SetDraggable, 0x007180B0,
         (CGuiControl* self, void* edx, char draggable));

CSF_GLOBAL(std::uint32_t, g_SwitchVtable, 0x00866B58);
CSF_GLOBAL(volatile std::uint32_t, g_CellTextColorSentinelRed, 0x008A7DA0);
CSF_GLOBAL(volatile std::uint32_t, g_CellTextColorSentinelGreen, 0x008A7DA4);
CSF_GLOBAL(volatile std::uint32_t, g_CellTextColorSentinelBlue, 0x008A7DA8);
CSF_GLOBAL(volatile std::uint32_t, g_CellTextColorSentinelAlpha, 0x008A7DAC);

static int __fastcall CGuiSwitch_Construct(
    CGuiSwitch* self, void* /*edx*/, int argument1, int argument2)
{
    CGuiControl_Construct_71A010(
        reinterpret_cast<CGuiControl*>(self), nullptr, argument1, argument2);

    CSF_AT(std::uint32_t, self, 0x000) =
        reinterpret_cast<std::uintptr_t>(&g_SwitchVtable);

    CSF_AT(std::uint32_t, self, 0x15C) = 0;
    CSF_AT(std::uint32_t, self, 0x160) = 0;
    CSF_AT(std::uint32_t, self, 0x164) = 0;
    CSF_AT(std::uint32_t, self, 0x168) = 0x3F800000;
    CSF_AT(std::uint32_t, self, 0x16C) = 0;
    CSF_AT(std::uint32_t, self, 0x170) = 0;
    CSF_AT(std::uint32_t, self, 0x174) = 0;
    CSF_AT(std::uint32_t, self, 0x178) = 0x3F800000;

    CSF_AT(std::uint32_t, self, 0x134) = 0;
    CSF_AT(std::uint32_t, self, 0x138) = 0;
    CSF_AT(std::uint32_t, self, 0x13C) = 0;
    CSF_AT(std::uint32_t, self, 0x140) = 0;
    CSF_AT(std::uint32_t, self, 0x144) = 0;
    CSF_AT(std::uint32_t, self, 0x148) = 0;

    CSF_AT(std::uint32_t, self, 0x15C) = g_CellTextColorSentinelRed;
    CSF_AT(std::uint32_t, self, 0x160) = g_CellTextColorSentinelGreen;
    CSF_AT(std::uint32_t, self, 0x164) = g_CellTextColorSentinelBlue;
    CSF_AT(std::uint32_t, self, 0x168) = g_CellTextColorSentinelAlpha;
    CSF_AT(std::uint32_t, self, 0x16C) = g_CellTextColorSentinelRed;
    CSF_AT(std::uint32_t, self, 0x170) = g_CellTextColorSentinelGreen;
    CSF_AT(std::uint32_t, self, 0x174) = g_CellTextColorSentinelBlue;
    CSF_AT(std::uint32_t, self, 0x178) = g_CellTextColorSentinelAlpha;

    CSF_AT(std::uint32_t, self, 0x14C) = 0;
    CSF_AT(std::uint32_t, self, 0x150) = 0;
    CSF_AT(std::uint32_t, self, 0x158) = 0;
    CSF_AT(std::uint32_t, self, 0x154) = 0;

    CGuiControl_SetDraggable(reinterpret_cast<CGuiControl*>(self), nullptr, 0);

    CSF_AT(std::uint32_t, self, 0x17C) = 0x3F800000;
    return reinterpret_cast<int>(self);
}

HOOK(0x007302A0, CGuiSwitch_Construct, "auto");
