#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Scene table: pointer to an array of dwords at 0x899730, element count at 0x899738.
CSF_GLOBAL(uint32_t, g_sceneTableBase, 0x899730);
CSF_GLOBAL(uint32_t, g_numScenes, 0x899738);

// Returns the dword stored in the scene table at index sceneIndex, or 0 when the index
// is outside the table. The comparison is unsigned (jae in the original).
static int __cdecl SceneLoader_IsEntryAvailable(uint32_t sceneIndex) {
    if (sceneIndex >= g_numScenes) {
        return 0;
    }
    const uint32_t* sceneTable = reinterpret_cast<const uint32_t*>(static_cast<uintptr_t>(g_sceneTableBase));
    return static_cast<int>(sceneTable[sceneIndex]);
}

HOOK(0x450610, SceneLoader_IsEntryAvailable, "auto");
