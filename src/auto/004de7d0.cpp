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

// Callee at 0x4DE0A0: removes a scene object from the loader's list. Returns char, pops 4 bytes.
CSF_FUNC(char, __fastcall, SceneObjectList_RemoveEntry, 0x004DE0A0, (void* self, void* edx, void* a2));

// Tail-jumps into SceneObjectList_RemoveEntry with the scene list taken from this+0x18.
static char __fastcall SceneLoader_UnregisterSceneObject(C_008342e8* self, void* /*edx*/, C_00867260* a2)
{
    void* sceneList = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x18));
    return SceneObjectList_RemoveEntry(sceneList, nullptr, a2);
}

HOOK(0x004DE7D0, SceneLoader_UnregisterSceneObject, "auto");
