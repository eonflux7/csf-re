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

// Scene menu context accessor: returns the menu context for g_pCurrentScene.
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Reloads g_pCurrentScene on every call, as the original does.
static uint8_t* HudGetCurrentSceneMenuContext() {
    return reinterpret_cast<uint8_t*>(GetSceneMenuContext(g_pCurrentScene, nullptr));
}

// Refreshes the cached menu snapshot stored in the hud object.
// Returns 1 when the menu id changed since the last snapshot, 0 otherwise.
static char __fastcall HudCheckSceneMenuChanged(void* self, void* /*edx*/) {
    if (CSF_AT(int, self, 0x220) == -1) {
        // First run: capture the menu id, its secondary value, and the
        // object at +0x38 whose vtable slot 40 yields the cached value.
        uint8_t* menuContext = HudGetCurrentSceneMenuContext();
        CSF_AT(uint32_t, self, 0x220) = CSF_AT(uint32_t, menuContext, 0x20);
        CSF_AT(uint32_t, self, 0x224) = CSF_AT(uint32_t, menuContext, 0x24);

        uint8_t* menuContextForObject = HudGetCurrentSceneMenuContext();
        void* menuObject = reinterpret_cast<void*>(CSF_AT(uint32_t, menuContextForObject, 0x38));
        uint32_t objectValue =
            ((uint32_t(__fastcall*)(void*, void*))CSF_AT(void**, menuObject, 0)[40])(menuObject, nullptr);
        CSF_AT(uint32_t, self, 0x218) = objectValue;

        CSF_AT(uint32_t, self, 0x20C) = 0;
        CSF_AT(uint32_t, self, 0x214) = 0;
        CSF_AT(uint32_t, self, 0x210) = 0;
    }
    else if (CSF_AT(uint32_t, self, 0x220) != CSF_AT(uint32_t, HudGetCurrentSceneMenuContext(), 0x20)) {
        // Menu id changed: store the new id and secondary value.
        uint8_t* menuContext = HudGetCurrentSceneMenuContext();
        CSF_AT(uint32_t, self, 0x220) = CSF_AT(uint32_t, menuContext, 0x20);
        CSF_AT(uint32_t, self, 0x224) = CSF_AT(uint32_t, menuContext, 0x24);
        return 1;
    }
    return 0;
}

HOOK(0x42D980, HudCheckSceneMenuChanged, "auto");
