#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Bound per the packet's "bind as" line.
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// sub_452C70 reads ecx (this) on entry, so it is bound __fastcall with the object in ecx.
// It takes two stack args (ret 8). Its edi/esi register inputs cannot be set from C++ here.
CSF_FUNC(int, __fastcall, sub_452C70, 0x00452C70, (void* self, void* edx, int a2, int a5));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __stdcall SceneHandle_ResolveAndDispatch(int a1, int a2, uint32_t* a3)
{
    // v3 = *(*a3 + 4)
    uint8_t* firstObj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(*a3));
    uint8_t* handleObj = reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(CSF_AT(uint32_t, firstObj, 4)));

    // result = *(v3 + 12), signed compare below
    int result = static_cast<int>(CSF_AT(uint32_t, handleObj, 0x0C));

    // v5 = v3 + 4
    uint8_t* handleSlot = handleObj + 4;

    if (result >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                      static_cast<int>(reinterpret_cast<uintptr_t>(handleSlot)));
        if (result)
        {
            // value = *(*(*(a3[1] + 4) + 4) + 4), reloaded after the Resolve call
            uint8_t* ctxObj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a3[1]));
            uint8_t* innerObj = reinterpret_cast<uint8_t*>(
                static_cast<uintptr_t>(CSF_AT(uint32_t, ctxObj, 4)));
            uint8_t* leafObj = reinterpret_cast<uint8_t*>(
                static_cast<uintptr_t>(CSF_AT(uint32_t, innerObj, 4)));
            int payload = static_cast<int>(CSF_AT(uint32_t, leafObj, 4));

            // ecx = g_pCurrentScene at the call site; stack: slot pointer, then payload.
            return sub_452C70(g_pCurrentScene, nullptr,
                              static_cast<int>(reinterpret_cast<uintptr_t>(handleSlot)),
                              payload);
        }
    }
    return result;
}

HOOK(0x0045E770, SceneHandle_ResolveAndDispatch, "auto");
