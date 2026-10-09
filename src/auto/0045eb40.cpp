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

// CHandleTable::Resolve(this = g_pCurrentScene, handle) -> entry pointer or 0
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// Callee at 0x48E4D0: __fastcall with one stack argument pushed by the caller.
CSF_FUNC(int, __fastcall, SceneObject_ApplyStateAction, 0x0048E4D0, (void* self, void* edx, int arg));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Resolves a handle stored in a nested structure and, when the entry is
// flagged as valid (flag >= 0) and the handle resolves, applies a state
// action to the resolved object. Returns the resolved pointer, or the
// action's result when it was applied.
static C_00833ee8* __stdcall ResolveSceneEntryHandle(int a1, int a2, int a3)
{
    (void)a1;
    (void)a2;

    auto* argRef = reinterpret_cast<uint32_t*>(a3);

    // v3 = *(*(a3) + 4)
    uint32_t entry = *reinterpret_cast<uint32_t*>(argRef[0] + 4);
    // v4 = *(entry + 12)
    int32_t entryFlag = *reinterpret_cast<int32_t*>(entry + 12);

    uint32_t handleAddress = entry + 4;
    C_00833ee8* result = reinterpret_cast<C_00833ee8*>(handleAddress);

    if (entryFlag >= 0)
    {
        result = reinterpret_cast<C_00833ee8*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleAddress)));

        if (result)
        {
            // ecx = [a3 + 4]; edx = [ecx + 4]; value = [edx + 4]
            uint32_t* contextLevel1 = reinterpret_cast<uint32_t*>(argRef[1]);
            uint32_t* contextLevel2 = reinterpret_cast<uint32_t*>(contextLevel1[1]);
            int argValue = static_cast<int>(contextLevel2[1]);

            return reinterpret_cast<C_00833ee8*>(
                SceneObject_ApplyStateAction(result, nullptr, argValue));
        }
    }

    return result;
}

HOOK(0x0045EB40, ResolveSceneEntryHandle, "auto");
