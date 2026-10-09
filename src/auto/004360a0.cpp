#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(void, __fastcall, SceneRoot_Refresh, 0x004A0410, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(void, __fastcall, HudMenu_UpdateItem, 0x0042DAB0, (void* self, void* edx, void* a1));
CSF_FUNC(int, __fastcall, HudMenu_HandleActiveItem, 0x00435DD0, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BB4CC, 0x006C8B80, ());

static int __fastcall Hud_RefreshSceneMenuItems(void* self, void* /*edx*/)
{
    SceneRoot_Refresh(CSF_AT(void*, g_pCurrentScene, 0xAC), nullptr);

    void* scene = g_pCurrentScene;
    const uint32_t initialCount = CSF_AT(uint32_t, scene, 0x68);
    uint8_t* arrayHeader = static_cast<uint8_t*>(scene) + 0x60;
    uint32_t index = 0;

    if (initialCount != 0) {
        do {
            void* item = CSF_AT(void**, arrayHeader, 0)[index];

            if (reinterpret_cast<uintptr_t>(item) ==
                GetSceneMenuContext(g_pCurrentScene, nullptr)) {
                HudMenu_HandleActiveItem(self, nullptr, reinterpret_cast<int>(item));
            }

            HudMenu_UpdateItem(self, nullptr, item);
            ++index;
        } while (index < CSF_AT(uint32_t, arrayHeader, 0x08));
    }

    return GetGlobalValue_8BB4CC();
}

HOOK(0x004360A0, Hud_RefreshSceneMenuItems, "auto");
