#include <cstdint>
#include "globals.h"
#include "hook.h"

static int __fastcall C_00833908_SetSecondarySceneObject(void* self, void* /*edx*/, int sceneObject)
{
    CSF_AT(int, self, 0x22C) = sceneObject;
    return sceneObject;
}

HOOK(0x494930, C_00833908_SetSecondarySceneObject, "auto");
