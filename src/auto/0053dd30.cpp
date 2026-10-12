#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AudioSoundManager_InitializeRequest, 0x0040FE30, (S_00526d40* self, void* , int a2, int a3, int a4, int a5, int a6));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, UmsVirtualProcessor_GetDefaultDestination, 0x004FE4A0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, CSoundManager_ClearSounds, 0x005233D0, (CSoundManager* self, void*));
CSF_FUNC(int, __fastcall, S_00521df0_PlaySoundRequest, 0x00523940, (S_00521df0* self, void* , void* request, float volume));
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* , int index));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall MenuMissionInfo_PlayMissionSound(void* self, void* /*edx*/)
{
    const int stateAddress = CSF_AT(int, self, 0x14);
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(stateAddress), 0x4))
        return stateAddress;

    auto* soundManager =
        reinterpret_cast<CSoundManager*>(GetGlobalValue_521BB0());
    CSoundManager_ClearSounds(soundManager, nullptr);

    void* processor = reinterpret_cast<void*>(GetGlobalValue_8B0568());
    void* destination =
        UmsVirtualProcessor_GetDefaultDestination(processor, nullptr);
    const uint32_t missionType = CSF_AT(uint32_t, destination, 0x1C);

    int resourceIndex;
    if (missionType == 0)
        resourceIndex = 67;
    else if (missionType == 1)
        resourceIndex = 68;
    else
        return static_cast<int>(missionType - 1);

    void* sceneLoader = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    const int resource = SceneLoader_GetResourceById(
        static_cast<uint8_t*>(sceneLoader) + 0x22A4, nullptr, resourceIndex);

    alignas(8) uint8_t requestBytes[0x38];
    S_00526d40& request = *reinterpret_cast<S_00526d40*>(requestBytes);
    AudioSoundManager_InitializeRequest(&request, nullptr, 3, resource, 4, 0, -1);

    auto* playbackManager =
        reinterpret_cast<S_00521df0*>(GetGlobalValue_521BB0());
    return S_00521df0_PlaySoundRequest(
        playbackManager, nullptr, &request, -1.0f);
}

HOOK(0x0053DD30, MenuMissionInfo_PlayMissionSound, "auto");
