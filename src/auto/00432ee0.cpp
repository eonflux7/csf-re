#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(void, __cdecl, Hud_RenderWidgetAtScaledRect, 0x0042F4F0, (void* widget, const int* rect));
CSF_FUNC(int, __fastcall, CHud_LookupValueByKey, 0x00431F10, (CHud* self, void* , int key));
CSF_FUNC(void*, __fastcall, Hud_GetSceneState, 0x0044F8B0, (void* self, void* edx));

// Preserve the unknown virtual target's tail-call ABI.
static __declspec(naked) char __fastcall CHud_TailRenderAnchoredWidget(
    CHud* self, void* /*edx*/)
{
    __asm {
        mov ecx, dword ptr [ecx + 0x2C8]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 8]
    }
}

static char __fastcall CHud_RenderAnchoredMessages(CHud* self, void* /*edx*/)
{
    const uint8_t enabled = self->field_02a2;
    if (!enabled)
        return static_cast<char>(enabled);

    if (!self->field_02c8)
        return 0;

    const uintptr_t sceneState =
        reinterpret_cast<uintptr_t>(Hud_GetSceneState(g_pCurrentScene, nullptr));

    int key;
    switch (sceneState)
    {
    case 0: key = 8;  break;
    case 1: key = 9;  break;
    case 2: key = 10; break;
    case 3: key = 13; break;
    case 4: key = 14; break;
    default:
        return static_cast<char>(sceneState);
    }

    const int widget = CHud_LookupValueByKey(self, nullptr, key);
    if (widget)
        Hud_RenderWidgetAtScaledRect(reinterpret_cast<void*>(widget), nullptr);

    [[clang::musttail]] return CHud_TailRenderAnchoredWidget(self, nullptr);
}

HOOK(0x00432EE0, CHud_RenderAnchoredMessages, "auto");
