#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiAttackTasks_FindSceneEntryIndex, 0x00770610, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneEntry, 0x00770690, (void* self, void* edx, int a2));
CSF_FUNC(char, __stdcall, SceneEntry_ComputeSlotStatusByte, 0x004EF080, (void* a1, int a2));

// Looks up the scene entry for sceneKey and returns its slot status byte; 0 when the key is not found.
static char __fastcall ComputeSlotStatusByte(S_004ef780* self, void* /*edx*/, uint32_t sceneKey, int statusArg)
{
    int sceneEntryIndex = AiAttackTasks_FindSceneEntryIndex(self, nullptr, sceneKey);
    if (sceneEntryIndex == -1)
        return 0;

    void* sceneEntry = reinterpret_cast<void*>(SceneLoader_GetSceneEntry(self, nullptr, sceneEntryIndex));
    return SceneEntry_ComputeSlotStatusByte(sceneEntry, statusArg);
}

HOOK(0x004EF150, ComputeSlotStatusByte, "auto");
