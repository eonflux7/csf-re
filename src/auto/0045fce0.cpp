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

// Resolves the scene object owned by the record at a3, and if it has a
// linked component (at +0x34), sends it message 29 through vtable slot 42.
static uint32_t* __stdcall SendMessage29ToResolvedHandle(int a1, int a2, int a3) {
    (void)a1;
    (void)a2;

    uint32_t owner = *reinterpret_cast<uint32_t*>(a3);
    uint32_t v3 = *reinterpret_cast<uint32_t*>(owner + 4);
    int v4 = *reinterpret_cast<int*>(v3 + 12);
    uint32_t result = v3 + 4;

    if (v4 >= 0) {
        uint32_t* resolved = reinterpret_cast<uint32_t*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(result)));
        result = reinterpret_cast<uint32_t>(resolved);
        if (resolved) {
            uint32_t component = CSF_AT(uint32_t, resolved, 0x34);
            result = component;
            if (component) {
                void* componentObj = reinterpret_cast<void*>(component);
                a3 = 29;
                result = ((uint32_t(__fastcall*)(void*, void*, int*))
                          CSF_AT(void**, componentObj, 0)[168 / 4])(componentObj, nullptr, &a3);
                return reinterpret_cast<uint32_t*>(result);
            }
        }
    }
    return reinterpret_cast<uint32_t*>(result);
}

HOOK(0x0045FCE0, SendMessage29ToResolvedHandle, "auto");
