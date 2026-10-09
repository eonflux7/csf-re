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

CSF_FUNC(char, __fastcall, HudApplySceneMenuContext, 0x0043A3E0, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Marks the scene menu context of the HUD (if present) as active, then, if the
// current scene exposes a menu context, forwards the HUD object to the menu applier.
static int __fastcall HudUpdateSceneMenuVisibility(void* self, void* /*edx*/)
{
    uint32_t sceneMenuContext = CSF_AT(uint32_t, self, 0x24C);
    if (sceneMenuContext != 0)
    {
        CSF_AT(uint8_t, reinterpret_cast<void*>(sceneMenuContext), 0x4C) = 1;
        sceneMenuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
        if (sceneMenuContext != 0)
        {
            return HudApplySceneMenuContext(
                reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x24C)), nullptr);
        }
    }
    return 0;
}

HOOK(0x0042FA50, HudUpdateSceneMenuVisibility, "auto");
