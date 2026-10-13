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
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* , int index));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, S_00521df0_PlaySoundRequest, 0x00523940, (S_00521df0* self, void* , void* request, float volume));

// Plays the objective banner sound request once scene resource 100 is available.
static int HudObjectives_PlayBannerSoundWhenResourceReady()
{
    // Scene loader lives at +0x22A4 inside the global value block.
    int globalBlock = GetGlobalValue_8BC1E0();
    uint8_t* sceneLoaderObj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(globalBlock)) + 0x22A4;

    int resourceId = SceneLoader_GetResourceById(sceneLoaderObj, nullptr, 100);
    if (resourceId == -1)
        return resourceId;

    // Sound request object kept on this frame: 0x38 bytes.
    alignas(8) uint8_t requestBytes[0x38];
    std::memset(requestBytes, 0, sizeof(requestBytes));

    CSF_AT(int, requestBytes, 0x00) = 0;
    CSF_AT(int, requestBytes, 0x04) = 3;
    CSF_AT(int, requestBytes, 0x08) = resourceId;
    CSF_AT(int, requestBytes, 0x0C) = -1;
    CSF_AT(float, requestBytes, 0x10) = -1.0f;
    CSF_AT(int, requestBytes, 0x14) = 0;
    CSF_AT(int, requestBytes, 0x18) = 0;
    CSF_AT(int, requestBytes, 0x1C) = 0;
    CSF_AT(int, requestBytes, 0x20) = 0;
    CSF_AT(void*, requestBytes, 0x24) = CSF_VTBL(0x0082D448);
    CSF_AT(int, requestBytes, 0x28) = -1;
    CSF_AT(int, requestBytes, 0x2C) = -1;
    CSF_AT(int, requestBytes, 0x30) = 0;
    CSF_AT(uint8_t, requestBytes, 0x34) = 0;
    CSF_AT(uint8_t, requestBytes, 0x35) = 1;
    CSF_AT(uint8_t, requestBytes, 0x36) = 1;

    S_00521df0* soundObj = reinterpret_cast<S_00521df0*>(static_cast<uintptr_t>(GetGlobalValue_521BB0()));
    return S_00521df0_PlaySoundRequest(soundObj, nullptr, requestBytes, -12345.0f);
}

HOOK(0x00428350, HudObjectives_PlayBannerSoundWhenResourceReady, "auto");
