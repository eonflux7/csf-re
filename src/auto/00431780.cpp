#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(char, __fastcall, Menus_GetInnerObjectFlag, 0x0050D800, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __stdcall, RwEngine_SetRenderState, 0x006C7B40, (int a1, int a2));
CSF_FUNC(void, __cdecl, Hud_RenderWidgetAtScaledRect, 0x0042F4F0, (void* widget, const int* rect));

static int __fastcall Hud_RenderDebugOverlay(void* self, void* /*edx*/)
{
    const char enabled = Menus_GetInnerObjectFlag(
        reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);
    if (!enabled)
        return 0;

    if (CSF_AT(int*, self, 0x154) == nullptr)
        return 0;

    void* renderGlobals = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    (void)CSF_AT(int, renderGlobals, 0x4);

    RwEngine_SetRenderState(20, 1);
    RwEngine_SetRenderState(6, 0);
    RwEngine_SetRenderState(8, 0);
    RwEngine_SetRenderState(29, 5);
    RwEngine_SetRenderState(30, 0);
    RwEngine_SetRenderState(10, 5);
    RwEngine_SetRenderState(11, 6);

    Hud_RenderWidgetAtScaledRect(CSF_AT(void*, self, 0x154), nullptr);

    RwEngine_SetRenderState(20, 2);
    RwEngine_SetRenderState(6, 1);
    RwEngine_SetRenderState(8, 1);
    return RwEngine_SetRenderState(29, 8);
}

HOOK(0x00431780, Hud_RenderDebugOverlay, "auto");
