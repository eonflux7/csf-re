#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of scene entry `index` in the table whose base is stored at
// this+0x08. Each entry is 0x68 (104) bytes.
static int __fastcall SceneLoader_GetSceneEntry(S_004ef780* self, void* /*edx*/, int index)
{
    int tableBase = static_cast<int>(self->field_0008);
    return tableBase + index * 0x68;
}

HOOK(0x00770690, SceneLoader_GetSceneEntry, "auto");
