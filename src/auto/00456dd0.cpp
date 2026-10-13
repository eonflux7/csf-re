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

// Original addresses bound by address.
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(unsigned int, __fastcall, sub_456D80, 0x00456D80, (void* self, void* edx, int a2));

// Looks up a handle record (type tag 8 at +4, handle at +8) in the current scene's
// handle table and, if the control resolves, forwards its +0x90 value to sub_456D80.
static unsigned int __fastcall HudPortraits_ResolveHandleAndApply(C_008312e0* self, void* /*edx*/, unsigned int a2)
{
    unsigned int result = a2;
    uint8_t* record = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2));

    if (CSF_AT(uint32_t, record, 4) == 8)
    {
        int handle = static_cast<int>(a2 + 8);
        uint8_t* control = reinterpret_cast<uint8_t*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));
        result = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(control));
        if (control)
        {
            return sub_456D80(self, nullptr, static_cast<int>(CSF_AT(uint32_t, control, 0x90)));
        }
    }
    return result;
}

HOOK(0x00456DD0, HudPortraits_ResolveHandleAndApply, "auto");
