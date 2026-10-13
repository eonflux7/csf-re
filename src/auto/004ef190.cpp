#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiAttackTasks_FindSceneEntryIndex, 0x00770610, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneEntry, 0x00770690, (void* self, void* edx, int a2));
CSF_FUNC(char, __stdcall, SceneEntry_MatchesAssociationKey, 0x004EF080, (void* a1, int a2));

// ecx carries the scene object (this) into the lookup calls; the two stack
// arguments are the key (a1) and an offset value (a2) consumed by the caller.
static char __fastcall FxParticleManager_IsSceneEntryAssociated(S_004ef780* self, void* /*edx*/, uint32_t a1, int a2)
{
    int sceneEntryIndex = AiAttackTasks_FindSceneEntryIndex(self, nullptr, a1);
    if (sceneEntryIndex == -1)
        return 0;

    void* sceneEntry = reinterpret_cast<void*>(static_cast<uintptr_t>(
        SceneLoader_GetSceneEntry(self, nullptr, sceneEntryIndex)));
    return SceneEntry_MatchesAssociationKey(sceneEntry, a2 + 0x1c);
}

HOOK(0x004EF190, FxParticleManager_IsSceneEntryAssociated, "auto");
