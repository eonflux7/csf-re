#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee bindings
CSF_FUNC(uint64_t, __cdecl, Hud_ComputeOverlayRect, 0x0042F3B0, (int* a1));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall Hud_RefreshSceneOverlay(void* self, void* /*edx*/)
{
    // Field at +0x240 holds the overlay widget (read before and after calls, as the original does).
    if (CSF_AT(void*, self, 0x240) == nullptr)
        return 0;

    uint32_t sceneContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (sceneContext == 0)
        return 0;

    void* sceneMenu = reinterpret_cast<void*>(sceneContext);
    if (CSF_AT(int32_t, sceneMenu, 0x78) != 2)
        return static_cast<int>(sceneContext);

    // Scene menu is in state 2: update the overlay widget.
    void* overlay = CSF_AT(void*, self, 0x240);
    void** overlayVtbl = CSF_AT(void**, overlay, 0);
    // slot 0x3C / 4 = 15: SetText-style call with the "I" tag
    ((void(__fastcall*)(void*, void*, const char*))overlayVtbl[15])(
        overlay, nullptr, CSF_STR(0x00830910, "I"));

    int32_t rect[2];
    rect[0] = 250;   // 0xFA
    rect[1] = 530;   // 0x212
    Hud_ComputeOverlayRect(rect);

    overlay = CSF_AT(void*, self, 0x240);
    overlayVtbl = CSF_AT(void**, overlay, 0);
    // slot 0x50 / 4 = 20: apply rect
    ((void(__fastcall*)(void*, void*, int*))overlayVtbl[20])(overlay, nullptr, rect);

    overlay = CSF_AT(void*, self, 0x240);
    overlayVtbl = CSF_AT(void**, overlay, 0);
    // slot 8 / 4 = 2: result of this call is the function's return value
    return ((int(__fastcall*)(void*, void*))overlayVtbl[2])(overlay, nullptr);
}

HOOK(0x0042F880, Hud_RefreshSceneOverlay, "auto");
