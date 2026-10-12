#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, Hud_RenderWidgetAtScaledRect, 0x0042F4F0, (void* widget, const int* rect));
CSF_FUNC(char, __fastcall, MinimapRadar_RenderEntity, 0x0043BF20, (void* self, void* edx, void* arg0));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, GfxSkin_ForwardToRwEngineSlot20, 0x006C7B40, (void* engine, void* edx, int a1, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall C_00830e5c_RenderRadar(C_00830e5c* self, void* /*edx*/)
{
    char result = static_cast<char>(self->field_0008);
    if (!result)
        return result;

    int global = GetGlobalValue_6C5760();
    void* engine = CSF_AT(void*, reinterpret_cast<void*>(global), 4);

    GfxSkin_ForwardToRwEngineSlot20(engine, nullptr, 20, 1);
    GfxSkin_ForwardToRwEngineSlot20(engine, nullptr, 6, 0);
    GfxSkin_ForwardToRwEngineSlot20(engine, nullptr, 8, 0);
    GfxSkin_ForwardToRwEngineSlot20(engine, nullptr, 29, 5);
    GfxSkin_ForwardToRwEngineSlot20(engine, nullptr, 30, 0);
    GfxSkin_ForwardToRwEngineSlot20(engine, nullptr, 10, 5);
    GfxSkin_ForwardToRwEngineSlot20(engine, nullptr, 11, 6);

    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x20 / 4])(self, nullptr);

    void* child = reinterpret_cast<void*>(self->field_001c);
    if (child)
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[8 / 4])(child, nullptr);

    uint32_t count = CSF_AT(uint32_t, g_pCurrentScene, 0x28);
    for (uint32_t i = 0; i < count; ++i)
    {
        void* entity = CSF_AT(void**, g_pCurrentScene, 0x20)[i];
        if (entity)
        {
            void* r = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, entity, 0)[0x28 / 4])(entity, nullptr);
            if (r)
                MinimapRadar_RenderEntity(self, nullptr, entity);
        }
    }

    uint8_t* widgets = reinterpret_cast<uint8_t*>(self->field_000c);
    Hud_RenderWidgetAtScaledRect(CSF_AT(void*, widgets, 0), nullptr);
    widgets = reinterpret_cast<uint8_t*>(self->field_000c);
    Hud_RenderWidgetAtScaledRect(CSF_AT(void*, widgets, 0x34), nullptr);

    GfxSkin_ForwardToRwEngineSlot20(engine, nullptr, 20, 2);
    GfxSkin_ForwardToRwEngineSlot20(engine, nullptr, 6, 1);
    GfxSkin_ForwardToRwEngineSlot20(engine, nullptr, 8, 1);
    return static_cast<char>(GfxSkin_ForwardToRwEngineSlot20(engine, nullptr, 29, 8));
}

HOOK(0x0043C160, C_00830e5c_RenderRadar, "auto");
