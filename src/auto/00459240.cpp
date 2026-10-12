#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x00459140 - HUD widget preparation step, runs before the draw/refresh call.
CSF_FUNC(int, __fastcall, HudWidget_PrepareState, 0x00459140,
         (void* self, void* /*edx*/, float a2));

// 0x00458F40 - HUD primary widget refresh; its result is the function's result.
CSF_FUNC(int, __fastcall, HudWidget_RefreshPrimary, 0x00458F40,
         (void* self, void* /*edx*/, float a2));

// Vtable reference of S_00458f40 used by the original's call target table.
static void* const s_S00458f40VtableRef = CSF_VTBL(0x00458F40);

// 0x00459240 - primary HUD widget update: prepares the widget state, then
// refreshes the primary widget, forwarding the refresh result unchanged.
static int __fastcall Hud_UpdatePrimaryWidget(S_00458f40* self, void* /*edx*/, float a2)
{
    HudWidget_PrepareState(self, nullptr, a2);
    return HudWidget_RefreshPrimary(self, nullptr, a2);
}

HOOK(0x459240, Hud_UpdatePrimaryWidget, "auto");
