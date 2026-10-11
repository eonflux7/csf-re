#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneMissionSetup_GetAnimationEntry, 0x00772120, (void* self, void* edx, int a2));

// Returns the animation entry stored in the scene mission setup object reached
// through GetValue8B0554()'s result (+0x1FC4), keyed by this->field_0048.
static int __fastcall C_008344f0_GetSceneSetupAnimationEntry(C_008344f0* self, void* /*edx*/)
{
    GetValue8B0554();
    void* root = reinterpret_cast<void*>(static_cast<uintptr_t>(GetValue8B0554()));
    void* sceneSetup = CSF_AT(void*, root, 0x1FC4);
    return SceneMissionSetup_GetAnimationEntry(sceneSetup, nullptr, self->field_0048);
}

HOOK(0x4A3090, C_008344f0_GetSceneSetupAnimationEntry, "auto");
