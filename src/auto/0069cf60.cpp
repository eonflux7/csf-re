#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(int, g_SceneLoaderBlock, 0x008BAFD8);

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static int SceneLoader_ReleaseGlobalBlock()
{
    int result = 0;

    if (g_SceneLoaderBlock)
    {
        SceneLoader_NoOp();

        if (g_SceneLoaderBlock)
        {
            int blockHandle = g_SceneLoaderBlock;
            void* blockPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(blockHandle));

            SceneLoader_NoOp();
            result = CoreMemory_Free(blockPtr);
            g_SceneLoaderBlock = 0;
        }
    }

    return result;
}

HOOK(0x0069CF60, SceneLoader_ReleaseGlobalBlock, "auto");
