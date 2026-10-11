#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __cdecl, SceneLoader_SetObjectPlacement, 0x006C4EB0, (int* a1, int a2));

// Applies a placement to the object when a placement handle is supplied.
// Returns the object (a1) when the placement was applied, otherwise 0.
static int __cdecl GfxSkin_ApplyPlacementAndReturnObject(int objectHandle, int placementHandle) {
    if (placementHandle == 0) {
        return 0;
    }
    SceneLoader_SetObjectPlacement(reinterpret_cast<int*>(objectHandle), placementHandle);
    return objectHandle;
}

HOOK(0x006C4F30, GfxSkin_ApplyPlacementAndReturnObject, "auto");
