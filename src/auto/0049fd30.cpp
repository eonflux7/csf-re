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

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_LayoutWidthAdjustment, 0x00833E44);

// Resolves a handle to a scene control; if that control has flag 0x2000 set,
// fetches its bounds through a virtual slot, offsets the Y coordinate by the
// layout adjustment, and forwards the bounds to the handle's owner.
static uint32_t* __stdcall HudAlertIcons_UpdateHandleIcon(int handle) {
    uint32_t* result = reinterpret_cast<uint32_t*>(
        static_cast<intptr_t>(CHandleTable_Resolve(g_pCurrentScene, nullptr, handle)));

    if (result != nullptr) {
        if ((CSF_AT(uint32_t, result, 0x170) & 0x2000) != 0) {
            alignas(8) uint8_t boundsBuf[0xC];
            float& coordY = *reinterpret_cast<float*>(boundsBuf + 4);

            // Virtual slot 164 / 4 = 41: fills boundsBuf with the control's bounds.
            ((void (__fastcall*)(void*, void*, void*))CSF_AT(void**, result, 0)[41])(
                result, nullptr, boundsBuf);

            coordY = coordY + g_LayoutWidthAdjustment;

            void* owner = CSF_AT(void*, reinterpret_cast<void*>(static_cast<intptr_t>(handle)), 0x14);

            // Virtual slot 56 / 4 = 14 on the owner.
            int ownerResult = ((int (__fastcall*)(void*, void*, void*))CSF_AT(void**, owner, 0)[14])(
                owner, nullptr, boundsBuf);
            return reinterpret_cast<uint32_t*>(static_cast<intptr_t>(ownerResult));
        }
    }
    return result;
}

HOOK(0x0049FD30, HudAlertIcons_UpdateHandleIcon, "auto");
