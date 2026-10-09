#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));

// Resets the two-dword range at (*(a1+4)+4) to -1, then, if the current scene
// has a menu context, copies its +0x20/+0x24 dwords into that range.
// Returns 0 when there is no context, otherwise the value of context+0x24.
static int __stdcall SceneMenu_ResetAndFillContextRange(int a1, int a2, int a3)
{
    (void)a2;
    (void)a3;

    uint32_t rangeAddr = CSF_AT(uint32_t, reinterpret_cast<void*>(a1), 4) + 4;
    CSF_AT(uint32_t, reinterpret_cast<void*>(rangeAddr), 4) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, reinterpret_cast<void*>(rangeAddr), 8) = 0xFFFFFFFFu;

    uint32_t context = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (context != 0)
    {
        // Re-read a1+4 after the call; the callee may have changed the object.
        uint32_t rangeAddrAfter = CSF_AT(uint32_t, reinterpret_cast<void*>(a1), 4) + 4;
        CSF_AT(uint32_t, reinterpret_cast<void*>(rangeAddrAfter), 4) =
            CSF_AT(uint32_t, reinterpret_cast<void*>(context), 0x20);
        context = CSF_AT(uint32_t, reinterpret_cast<void*>(context), 0x24);
        CSF_AT(uint32_t, reinterpret_cast<void*>(rangeAddrAfter), 8) = context;
    }
    return static_cast<int>(context);
}

HOOK(0x00461BA0, SceneMenu_ResetAndFillContextRange, "auto");
