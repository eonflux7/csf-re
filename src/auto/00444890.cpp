#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

// Bound by address: a __fastcall callee (ecx = this) taking one stack argument.
CSF_FUNC(char, __fastcall, S_0046b530_LoadGameSceneScript, 0x0046BC40, (S_0046b530* self, void* /*edx*/, int path));

// The original is a tail jump: it reads [ecx+4] and forwards ecx and the stack
// argument unchanged to LoadGameSceneScript. Declared __fastcall so ecx is visible
// as the first parameter; the single stack argument is popped with ret 4, matching
// the original stdcall stack contract. The callee's byte result passes through in al.
static char __fastcall SceneLoader_LoadCompiledSceneScript(void* self, void* /*edx*/, S_0046b530* a1)
{
    S_0046b530* sceneScript = CSF_AT(S_0046b530*, self, 0x4);
    return S_0046b530_LoadGameSceneScript(sceneScript, nullptr, reinterpret_cast<int>(a1));
}

HOOK(0x00444890, SceneLoader_LoadCompiledSceneScript, "auto");
