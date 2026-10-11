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
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, S_00521df0_PlaySoundRequest, 0x00523940, (S_00521df0* self, void* edx, void* request, float volume));

// Looks up a scene resource by id and, if it exists, plays it through the global sound player.
// Returns -1 (the lookup result) when the resource is missing.
static int __stdcall PlaySceneResourceSound(int resourceId)
{
    uint8_t* globalBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));
    int resourceHandle = SceneLoader_GetResourceById(globalBase + 0x22A4, nullptr, resourceId);
    if (resourceHandle == -1)
        return resourceHandle;

    // Sound request record (0x38 bytes) built on the stack.
    uint8_t soundRequest[0x38] = {};
    CSF_AT(int, soundRequest, 0x00) = 4;
    CSF_AT(int, soundRequest, 0x04) = 4;
    CSF_AT(int, soundRequest, 0x08) = resourceHandle;
    CSF_AT(int, soundRequest, 0x0C) = -1;
    CSF_AT(int, soundRequest, 0x10) = 0;
    CSF_AT(int, soundRequest, 0x14) = 0;
    CSF_AT(int, soundRequest, 0x18) = 0;
    CSF_AT(int, soundRequest, 0x1C) = 0;
    CSF_AT(int, soundRequest, 0x20) = 0;
    CSF_AT(void*, soundRequest, 0x24) = CSF_VTBL(0x0082D448);
    CSF_AT(int, soundRequest, 0x28) = -1;
    CSF_AT(int, soundRequest, 0x2C) = -1;
    CSF_AT(int, soundRequest, 0x30) = 0;
    CSF_AT(uint8_t, soundRequest, 0x34) = 0;
    CSF_AT(uint8_t, soundRequest, 0x35) = 1;
    CSF_AT(uint8_t, soundRequest, 0x36) = 1;

    S_00521df0* soundPlayer = reinterpret_cast<S_00521df0*>(static_cast<uintptr_t>(GetGlobalValue_521BB0()));
    return S_00521df0_PlaySoundRequest(soundPlayer, nullptr, soundRequest, -1.0f);
}

HOOK(0x0040FEE0, PlaySceneResourceSound, "auto");
