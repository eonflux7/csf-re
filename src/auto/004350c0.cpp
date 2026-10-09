#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(char, __fastcall, Menus_GetInnerObjectFlag, 0x0050D800, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __stdcall, Hud_SetRenderState, 0x006C7B40, (int a1, int a2));

// Uses the normalized signature of the supplied reimplementation.
CSF_FUNC(void, __cdecl, Hud_RenderWidgetAtScaledRect, 0x0042F4F0, (void* widget, const int* rect));

CSF_FUNC(int, __fastcall, CHud_UpdateMainWidgets, 0x00431840, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CHud_UpdateWidgetVisibility, 0x00430FE0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CHud_RenderAnchoredWidgets, 0x00431020, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CHud_RenderStatusWidgets, 0x00431350, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CHud_RenderAdditionalWidgets, 0x00431680, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CHud_RenderAnchoredOverlay, 0x0042F820, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CHud_RenderIndicatorWidgets, 0x00431530, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CHud_RenderAnchoredIndicators, 0x00431620, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CHud_RenderAnchoredMessages, 0x00432EE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHud_RenderOverlay, 0x0042DB20, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Hud_RenderAttachedOverlay, 0x004275C0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CHud_RenderMessages, 0x00432F70, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHud_RenderUpperBar, 0x0042F880, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHud_RenderLowerBar, 0x0042F900, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CHud_RenderNotifications, 0x00431B20, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHud_RenderDebugOverlay, 0x00431C50, (void* self, void* edx));
CSF_FUNC(void, __fastcall, Hud_RenderSubtitlePanel, 0x00439CF0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CHud_RenderFinalWidgets, 0x004345E0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CHud_RenderFinalOverlay, 0x004346B0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, GetGlobal8B0558, 0x004F4920, ());
CSF_FUNC(char, __fastcall, Hud_UpdateMenuThumbnails, 0x004F4A20, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Hud_IsSceneMenuActive, 0x00495520, (void* self, void* edx));

static int __fastcall CHud_Render(CHud* self, void* /*edx*/)
{
    int result = Menus_GetInnerObjectFlag(
        reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);
    if (!static_cast<uint8_t>(result))
        return result;

    GetGlobalValue_6C5760();  // the original loads its +4 into a register no callee reads (0x6C7B40 is stdcall)

    Hud_SetRenderState(20, 1);
    Hud_SetRenderState(6, 0);
    Hud_SetRenderState(8, 0);
    Hud_SetRenderState(29, 5);
    Hud_SetRenderState(30, 0);
    Hud_SetRenderState(10, 5);
    Hud_SetRenderState(11, 6);

    CHud_UpdateMainWidgets(self, nullptr);
    CHud_UpdateWidgetVisibility(self, nullptr);

    if (self->field_0118)
    {
        Hud_RenderWidgetAtScaledRect(CSF_AT(int*, reinterpret_cast<void*>(self->field_0004), 0x000),
            nullptr);
        Hud_RenderWidgetAtScaledRect(CSF_AT(int*, reinterpret_cast<void*>(self->field_0004), 0x008),
            nullptr);
        Hud_RenderWidgetAtScaledRect(CSF_AT(int*, reinterpret_cast<void*>(self->field_0004), 0x004),
            nullptr);
    }

    if (self->field_0119)
    {
        Hud_RenderWidgetAtScaledRect(CSF_AT(int*, reinterpret_cast<void*>(self->field_0014), 0x000),
            nullptr);
    }

    CHud_RenderAnchoredWidgets(self, nullptr);
    CHud_RenderStatusWidgets(self, nullptr);
    CHud_RenderAdditionalWidgets(self, nullptr);
    CHud_RenderAnchoredOverlay(self, nullptr);
    CHud_RenderIndicatorWidgets(self, nullptr);
    CHud_RenderAnchoredIndicators(self, nullptr);
    CHud_RenderAnchoredMessages(self, nullptr);
    CHud_RenderOverlay(self, nullptr);

    if (self->field_02cc)
        Hud_RenderAttachedOverlay(
            reinterpret_cast<void*>(self->field_02cc), nullptr);

    CHud_RenderMessages(self, nullptr);
    CHud_RenderUpperBar(self, nullptr);
    CHud_RenderLowerBar(self, nullptr);
    CHud_RenderNotifications(self, nullptr);
    CHud_RenderDebugOverlay(self, nullptr);

    Hud_UpdateMenuThumbnails(
        reinterpret_cast<void*>(GetGlobal8B0558()), nullptr);

    void* sceneMenu = reinterpret_cast<void*>(
        GetSceneMenuContext(g_pCurrentScene, nullptr));
    if (!sceneMenu || !Hud_IsSceneMenuActive(sceneMenu, nullptr))
    {
        if (self->field_011c)
        {
            void* widget = reinterpret_cast<void*>(self->field_00bc);
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, widget, 0)[6])(widget, nullptr);
        }

        if (self->field_024c)
        {
            if (self->field_02a1)
                Hud_RenderSubtitlePanel(
                    reinterpret_cast<void*>(self->field_024c), nullptr);
        }
    }

    CHud_RenderFinalWidgets(self, nullptr);
    CHud_RenderFinalOverlay(self, nullptr);

    Hud_SetRenderState(20, 2);
    Hud_SetRenderState(6, 1);
    Hud_SetRenderState(8, 1);
    return Hud_SetRenderState(29, 8);
}

HOOK(0x004350C0, CHud_Render, "auto");

