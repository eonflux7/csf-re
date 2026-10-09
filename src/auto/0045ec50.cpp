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

// Arguments block passed by address to the dispatched virtual method (v6/v7 in the original).
struct SceneDispatchArgs {
    int32_t kind;
    uint8_t flag;
};

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// Resolves a handle taken from the object's sub-record, and if the resolved scene
// entry has a dispatch target, invokes its virtual slot 42 (offset 0xA8) with kind=3, flag=1.
static uint32_t* __stdcall SceneHandle_ResolveAndDispatch(int a1, int a2, int a3)
{
    (void)a1;
    (void)a2;

    uint32_t* holder = reinterpret_cast<uint32_t*>(a3);
    uint32_t* obj = reinterpret_cast<uint32_t*>(*holder);
    uint32_t* subRecord = reinterpret_cast<uint32_t*>(obj[1]);

    int32_t subRecordFlags = static_cast<int32_t>(subRecord[3]);
    uint32_t* result = reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(subRecord) + 4);

    if (subRecordFlags >= 0)
    {
        int handle = static_cast<int>(reinterpret_cast<uintptr_t>(result));
        result = reinterpret_cast<uint32_t*>(
            static_cast<uintptr_t>(CHandleTable_Resolve(g_pCurrentScene, nullptr, handle)));

        if (result)
        {
            // Dispatch target stored at +0x34 of the resolved entry.
            result = CSF_AT(uint32_t*, result, 0x34);
            if (result)
            {
                SceneDispatchArgs args;
                args.kind = 3;
                args.flag = 1;

                using DispatchFn = void* (__fastcall*)(void* self, void* edx, SceneDispatchArgs* arguments);
                DispatchFn dispatch = reinterpret_cast<DispatchFn>(CSF_AT(void**, result, 0)[42]);
                result = reinterpret_cast<uint32_t*>(dispatch(result, nullptr, &args));
            }
        }
    }

    return result;
}

HOOK(0x0045EC50, SceneHandle_ResolveAndDispatch, "auto");
