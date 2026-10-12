#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __stdcall, MenuManager_SetGameplayControllerState, 0x0042DF80, (int a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, MenuManager_SetInnerState, 0x0050E930, (int* self, void* , char a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, SceneLoader_UpdateLoadingState, 0x00521F40, (void* self, void* edx, int state, int enabled));
CSF_FUNC(bool, __fastcall, AudioSoundManager_QueryActiveSound, 0x00521F60, (void* self, void* edx));
CSF_FUNC(char, __fastcall, AudioSoundManager_ProcessPendingStops, 0x005227B0, (void* self, void*));
CSF_FUNC(void*, __cdecl, CoreFiles_SetFileOption, 0x00698600, (char locked));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static int __fastcall C_00830578_SetStateAndResetFlags(
    C_00830578* self, void* /*edx*/, char state)
{
    MenuManager_SetInnerState(
        reinterpret_cast<int*>(GetGlobalState_50D190()), nullptr, state);

    if (AudioSoundManager_QueryActiveSound(
            reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr))
    {
        SceneLoader_UpdateLoadingState(
            reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr, 0, 1);
        AudioSoundManager_ProcessPendingStops(
            reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr);
    }

    CoreFiles_SetFileOption(
        CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_8B0568()), 0xB5) == 0);

    CSF_AT(uint8_t, self, 0x108) = 0;
    CSF_AT(uint8_t, self, 0x109) = 0;
    CSF_AT(uint8_t, self, 0x17F) = 0;

    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalInt()), 0x4))
    {
        GetGlobal8afc78();
        MenuManager_SetGameplayControllerState(1);
    }

    void* controller = CSF_AT(
        void*, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x10);
    return reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, controller, 0)[2])(controller, nullptr);
}

HOOK(0x00536650, C_00830578_SetStateAndResetFlags, "auto");
