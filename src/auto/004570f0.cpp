#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Unnamed callee: clears the per-scene state held by the loader (called only when field_0008 is set).
CSF_FUNC(int, __fastcall, SceneLoader_ReleaseSceneState, 0x00456E10, (void* self, void* edx));

// Registers the loader object with the multiplayer spawn-point subsystem.
CSF_FUNC(uint32_t, __cdecl, MpSpawnPoints_RegisterObject, 0x00475850, (int a1));

// Called when the scene changes: releases the previous scene state if one is active,
// resets the secondary counter, re-registers this loader, and marks the state as active.
static char __fastcall SceneLoader_NotifySceneChange(C_008312e0* self, void* /*edx*/)
{
    if (self->field_0008)
    {
        SceneLoader_ReleaseSceneState(self, nullptr);
        self->field_0008 = 0;
    }
    self->field_000c = 0;
    MpSpawnPoints_RegisterObject((int)(uintptr_t)self);
    self->field_0008 = 1;
    return 1;
}

HOOK(0x4570F0, SceneLoader_NotifySceneChange, "auto");
