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

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, sub_4686A0, 0x004686A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_4686F0, 0x004686F0, (void* self, void* edx, int a2, int a3));

// Resolves the handle stored in the first argument object, then looks it up in the
// current scene's table (scene+0xBC). On success marks the found entry (+0x80) and
// returns it; otherwise falls back to the secondary lookup. Returns the entry handle
// unchanged when the handle is negative.
static int __stdcall Scene_ResolveHandleOrFallback(int a1, int a2, uint32_t* a3)
{
    (void)a1;
    (void)a2;

    uint32_t firstObj = a3[0];
    uint32_t entry = CSF_AT(uint32_t, reinterpret_cast<void*>(firstObj), 4);
    int32_t handleValue = CSF_AT(int32_t, reinterpret_cast<void*>(entry), 12);
    int result = (int)(entry + 4);

    if (handleValue >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (result)
        {
            uint32_t mid = CSF_AT(uint32_t, reinterpret_cast<void*>(a3[1]), 4);
            uint32_t deepArg = CSF_AT(uint32_t, reinterpret_cast<void*>(mid), 4);
            void* sceneTable = CSF_AT(void*, g_pCurrentScene, 0xBC);

            result = sub_4686F0(sceneTable, nullptr, result, (int)deepArg);
            if (result)
            {
                CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0x80) = 1;
            }
            else
            {
                // Code-address annotation: function value, not a vtable.
                [[maybe_unused]] auto sceneFallbackVtbl = reinterpret_cast<void**>(CSF_ADDR_FN(0x004686A0));
                uint32_t midAgain = CSF_AT(uint32_t, reinterpret_cast<void*>(a3[1]), 4);
                uint32_t deepArgAgain = CSF_AT(uint32_t, reinterpret_cast<void*>(midAgain), 4);
                void* sceneTableAgain = CSF_AT(void*, g_pCurrentScene, 0xBC);
                return sub_4686A0(sceneTableAgain, nullptr, (int)deepArgAgain);
            }
        }
    }
    return result;
}

HOOK(0x0045EE70, Scene_ResolveHandleOrFallback, "auto");
