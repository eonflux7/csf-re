#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(int, __fastcall, ToggleTimer_IsActive, 0x004A1210, (void* self, void* edx));
CSF_FUNC(char, __fastcall, ToggleTimer_SetState, 0x004A1260, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(void, __fastcall, GlobalValue4FBDE0_SetToggleMode, 0x004FC210, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, CSoundManager_SetVolumeModeFlag, 0x00522B40, (CSoundManager* self, void* , uint32_t flagArg));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(int, __fastcall, C_00833908_SetSceneResourceIfSceneMatches, 0x00525A50, (C_00833908* self, void* , int sceneId, int resourceId));

// The original function's result is whatever the last call left in al; on the
// disable path that is the result of CSoundManager_SetVolumeModeFlag, so that
// call is made through a char-returning view of the same binding.
typedef char(__fastcall* SetVolumeModeFlagWithResult)(CSoundManager*, void*, uint32_t);

static char __fastcall C_00834050_SetCharacterToggleState(C_00834050* self, void* /*edx*/, char enable)
{
    // Current state: flag 0x40 set, owner's sub-object in mode 2, and not blocked.
    char currentState = 0;
    if (CSF_AT(uint8_t, self, 0x2E0) & 0x40)
    {
        void* owner = reinterpret_cast<void*>(self->field_0044);
        C_008319c0* ownerSubObject =
            ((C_008319c0*(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[0xD8 / 4])(owner, nullptr);
        if (ownerSubObject != nullptr
            && C_008319c0_GetSubObjectField74(ownerSubObject, nullptr) == 2
            && CSF_AT(char, self, 0x360) == 0)
        {
            currentState = 1;
        }
    }

    if (currentState == enable)
        return currentState;

    void* toggleTimer = (uint8_t*)self + 0x344;

    if (enable)
    {
        if (!(char)ToggleTimer_IsActive(toggleTimer, nullptr))
        {
            uint32_t savedState[2] = { 0, 0 };
            ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, self, 0)[0x38 / 4])(self, nullptr, savedState);
            self->field_01b0 = savedState[1];
            self->field_01ac = 0;

            void* field16c = reinterpret_cast<void*>(self->vGetField016c());
            void* target = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, field16c, 0)[0x18 / 4])(field16c, nullptr);
            ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, target, 0)[0x34 / 4])(target, nullptr, savedState[0]);
        }

        ToggleTimer_SetState(toggleTimer, nullptr, 0);
        C_00833908_SetSceneResourceIfSceneMatches(
            reinterpret_cast<C_00833908*>(Get_008b0678()), nullptr, (int)(uintptr_t)self, 56);

        if (CSF_AT(char, GetGlobalState_50D190(), 0xD0) == 0)
        {
            GlobalValue4FBDE0_SetToggleMode(reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr, 1);
            CSoundManager_SetVolumeModeFlag(reinterpret_cast<CSoundManager*>(GetGlobalValue_521BB0()), nullptr, 1);
        }

        int globalState = GetGlobalState_50D190();
        void* listener = CSF_AT(void*, globalState, 0x74);
        if (listener == nullptr)
            return (char)globalState;

        char notificationsSuppressed = CSF_AT(char, self, 0x228);
        if (notificationsSuppressed != 0)
            return notificationsSuppressed;

        return ((char(__fastcall*)(void*, void*, int, int))CSF_AT(void**, listener, 0)[0x4 / 4])(listener, nullptr, 8, 0);
    }

    ToggleTimer_SetState(toggleTimer, nullptr, 1);
    C_00833908_SetSceneResourceIfSceneMatches(
        reinterpret_cast<C_00833908*>(Get_008b0678()), nullptr, (int)(uintptr_t)self, 58);

    void* listener = CSF_AT(void*, GetGlobalState_50D190(), 0x74);
    if (listener != nullptr && CSF_AT(char, self, 0x228) == 0)
    {
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, listener, 0)[0x8 / 4])(listener, nullptr);
    }

    int globalState = GetGlobalState_50D190();
    if (CSF_AT(char, globalState, 0xD0) != 0)
        return (char)globalState;

    GlobalValue4FBDE0_SetToggleMode(reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr, 0);
    return ((SetVolumeModeFlagWithResult)CSoundManager_SetVolumeModeFlag)(
        reinterpret_cast<CSoundManager*>(GetGlobalValue_521BB0()), nullptr, 0);
}

HOOK(0x004975E0, C_00834050_SetCharacterToggleState, "auto");
