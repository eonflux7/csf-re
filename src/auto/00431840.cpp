#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Hud_GetAlternateSceneSelection, 0x004950B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_004950e0_GetField398, 0x004950E0, (void* self, void*));
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int code, int flag));

CSF_FUNC(void, __cdecl, Hud_RenderWidgetAtScaledRect, 0x0042F4F0, (void* widget, const int* rect));

static __declspec(naked) int __fastcall Hud_TailRefreshMessage(
    void* /*self*/, void* /*postCallEdx*/)
{
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax+0x8]
    }
}

static int __fastcall Hud_RefreshMenuSelection(void* self, void* /*edx*/)
{
    void* context =
        reinterpret_cast<void*>(GetSceneMenuContext(g_pCurrentScene, nullptr));

    int selection = 0;
    if (context)
    {
        // Unordered comparisons take the same branch as <=.
        if (CSF_AT(float, self, 0x12C) > g_HalfDurationScale)
            selection = Hud_GetAlternateSceneSelection(context, nullptr);
        else
            selection = C_004950e0_GetField398(context, nullptr);
    }

    const uint32_t selectionIndex = static_cast<uint32_t>(selection) - 1;
    uint32_t widgetIndex;
    bool hasWidgetIndex = true;
    switch (selection)
    {
    case 1: widgetIndex = 0; break;
    case 2: widgetIndex = 1; break;
    case 3: widgetIndex = 2; break;
    case 4: widgetIndex = 3; break;
    case 5: widgetIndex = 4; break;
    default:
        widgetIndex = 0;
        hasWidgetIndex = false;
        break;
    }

    if (hasWidgetIndex && widgetIndex < CSF_AT(uint32_t, self, 0x1F0))
    {
        int* widget = CSF_AT(int**, self, 0x1E8)[widgetIndex];
        if (widget)
        {
            Hud_RenderWidgetAtScaledRect(widget, nullptr);
        }
    }

    uint32_t messageObjectValue = CSF_AT(uint32_t, self, 0x208);
    if (!messageObjectValue)
        return 0;

    int messageKey;
    switch (selectionIndex)
    {
    case 0: messageKey = 1194342708; break;
    case 1: messageKey = 1194342709; break;
    case 2: messageKey = 1194342710; break;
    case 3: messageKey = 1194342711; break;
    case 4: messageKey = 1194342712; break;
    default: return static_cast<int>(messageObjectValue);
    }

    void* messageObject = CSF_AT(void*, self, 0x208);
    void** messageVtable = CSF_AT(void**, messageObject, 0);
    int entry = MenuMessageBox_ResolveEntryByKey(messageKey, 8);

    // Capture machine EDX:EAX; EDX must survive the final forwarding jump.
    using SetMessageAndCaptureRegisters =
        uint64_t(__fastcall*)(void*, void*, int);
    uint64_t postCallRegisters =
        reinterpret_cast<SetMessageAndCaptureRegisters>(messageVtable[16])(
            CSF_AT(void*, self, 0x208), nullptr, entry);

    void* refreshedObject = CSF_AT(void*, self, 0x208);
    void* postCallEdx = reinterpret_cast<void*>(
        static_cast<uint32_t>(postCallRegisters >> 32));

    [[clang::musttail]]
    return Hud_TailRefreshMessage(refreshedObject, postCallEdx);
}

HOOK(0x00431840, Hud_RefreshMenuSelection, "auto");
