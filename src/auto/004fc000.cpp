#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, CSoundManager_SetMasterAndEffectsVolumes, 0x005228C0, (CSoundManager* self, void* /*edx*/, float masterVolume, float effectsVolume));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(char, __fastcall, SoundManager_PlaySoundInSceneMenuContext, 0x00525AB0, (int* self, void* /*edx*/, int soundArg));

// Reads the two volume settings from the global config block, applies them to the
// global sound manager, then plays scene menu sound 59.
static int __cdecl CommAnims_ApplyVolumesAndPlayMenuSound()
{
    uint8_t* settingsA = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
    float effectsVolume = CSF_AT(float, settingsA, 0x18B4);

    uint8_t* settingsB = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
    float masterVolume = CSF_AT(float, settingsB, 0x18B8);

    CSoundManager* soundManager = reinterpret_cast<CSoundManager*>(GetGlobalValue_521BB0());
    CSoundManager_SetMasterAndEffectsVolumes(soundManager, nullptr, masterVolume, effectsVolume);

    int* sceneMenuContext = reinterpret_cast<int*>(Get_008b0678());
    return SoundManager_PlaySoundInSceneMenuContext(sceneMenuContext, nullptr, 59);
}

HOOK(0x004FC000, CommAnims_ApplyVolumesAndPlayMenuSound, "auto");
