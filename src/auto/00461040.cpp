#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, sub_452A80, 0x00452A80, (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, sub_452B30, 0x00452B30, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __cdecl, sub_755810, 0x00755810, (unsigned int a1));

// Dispatches a scene-handle action: resolves the handle's table entry, checks its
// flag, then forwards to sub_452B30 or sub_452A80 depending on the lookup value.
static int __stdcall SceneHandle_DispatchActionOnHandle(int /*a1*/, int /*a2*/, uint32_t* a3)
{
    // tablePtr = *(*a3 + 4); handle key is at tablePtr + 4.
    uint32_t tablePtr = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a3[0]))[1];
    int handleKey = static_cast<int>(tablePtr + 4);
    int result = *reinterpret_cast<int*>(static_cast<uintptr_t>(tablePtr) + 12);

    if (result >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, handleKey);
        if (result != 0)
        {
            uint32_t entryFlagSource = *reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(result) + 0x50);
            int flagResult = sub_755810(entryFlagSource);
            result = flagResult;
            if ((flagResult & 0xFF) != 0)
            {
                // ctrl = a3[1]; v6 = *(ctrl + 4); lookup = *(v6 + 0x20).
                uint32_t ctrl = a3[1];
                uint32_t v6 = *reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(ctrl) + 4);
                int lookupValue = *reinterpret_cast<int*>(static_cast<uintptr_t>(v6) + 0x20);
                if (lookupValue == -1)
                    return sub_452B30(g_pCurrentScene, nullptr, 1, handleKey);
                return sub_452A80(g_pCurrentScene, nullptr, 1, handleKey, lookupValue);
            }
        }
    }
    return result;
}

HOOK(0x00461040, SceneHandle_DispatchActionOnHandle, "auto");
