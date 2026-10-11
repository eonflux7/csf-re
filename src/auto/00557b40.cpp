#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);
CSF_GLOBAL(uint32_t, g_MenuMPCharacterSelect_LastState, 0x008B0698);

CSF_FUNC(int, __cdecl, GetValue8B055C, 0x004F9660, ());
CSF_FUNC(int, __fastcall, MpSpawnPoints_ResetSession, 0x004F9990, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void, __fastcall, GlobalMenu_SetSelectedScenarioName, 0x00501990, (S_004ff0f0* self, void* , char* String1, char a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(uint8_t*, __fastcall, MpSpawnPoints_SetScenarioName, 0x00550B50, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, S_00552b80_ResetSelection, 0x005514E0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t, __cdecl, SpawnPoints_ProcessCharacterSelectState, 0x00551C80, ());
CSF_FUNC(char, __fastcall, S_00552b80_SetSelectionMode, 0x00552A20, (void* self, void* edx, char a3));
CSF_FUNC(int, __fastcall, S_00552b80_GetStatsInterface, 0x00552B80, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, S_00552b80_RefreshSelection, 0x00553730, (void* self, void* edx));
CSF_FUNC(void**, __cdecl, MpSpawnPoints_RefreshScenario, 0x00555270, ());
CSF_FUNC(uint32_t, __fastcall, S_00552b80_ClearPreviousSelection, 0x00557A20, (int** self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(char*, __fastcall, MpSpawnPoints_GetSelectionName, 0x006B29A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MpSpawnPoints_ApplyCharacterSelection, 0x0073C900, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_ResetCharacterMenu, 0x0073F340, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_AttachStatsInterface, 0x0073F730, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, MpSpawnPoints_ResetStats, 0x00740C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_SetStatsMode, 0x00740F40, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, MpSpawnPoints_RefreshStats, 0x00741660, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_ResetSelectedCharacter, 0x00748740, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_GetSelectionId, 0x00749920, (int a1, int a2));
CSF_FUNC(int, __cdecl, GetValue_8BC170, 0x0074A680, ());

static char __fastcall S_00552b80_ProcessSelectionChange(
    S_00552b80* self, void* /*edx*/)
{
    // The packet labels this direct-call address as a vtable reference.
    (void)CSF_VTBL(0x00552B80);

    if (!g_CharacterSelectState8B0694)
        return 0;

    uint32_t selectedAddress = CSF_AT(
        uint32_t, reinterpret_cast<void*>(g_CharacterSelectState8B0694), 4);
    if (!selectedAddress)
        return 0;

    uint32_t ownerAddress = selectedAddress - 776;
    if (!ownerAddress)
        return 0;

    void* selectedOwner = reinterpret_cast<void*>(ownerAddress);
    if (!g_MenuMPCharacterSelect_LastState)
    {
        g_MenuMPCharacterSelect_LastState =
            CSF_AT(uint32_t, selectedOwner, 0x2B8);
        return 0;
    }

    if (CSF_AT(uint32_t, selectedOwner, 0x2B8) ==
        g_MenuMPCharacterSelect_LastState)
        return 0;

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetValue8B055C()), 0x10))
    {
        MpSpawnPoints_ResetSession(
            reinterpret_cast<void*>(GetValue8B055C()), nullptr);
    }

    S_00552b80_ClearPreviousSelection(reinterpret_cast<int**>(self), nullptr);
    g_MenuMPCharacterSelect_LastState =
        CSF_AT(uint32_t, selectedOwner, 0x2B8);

    char* selectionName = MpSpawnPoints_GetSelectionName(selectedOwner, nullptr);
    MpSpawnPoints_SetScenarioName(
        reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B06A0)),
        nullptr, reinterpret_cast<int>(selectionName));

    SpawnPoints_ProcessCharacterSelectState();

    uint32_t selectionView = self->field_0c8c;
    if (selectionView)
    {
        auto* view = reinterpret_cast<C_00833908*>(selectionView);
        void* control = reinterpret_cast<void*>(view->field_0038);
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, control, 0)[0xA4 / 4])(control, nullptr, 0);
    }

    S_00552b80_SetSelectionMode(self, nullptr, 0);
    self->field_0c90 = S_00552b80_GetStatsInterface(self, nullptr);
    MpSpawnPoints_RefreshScenario();

    uint32_t statsInterface = self->field_0c90;
    if (statsInterface)
    {
        MpSpawnPoints_AttachStatsInterface(
            reinterpret_cast<uint8_t*>(self) + 0xC94,
            nullptr, static_cast<int>(statsInterface));
    }

    CSF_AT(uint8_t, reinterpret_cast<void*>(GetValue_8BC170()), 0) = 1;

    char* scenarioName =
        MpSpawnPoints_GetSelectionName(selectedOwner, nullptr) + 7;
    auto* menu = reinterpret_cast<S_004ff0f0*>(GetGlobalValue_8B0568());
    GlobalMenu_SetSelectedScenarioName(menu, nullptr, scenarioName, 0);

    CSF_AT(uint8_t, self, 0x14A2) = 1;
    CSF_AT(uint8_t, self, 0xC9D) = 0;
    CSF_AT(uint8_t, self, 0x1400) = 1;
    CSF_AT(uint8_t, self, 0x13C8) = 0;

    int team = static_cast<int>(static_cast<int8_t>(
        MpSpawnPoints_GetSelectionName(selectedOwner, nullptr)[6]));
    CSF_AT(int, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xCC) = team;

    void* characterState = reinterpret_cast<uint8_t*>(selectedOwner) + 0x308;
    if (((bool(__fastcall*)(void*, void*))
            CSF_AT(void**, characterState, 0)[8 / 4])(characterState, nullptr))
    {
        MpSpawnPoints_ResetSelectedCharacter(selectedOwner, nullptr);
    }

    S_00552b80_ResetSelection(self, nullptr, 0);
    ((void(__fastcall*)(void*, void*, float))
        CSF_AT(void**, characterState, 0)[12 / 4])(characterState, nullptr, 1.0f);

    MpSpawnPoints_ResetCharacterMenu(
        reinterpret_cast<void*>(GetGlobalDword_73C020()), nullptr);

    if (GetGlobalState_6A6D70() &&
        ((bool(__fastcall*)(void*, void*))
            CSF_AT(void**, characterState, 0)[8 / 4])(characterState, nullptr))
    {
        void* networkState = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        char* fullName = MpSpawnPoints_GetSelectionName(selectedOwner, nullptr);
        // Only the low byte was initialized in the original stack argument.
        int selectionKind = static_cast<uint8_t>(
            MpSpawnPoints_GetSelectionName(selectedOwner, nullptr)[3]);
        char* teamName = MpSpawnPoints_GetSelectionName(selectedOwner, nullptr);
        void** networkVtable = CSF_AT(void**, networkState, 0);
        int selectedTeam = static_cast<int>(static_cast<int8_t>(teamName[6]));
        char* idName = MpSpawnPoints_GetSelectionName(selectedOwner, nullptr);
        int selectionId =
            MpSpawnPoints_GetSelectionId(reinterpret_cast<int>(idName), 0);

        ((void(__fastcall*)(void*, void*, int, int, int, char*))
            networkVtable[104 / 4])(
                networkState, nullptr, selectionId, selectedTeam,
                selectionKind, fullName + 7);
    }

    statsInterface = self->field_0c90;
    if (statsInterface)
    {
        MpSpawnPoints_ResetStats(
            reinterpret_cast<void*>(statsInterface - 1712), nullptr);

        statsInterface = self->field_0c90;
        MpSpawnPoints_RefreshStats(
            statsInterface ? reinterpret_cast<void*>(statsInterface - 1712)
                           : nullptr,
            nullptr);

        statsInterface = self->field_0c90;
        MpSpawnPoints_SetStatsMode(
            statsInterface ? reinterpret_cast<void*>(statsInterface - 1712)
                           : nullptr,
            nullptr, 0);
    }

    CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalDword_73C020()), 0x104) = 0;
    CSF_AT(uint8_t, self, 0x14AC) = 0;
    S_00552b80_RefreshSelection(self, nullptr);

    if (g_CharacterSelectState8B0694)
    {
        selectedAddress = CSF_AT(
            uint32_t, reinterpret_cast<void*>(g_CharacterSelectState8B0694), 4);
        if (selectedAddress)
        {
            ownerAddress = selectedAddress - 776;
            if (ownerAddress)
            {
                void* currentCharacterState =
                    reinterpret_cast<uint8_t*>(ownerAddress) + 0x308;
                if (((bool(__fastcall*)(void*, void*))
                        CSF_AT(void**, currentCharacterState, 0)[8 / 4])(
                            currentCharacterState, nullptr))
                {
                    MpSpawnPoints_ApplyCharacterSelection(
                        reinterpret_cast<void*>(GetGlobalDword_73C020()),
                        nullptr);
                }
            }
        }
    }

    return 1;
}

HOOK(0x00557B40, S_00552b80_ProcessSelectionChange, "auto");
