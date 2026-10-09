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

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHud_ApplySceneEvent, 0x00431970, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));

// Dispatches a scene event record by its type field (event+4):
//   type 3 -> resolve the handle at event+8 through the current scene's handle table
//   type 4 -> ask the scene's menu context whether it accepts the event; if so, apply it locally
//   other  -> returns type - 4
static int __fastcall CHud_DispatchSceneEvent(CHud* self, void* /*edx*/, int a2) {
    uint8_t* event = reinterpret_cast<uint8_t*>(a2);
    const uint32_t eventType = CSF_AT(uint32_t, event, 4);

    if (eventType == 3) {
        return CHandleTable_Resolve(g_pCurrentScene, nullptr, reinterpret_cast<int>(event + 8));
    }

    int result = static_cast<int>(eventType) - 4;
    if (eventType == 4) {
        const uint32_t menuContextRaw = GetSceneMenuContext(g_pCurrentScene, nullptr);
        result = static_cast<int>(menuContextRaw);
        if (result != 0) {
            uint8_t* menuContext = reinterpret_cast<uint8_t*>(menuContextRaw);
            void* control = CSF_AT(void*, menuContext, 0x38);
            void* slot = CSF_AT(void**, control, 0)[36]; // vtbl + 0x90
            using AcceptEventFn = int(__fastcall*)(void*, void*, uint32_t, int, int);
            AcceptEventFn acceptEvent = reinterpret_cast<AcceptEventFn>(slot);
            const int accepted = acceptEvent(control, nullptr,
                                             CSF_AT(uint32_t, event, 0x60),
                                             0,
                                             reinterpret_cast<int>(event + 8));
            if (static_cast<uint8_t>(accepted) != 0) {
                return CHud_ApplySceneEvent(self, nullptr, a2);
            }
            result = accepted;
        }
    }
    return result;
}

HOOK(0x004330A0, CHud_DispatchSceneEvent, "auto");
