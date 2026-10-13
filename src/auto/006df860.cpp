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

CSF_FUNC(S_0041e5f0*, __cdecl, ParticleEmitters_GetSharedInstance, 0x006E2760, ());
CSF_FUNC(int, __fastcall, C_00863180_ReleaseEmitterObjects, 0x007028C0, (C_00863180* self, void*));
CSF_FUNC(char, __fastcall, AtomicEmitter_InitAndApplyRegion, 0x00702840, (void* self, void* , int arg0));
CSF_FUNC(int, __fastcall, PTank_ResetState, 0x006D9A70, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(C_008656d0*, __fastcall, SceneLoader_CreateIndexedGeometry, 0x006B1DE0, (C_00860130* self, void* , int a2, int a3));

// Prepares the scene's object registry: releases emitter objects, re-applies
// the region, optionally resets the tank state, optionally creates the indexed
// geometry, then marks the registry as prepared.
// __thiscall (ecx = this), ret 0.
static char __fastcall GfxScene_PrepareObjectRegistry(void* self, void* /*edx*/)
{
    ParticleEmitters_GetSharedInstance();
    C_00863180_ReleaseEmitterObjects(CSF_AT(C_00863180*, self, 0x44), nullptr);
    AtomicEmitter_InitAndApplyRegion(CSF_AT(void*, self, 0x44), nullptr, CSF_AT(int, self, 0x08));

    if (CSF_AT(int, self, 0x48) != 0)
    {
        PTank_ResetState(reinterpret_cast<void*>(CSF_AT(int, self, 0x48)), nullptr);
    }

    // Return value is the low byte of the flag when the branch is skipped (0),
    // otherwise the low byte of the created geometry pointer.
    uint8_t resultByte = CSF_AT(uint8_t, self, 0x58);
    if (resultByte != 0)
    {
        C_00860130* globalInt = reinterpret_cast<C_00860130*>(GetGlobalInt());
        C_008656d0* indexedGeometry = SceneLoader_CreateIndexedGeometry(globalInt, nullptr, 1, 1);
        CSF_AT(C_008656d0*, self, 0x5C) = indexedGeometry;
        resultByte = static_cast<uint8_t>(reinterpret_cast<uintptr_t>(indexedGeometry));
    }

    CSF_AT(uint8_t, self, 0x10) = 1;
    return static_cast<char>(resultByte);
}

HOOK(0x006DF860, GfxScene_PrepareObjectRegistry, "auto");
