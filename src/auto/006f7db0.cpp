#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(int, g_Value8BBB28, 0x008BBB28);

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, GfxScene_ReleaseSceneStateSubtree, 0x006F7C20, (void* self, void* edx));

static uint32_t* GfxScene_ResetGlobalSceneState()
{
    // Release the previously stored scene-state block, if any.
    if (g_Value8BBB28 != 0)
    {
        int previousState = g_Value8BBB28;
        GfxScene_ReleaseSceneStateSubtree(
            reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(previousState))),
            nullptr);
        CoreMemory_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(previousState))));
        g_Value8BBB28 = 0;
    }

    // Allocate a fresh 16-byte scene-state block and initialise it.
    uint32_t allocated = static_cast<uint32_t>(CoreMemory_AllocUntagged(16));
    uint32_t* result = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(allocated));
    if (result != nullptr)
    {
        result[0] = 0;
        result[1] = 0;
        result[2] = 0;
        result[3] = 10;
        g_Value8BBB28 = static_cast<int>(allocated);
    }
    else
    {
        g_Value8BBB28 = 0;
    }
    return result;
}

HOOK(0x006F7DB0, GfxScene_ResetGlobalSceneState, "auto");
