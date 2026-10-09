#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(void, __cdecl, HudMenu_DrawSceneMenuLayer, 0x0042F4F0, (int* a2, int a3));

static void __fastcall HudOverlay_DrawSceneMenuIfEnabled(void* self, void* /*edx*/)
{
    void* globalsBase = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    if (CSF_AT(uint8_t, globalsBase, 0x225C) != 0)
        return;

    if (CSF_AT(uint8_t, self, 0x2A0) == 0)
        return;

    if (CSF_AT(uint32_t, self, 0x104) == 0)
        return;

    uint32_t sceneMenuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (sceneMenuContext == 0)
        return;

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(sceneMenuContext), 0x3DC) == 0)
        return;

    HudMenu_DrawSceneMenuLayer(reinterpret_cast<int*>(CSF_AT(uint32_t, self, 0x104)), 0);
}

HOOK(0x00431620, HudOverlay_DrawSceneMenuIfEnabled, "auto");
