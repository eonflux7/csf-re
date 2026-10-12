#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __fastcall, HudWidget_ForwardUpdateToSubobject, 0x0042DFB0, (void* self, void*));
CSF_FUNC(int, __fastcall, MissionScript_ResetPendingBlock, 0x00468830, (void* self, void* , char flag));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_ResetPending, 0x0050E1E0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, AudioSound_ForwardToGlobalObjectSlot29, 0x00521D90, (void* self, void* edx, int flag));
CSF_FUNC(char, __fastcall, AudioSoundManager_ProcessPendingStops, 0x005227B0, (void* self, void*));

static char __fastcall S_00537cd0_UpdateAndResetScene(
    S_00537cd0* self, void* /*edx*/, int /*unused*/)
{
    if (CSF_AT(char, CSF_AT(void*, g_pCurrentScene, 0xC0), 9))
    {
        reinterpret_cast<void (__fastcall*)(S_00537cd0*, void*, float)>(
            CSF_AT(void**, self, 0)[0x7C / 4])(self, nullptr, 0.1f);
    }

    const char result =
        CSF_AT(char, CSF_AT(void*, g_pCurrentScene, 0xC0), 9);
    if (result)
        return result;

    CSF_AT(int, self, 0x130) = 2;
    MissionScript_ResetPendingBlock(
        CSF_AT(void*, g_pCurrentScene, 0xBC), nullptr, 0);

    void* soundObject = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    AudioSound_ForwardToGlobalObjectSlot29(soundObject, nullptr, 1);

    soundObject = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    AudioSoundManager_ProcessPendingStops(soundObject, nullptr);

    void* hudWidget = reinterpret_cast<void*>(GetGlobal8afc78());
    HudWidget_ForwardUpdateToSubobject(hudWidget, nullptr);

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* stateObject = CSF_AT(void*, state, 0x10);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, stateObject, 0)[8 / 4])(stateObject, nullptr);

    state = reinterpret_cast<void*>(GetGlobalState_50D190());
    GameBddLoader_ResetPending(state, nullptr);

    CSF_AT(int, reinterpret_cast<void*>(GetGlobalState_50D190()), 4) = 0;

    return reinterpret_cast<char (__fastcall*)(S_00537cd0*, void*)>(
        CSF_AT(void**, self, 0)[0x98 / 4])(self, nullptr);
}

HOOK(0x005360B0, S_00537cd0_UpdateAndResetScene, "auto");
