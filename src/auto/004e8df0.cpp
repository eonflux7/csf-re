#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Scene object that receives registered objects (bound by address).
CSF_FUNC(int, __fastcall, CScene_AddObject, 0x00453420, (CScene* self, void* , C_00867260* object));

// Current scene pointer global.
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Registers an object with the current scene. The original tail-jumps to
// CScene_AddObject with ecx = g_pCurrentScene and the stack argument forwarded;
// the callee pops the 4-byte object argument, so this stdcall wrapper pops 4.
static int __stdcall SceneLoader_RegisterObject(C_00867260* object)
{
    return CScene_AddObject(reinterpret_cast<CScene*>(g_pCurrentScene), nullptr, object);
}

HOOK(0x004E8DF0, SceneLoader_RegisterObject, "auto");
