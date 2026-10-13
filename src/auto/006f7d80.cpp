#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, GfxMaterials_DestroySceneCache, 0x006F7C20, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_GLOBAL(int32_t, g_Value8BBB28, 0x008BBB28);

// Releases the global scene cache block if one exists: runs its teardown
// routine, frees the block through the game heap, and clears the global pointer.
static int __cdecl GfxMaterials_ResetSceneCaches()
{
    int result = 0;
    if (g_Value8BBB28 != 0)
    {
        void* cacheBlock = reinterpret_cast<void*>(static_cast<uintptr_t>(g_Value8BBB28));
        GfxMaterials_DestroySceneCache(cacheBlock, nullptr);
        result = CoreMemory_Free(cacheBlock);
        g_Value8BBB28 = 0;
    }
    return result;
}

HOOK(0x6F7D80, GfxMaterials_ResetSceneCaches, "auto");
