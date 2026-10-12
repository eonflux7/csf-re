#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));

static double __cdecl Minimap_GetFloatByContextFlag() {
    // Both calls are made in the original; the global is reloaded before each.
    uint8_t* menuContextA = reinterpret_cast<uint8_t*>(GetSceneMenuContext(g_pCurrentScene, nullptr));
    uint8_t* viewData = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, menuContextA, 0x3C));

    uint8_t* menuContextB = reinterpret_cast<uint8_t*>(GetSceneMenuContext(g_pCurrentScene, nullptr));
    uint32_t flags = CSF_AT(uint32_t, menuContextB, 0x2E0);

    if ((flags & 0x40000) != 0) {
        return static_cast<double>(CSF_AT(float, viewData, 0x38));
    }
    return static_cast<double>(CSF_AT(float, viewData, 0x34));
}

HOOK(0x0043C2C0, Minimap_GetFloatByContextFlag, "auto");
