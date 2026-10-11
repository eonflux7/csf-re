#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);
CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);

CSF_FUNC(int, __fastcall, GetRelatedObjectAndClearFlag, 0x0048E6F0, (void* self, void*));
CSF_FUNC(char, __fastcall, MpSpawnPoints_UpdateRelatedObject, 0x004E1E40, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MpSpawnPoints_SetProviderState, 0x004F5610, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, MpSpawnPoints_SetGameMode, 0x00536600, (void* self, void* edx, uint32_t mode));
CSF_FUNC(void, __fastcall, ClearFlag17E, 0x00536640, (void* self, void*));
CSF_FUNC(int, __cdecl, GetMpGameModeIndex, 0x005515F0, ());
CSF_FUNC(int, __fastcall, MpSpawnPoints_EmitStateMessage, 0x00551680, (void* self, void* edx, int a2, int a3, int a4, int a5));
CSF_FUNC(char, __fastcall, MpSpawnPoints_SetActiveState, 0x00552A20, (void* self, void* edx, char a3));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(char, __cdecl, SceneLoader_SetFileOption, 0x00698600, (char a1));
CSF_FUNC(int, __fastcall, MpSpawnPoints_NotifyRelatedOwner, 0x00740C10, (void* self, void* edx));
CSF_FUNC(double, __cdecl, _floor, 0x007AFD90, (double X));

