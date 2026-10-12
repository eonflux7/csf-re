#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, Minimap_SetupRenderState, 0x0043B250, (void* self, void* edx));
CSF_FUNC(int*, __fastcall, Minimap_FinishRender, 0x0043B7A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Minimap_IsOverlayActive, 0x0043D2C0, ());
CSF_FUNC(int, __fastcall, Minimap_DrawOverlay, 0x0043D3C0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Minimap_UpdateOverlay, 0x0043D4B0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, Minimap_PrepareOverlay, 0x0043DBC0, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __stdcall, GfxSkin_ForwardToRwEngineSlot20, 0x006C7B40, (int a1, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall C_00830e5c_Render(C_00830e5c* self, void* /*edx*/)
{
    char result = (char)self->field_0008;
    if (!result)
        return result;

    uint32_t menuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (self->field_001c && menuContext) {
        if ((char)Minimap_IsOverlayActive())
            Minimap_PrepareOverlay(self, nullptr);
        Minimap_UpdateOverlay(self, nullptr);
        uint32_t obj = self->field_001c;
        if (obj) {
            void** vtbl = CSF_AT(void**, reinterpret_cast<void*>(obj), 0);
            ((void(__fastcall*)(void*, void*))vtbl[2])(reinterpret_cast<void*>(obj), nullptr);
        }
    }

    GetGlobalValue_6C5760();
    GfxSkin_ForwardToRwEngineSlot20(20, 1);
    GfxSkin_ForwardToRwEngineSlot20(6, 0);
    GfxSkin_ForwardToRwEngineSlot20(8, 0);
    GfxSkin_ForwardToRwEngineSlot20(29, 5);
    GfxSkin_ForwardToRwEngineSlot20(30, 0);
    GfxSkin_ForwardToRwEngineSlot20(10, 5);
    GfxSkin_ForwardToRwEngineSlot20(11, 6);

    Minimap_SetupRenderState(self, nullptr);
    if (self->field_001c) {
        if ((char)Minimap_IsOverlayActive())
            Minimap_DrawOverlay(self, nullptr);
    }
    Minimap_FinishRender(self, nullptr);

    GfxSkin_ForwardToRwEngineSlot20(20, 2);
    GfxSkin_ForwardToRwEngineSlot20(6, 1);
    GfxSkin_ForwardToRwEngineSlot20(8, 1);
    return (char)GfxSkin_ForwardToRwEngineSlot20(29, 8);
}

HOOK(0x0043DF10, C_00830e5c_Render, "auto");
