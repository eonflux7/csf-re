#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(unsigned int, __fastcall, RegisterSceneObjects, 0x00475800, (void* self, void* edx, int a2));

// Original: push ecx; call GetGlobalObjectField184; mov ecx, eax; call RegisterSceneObjects; ret 0
// `this` is pushed as the stack argument RegisterSceneObjects pops (ret 4).
static int __fastcall SceneLoader_PrepareSceneData(S_0044d360* self, void* /*edx*/)
{
    void* globalObjectField184 = reinterpret_cast<void*>(GetGlobalObjectField184());
    return static_cast<int>(RegisterSceneObjects(globalObjectField184, nullptr, reinterpret_cast<int>(self)));
}

HOOK(0x0044D360, SceneLoader_PrepareSceneData, "auto");