static void __fastcall C_00844694_UpdateMultiplayerSpawnState(
    C_00844694* self, void* /*edx*/, float elapsed)
{
    void* selection = CSF_AT(
        void*, reinterpret_cast<void*>(g_CharacterSelectState8B0694), 0x4);
    void* spawnState = selection
        ? reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(selection) - 776)
        : nullptr;

    const bool isLocal = selection &&
        reinterpret_cast<bool (__fastcall*)(void*, void*)>(
            CSF_AT(void**, selection, 0)[2])(selection, nullptr);

    if (!spawnState)
        return;

    SceneLoader_SetFileOption(0);
    const uint32_t state = static_cast<uint32_t>(
        CSF_AT(int8_t, spawnState, 0x5C));
    SceneLoader_SetFileOption(1);

    if (self->field_0ca4 != state)
    {
        self->field_0ca4 = state;
        switch (state)
        {
        case 1:
        {
            const int globalState = GetGlobalState_50D190();
            ClearFlag17E(
                CSF_AT(void*, reinterpret_cast<void*>(globalState), 0x18),
                nullptr);
            GetRelatedObjectAndClearFlag(
                reinterpret_cast<void*>(self->field_0c8c), nullptr);
            if (isLocal)
                MpSpawnPoints_EmitStateMessage(self, nullptr, 1651322928, -1, -1, -1);
            break;
        }
        case 2:
        {
            const int globalState = GetGlobalState_50D190();
            ClearFlag17E(
                CSF_AT(void*, reinterpret_cast<void*>(globalState), 0x18),
                nullptr);
            if (isLocal)
                MpSpawnPoints_EmitStateMessage(self, nullptr, 1651322929, -1, -1, -1);
            break;
        }
        case 3:
        {
            void* related = reinterpret_cast<void*>(self->field_0c8c);
            self->field_13b8 = 0;
            self->field_13c4 = 0;
            GetRelatedObjectAndClearFlag(related, nullptr);
            MpSpawnPoints_UpdateRelatedObject(
                CSF_AT(void*, reinterpret_cast<void*>(self->field_0c8c), 0x38),
                nullptr);

            const int globalValue = GetGlobalValue_4FBDE0();
            const int firstElement = GetFirstElement(
                reinterpret_cast<void*>(globalValue), nullptr);
            void* provider = GetCachedObjectProvider(
                reinterpret_cast<void*>(firstElement), nullptr);
            MpSpawnPoints_SetProviderState(provider, nullptr, 0);

            const int globalState = GetGlobalState_50D190();
            void* modeController = CSF_AT(
                void*, reinterpret_cast<void*>(globalState), 0x18);
            const int mode = GetMpGameModeIndex();
            MpSpawnPoints_SetGameMode(modeController, nullptr, mode);
            MpSpawnPoints_SetActiveState(self, nullptr, 1);
            if (isLocal)
                MpSpawnPoints_EmitStateMessage(self, nullptr, 1651322930, -1, -1, -1);
            break;
        }
        }
    }

    switch (self->field_0ca4)
    {
    case 1:
        if (isLocal)
        {
            const uint32_t previousCountdown = self->field_0ca0;
            self->field_13c4 = 1157234688;
            const int64_t countdown = static_cast<int64_t>(
                _floor(CSF_AT(float, spawnState, 0x32C)));
            const uint32_t countdownValue = static_cast<uint32_t>(countdown);
            self->field_0ca0 = countdownValue;
            if (previousCountdown != countdownValue)
            {
                switch (countdownValue)
                {
                case 0:
                    MpSpawnPoints_EmitStateMessage(self, nullptr, 1667313712, -1, -1, -1);
                    break;
                case 1:
                    MpSpawnPoints_EmitStateMessage(self, nullptr, 1667313713, -1, -1, -1);
                    break;
                case 2:
                    MpSpawnPoints_EmitStateMessage(self, nullptr, 1667313714, -1, -1, -1);
                    break;
                case 3:
                    MpSpawnPoints_EmitStateMessage(self, nullptr, 1667313715, -1, -1, -1);
                    break;
                case 4:
                    MpSpawnPoints_EmitStateMessage(self, nullptr, 1667313716, -1, -1, -1);
                    break;
                case 5:
                    MpSpawnPoints_EmitStateMessage(self, nullptr, 1667313717, -1, -1, -1);
                    break;
                case 6:
                    MpSpawnPoints_EmitStateMessage(self, nullptr, 1667313718, -1, -1, -1);
                    break;
                case 7:
                    MpSpawnPoints_EmitStateMessage(self, nullptr, 1667313719, -1, -1, -1);
                    break;
                case 8:
                    MpSpawnPoints_EmitStateMessage(self, nullptr, 1667313720, -1, -1, -1);
                    break;
                case 9:
                    MpSpawnPoints_EmitStateMessage(self, nullptr, 1667313721, -1, -1, -1);
                    break;
                case 10:
                    MpSpawnPoints_EmitStateMessage(self, nullptr, 1667313968, -1, -1, -1);
                    break;
                case 11:
                    MpSpawnPoints_EmitStateMessage(self, nullptr, 1667313969, -1, -1, -1);
                    break;
                case 12:
                    MpSpawnPoints_EmitStateMessage(self, nullptr, 1667313970, -1, -1, -1);
                    break;
                case 13:
                    MpSpawnPoints_EmitStateMessage(self, nullptr, 1667313971, -1, -1, -1);
                    break;
                case 14:
                    MpSpawnPoints_EmitStateMessage(self, nullptr, 1667313972, -1, -1, -1);
                    break;
                case 15:
                    MpSpawnPoints_EmitStateMessage(self, nullptr, 1667313973, -1, -1, -1);
                    break;
                default:
                    return;
                }
            }
        }
        break;

    case 2:
        self->field_13c4 = 1157234688;
        break;

    case 3:
    {
        void* related = reinterpret_cast<void*>(self->field_0c8c);
        const float timer = elapsed + reinterpret_cast<float&>(self->field_13c4);
        reinterpret_cast<float&>(self->field_13c4) = timer;
        if (related && timer > g_MessageBoxAnimationScale && !self->field_13b8)
        {
            const uintptr_t ownerSubobject = CSF_AT(uint32_t, related, 0x18);
            if (ownerSubobject && ownerSubobject != 1712)
            {
                MpSpawnPoints_NotifyRelatedOwner(
                    reinterpret_cast<void*>(ownerSubobject - 1712), nullptr);
                self->field_13b8 = 1;
            }
        }
        break;
    }
    }
}

HOOK(0x005537D0, C_00844694_UpdateMultiplayerSpawnState, "auto");
