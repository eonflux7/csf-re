#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, Hud_RenderWidgetAtScaledRect, 0x0042F4F0, (void* widget, const int* rect));
CSF_FUNC(int, __fastcall, Minimap_DrawPrimary, 0x0043B250, (void* self, void* edx));
CSF_FUNC(int*, __fastcall, Minimap_DrawSecondary, 0x0043B7A0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, Minimap_PrepareDraw, 0x0043C9A0, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __stdcall, GfxSkin_ForwardToRwEngineSlot20, 0x006C7B40, (int a1, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static uint8_t __fastcall C_00830e5c_RenderRadar(C_00830e5c* self, void* /*edx*/)
{
    uint8_t result = self->field_0008;
    if (!result)
        return result;

    GetGlobalValue_6C5760();
    GfxSkin_ForwardToRwEngineSlot20(20, 1);
    GfxSkin_ForwardToRwEngineSlot20(6, 0);
    GfxSkin_ForwardToRwEngineSlot20(8, 0);
    GfxSkin_ForwardToRwEngineSlot20(29, 5);
    GfxSkin_ForwardToRwEngineSlot20(30, 0);
    GfxSkin_ForwardToRwEngineSlot20(10, 5);
    GfxSkin_ForwardToRwEngineSlot20(11, 6);

    uint32_t child = self->field_001c;
    if (child)
    {
        void* childObj = reinterpret_cast<void*>(child);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, childObj, 0)[2])(childObj, nullptr);
    }

    Minimap_PrepareDraw(self, nullptr);

    uint32_t menuCtx = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (CSF_AT(uint32_t, reinterpret_cast<void*>(menuCtx), 0x3C))
    {
        Minimap_DrawPrimary(self, nullptr);
        Minimap_DrawSecondary(self, nullptr);
    }

    void* owner = reinterpret_cast<void*>(self->field_000c);
    Hud_RenderWidgetAtScaledRect(reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x28)), nullptr);

    GfxSkin_ForwardToRwEngineSlot20(20, 2);
    GfxSkin_ForwardToRwEngineSlot20(6, 1);
    GfxSkin_ForwardToRwEngineSlot20(8, 1);
    return (uint8_t)GfxSkin_ForwardToRwEngineSlot20(29, 8);
}

HOOK(0x0043CC20, C_00830e5c_RenderRadar, "auto");
