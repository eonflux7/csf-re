#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(char, __fastcall, IsSceneMenuReady, 0x00495520, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __fastcall, CGuiControl_GetColorAnim, 0x004FE4B0, (CGuiControl* self, void*));
CSF_FUNC(uint64_t, __stdcall, ComputeControlAnimationKey, 0x00444BB0, (int a1, int a2, uint32_t* a3, char a4));
CSF_FUNC(int, __cdecl, IsAlternateControlKind, 0x00755860, (int a1));
CSF_FUNC(void, __cdecl, Hud_RenderWidgetAtScaledRect, 0x0042F4F0, (void* widget, const int* rect));
CSF_FUNC(void, __fastcall, S_0042eb60_ApplyHighlightState, 0x00431130, (void* self, void* edx, unsigned int a2, int a3, int a4, char a5));
CSF_FUNC(void, __fastcall, CHud_DispatchToResolvedControl, 0x00431950, (void* self, void* edx, int requestId));

static void __fastcall S_0042eb60_UpdateSceneMenuHighlight(
    S_0042eb60* self, void* /*edx*/)
{
    if (self->field_0290 != 3)
        return;

    void* sceneMenu = reinterpret_cast<void*>(
        GetSceneMenuContext(g_pCurrentScene, nullptr));
    if (!sceneMenu)
        return;

    if (!IsSceneMenuReady(sceneMenu, nullptr))
        return;

    if (CSF_AT(int32_t, sceneMenu, 632) < 0)
        return;

    uint8_t* handle = static_cast<uint8_t*>(sceneMenu) + 0x270;
    void* resolved = reinterpret_cast<void*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr, reinterpret_cast<int>(handle)));
    if (!resolved)
        return;

    int kind = CSF_AT(int, resolved, 80);
    if (kind == 1)
    {
        int selector = CSF_AT(uint32_t, sceneMenu, 636) != 1 ? 0 : 2;
        uint32_t flags;

        CGuiControl* globalControl =
            reinterpret_cast<CGuiControl*>(GetGlobalValue_8B0568());
        CGuiControl_GetColorAnim(globalControl, nullptr);

        unsigned int animationKey = static_cast<unsigned int>(
            ComputeControlAnimationKey(
                reinterpret_cast<int>(resolved), selector, &flags, 0));

        if (self->field_0290)
        {
            void* widgets = reinterpret_cast<void*>(self->field_0288);
            int* widget = CSF_AT(int*, widgets, 0);
            Hud_RenderWidgetAtScaledRect(widget, nullptr);

            widgets = reinterpret_cast<void*>(self->field_0288);
            widget = CSF_AT(int*, widgets, 4);
            Hud_RenderWidgetAtScaledRect(widget, nullptr);
        }

        int requestId = CSF_AT(int, resolved, 124);
        CHud_DispatchToResolvedControl(self, nullptr, requestId);

        if ((flags & 1) == 0)
        {
            S_0042eb60_ApplyHighlightState(
                self, nullptr, animationKey, 5, 1,
                static_cast<char>((flags & 4) != 0));
        }
    }
    else if (static_cast<uint8_t>(IsAlternateControlKind(kind)))
    {
        void* widgets = reinterpret_cast<void*>(self->field_0288);
        int* widget = CSF_AT(int*, widgets, 8);
        Hud_RenderWidgetAtScaledRect(widget, nullptr);
    }
}

HOOK(0x00432F70, S_0042eb60_UpdateSceneMenuHighlight, "auto");
