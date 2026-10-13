#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

CSF_GLOBAL(void*, g_Im3dSharedBlock, 0x008BB894);

// Frees the shared immediate-mode 3D block if it is allocated.
// Returns the Mem_Free result when a block was released, otherwise 0.
static int Im3d_ReleaseSharedResources() {
    int result = 0;
    if (g_Im3dSharedBlock) {
        SceneLoader_NoOp();
        if (g_Im3dSharedBlock) {
            result = Mem_Free(g_Im3dSharedBlock);
            g_Im3dSharedBlock = nullptr;
        }
    }
    return result;
}

HOOK(0x6E2F50, Im3d_ReleaseSharedResources, "auto");
