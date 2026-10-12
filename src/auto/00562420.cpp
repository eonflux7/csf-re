#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_008310b8_GetSelf, 0x004B2980, (C_008310b8* self, void*));
CSF_FUNC(std::uint32_t, __fastcall, SceneLoader_DestroyMenuBuffer, 0x004FF3C0, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, MenuBuffer_Init, 0x00501870, (uint8_t* self, void*));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(int, __fastcall, MpSpawnPoints_InitializeSelection, 0x00550C40, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __cdecl, MpSpawn_DispatchThreeCategories, 0x005512D0, (int context));
CSF_FUNC(uint32_t, __cdecl, MpSpawnPoints_InitializeCharacterSelection, 0x00551AD0, (int sourceObject));
CSF_FUNC(int, __cdecl, SpawnPoints_SumTeamCounts, 0x00551BA0, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(void*, __fastcall, NetSession_InitWithNameOrDefault, 0x0073C350, (void* self, void* , uint8_t* name));
CSF_FUNC(C_008310b8*, __stdcall, MpSpawnPoints_SetMapNameParameter, 0x0073C7C0, (int mapNameValue));
CSF_FUNC(int, __stdcall, NetSession_ReportCrcValue, 0x0073C920, (int crcValue));
CSF_FUNC(char, __fastcall, NetSession_TryConnect, 0x0073CF70, (void* self, void*));
CSF_FUNC(void*, __fastcall, NetSession_SetStationNameText, 0x0073D0A0, (void* self, void* , uint8_t* text));
CSF_FUNC(void*, __fastcall, NetSession_LookupStatsSlotHandle, 0x0073E770, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_ApplyConnectionSettings, 0x0073EDE0, (void* self, void* , char alternate));
CSF_FUNC(char*, __fastcall, MpSpawnPoints_SetSecondaryString, 0x007496C0, (void* self, void* edx, char* Source));
CSF_FUNC(char*, __fastcall, MpSpawnPoints_SetPrimaryString, 0x007496E0, (void* self, void* edx, char* Source));
CSF_FUNC(char*, __fastcall, MpSpawnPoints_SetTertiaryString, 0x00749700, (void* self, void* edx, char* Source));
CSF_FUNC(int, __fastcall, MpSpawnPoints_GetSettingsSelection, 0x00749920, (int a1, int a2));
CSF_FUNC(char*, __cdecl, _strncpy, 0x007AFEC0, (char* dest, const char* src, size_t count));

CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

static void __fastcall CMenuMpSessions_ConfigureAndConnectSession(
    CMenuMpSessions* self, void* /*edx*/,
    char selectionFlag, char* primaryName, int category, int sessionMode,
    int sessionOption, int connectionOption, char* stationName, int mapNameValue,
    char* secondaryName, char flag0, char flag1, char flag2,
    std::int16_t categoryValue, int categoryParameter)
{
    alignas(8) std::uint8_t menuBytes[0xB4];
    alignas(8) std::uint8_t selectionBytes[0x258];
    auto& selection = *reinterpret_cast<C_008310b8*>(selectionBytes);

    C_008310b8_GetSelf(&selection, nullptr);
    CSF_AT(std::uint8_t, selectionBytes, 0x19E) = 0;
    CSF_AT(std::uint32_t, selectionBytes, 0x1B8) = 15;
    CSF_AT(int, selectionBytes, 0x1B4) = 0;
    CSF_AT(std::uint8_t, selectionBytes, 0x1A4) = 0;
    MpSpawnPoints_InitializeSelection(&selection, nullptr);

    reinterpret_cast<std::uint8_t*>(&selection.field_0004)[0] = selectionFlag;
    MpSpawnPoints_SetPrimaryString(&selection, nullptr, primaryName);
    reinterpret_cast<std::uint8_t*>(&selection.field_0004)[1] =
        static_cast<std::uint8_t>(sessionMode);
    MpSpawnPoints_SetTertiaryString(&selection, nullptr, stationName);
    reinterpret_cast<std::uint8_t*>(&selection.vtbl)[2] = flag2;
    reinterpret_cast<std::uint8_t*>(&selection.vtbl)[0] = flag0;
    reinterpret_cast<std::uint8_t*>(&selection.vtbl)[1] = flag1;
    MpSpawnPoints_SetSecondaryString(&selection, nullptr, secondaryName);
    _strncpy(&CSF_AT(char, self, 0xB7), secondaryName, 0x80);

    reinterpret_cast<std::uint8_t*>(&selection.field_0004)[2] =
        static_cast<std::uint8_t>(category);
    reinterpret_cast<std::uint8_t*>(&selection.vtbl)[3] =
        static_cast<std::uint8_t>(sessionOption);
    void* session = reinterpret_cast<void*>(GetGlobalDword_73C020());
    NetSession_SetStationNameText(
        session, nullptr, reinterpret_cast<std::uint8_t*>(stationName));

    switch (reinterpret_cast<std::int8_t*>(&selection.field_0004)[2])
    {
    case 0:
        CSF_AT(std::int16_t, selectionBytes, 0x196) = categoryValue;
        CSF_AT(int, selectionBytes, 0x188) = categoryParameter;
        break;
    case 1:
        CSF_AT(std::int16_t, selectionBytes, 0x198) = categoryValue;
        CSF_AT(int, selectionBytes, 0x190) = categoryParameter;
        break;
    case 3:
        CSF_AT(std::int16_t, selectionBytes, 0x19C) = categoryValue;
        CSF_AT(int, selectionBytes, 0x18C) = categoryParameter;
        break;
    }

    int online;
    if (GetGlobal8B0688())
    {
        GetGlobal8B0688();
        online = g_OnlineMatchFlag;
    }
    else
    {
        online = g_MpNetworkType == 3;
    }
    g_OnlineMatchFlag = online != 0;

    MpSpawn_DispatchThreeCategories(reinterpret_cast<int>(&selection));
    session = reinterpret_cast<void*>(GetGlobalDword_73C020());
    void* statsSlot = NetSession_LookupStatsSlotHandle(session, nullptr);
    MpSpawnPoints_InitializeCharacterSelection(reinterpret_cast<int>(statsSlot));

    int settings = MpSpawnPoints_GetSettingsSelection(
        reinterpret_cast<int>(&selection), 0);
    session = reinterpret_cast<void*>(GetGlobalDword_73C020());
    NetSession_InitWithNameOrDefault(
        session, nullptr, reinterpret_cast<std::uint8_t*>(settings));

    CSF_AT(int, reinterpret_cast<void*>(GetGlobalDword_73C020()), 0x100) =
        static_cast<std::int8_t>(sessionMode);
    CSF_AT(std::uint8_t, reinterpret_cast<void*>(GetGlobalDword_73C020()), 0x114) =
        static_cast<std::uint8_t>(sessionOption);

    int teamCount = SpawnPoints_SumTeamCounts();
    GetGlobalDword_73C020();
    NetSession_ReportCrcValue(teamCount);
    GetGlobalDword_73C020();
    MpSpawnPoints_SetMapNameParameter(mapNameValue);

    if (GetGlobal8B0688())
    {
        GetGlobal8B0688();
        online = g_OnlineMatchFlag;
    }
    else
    {
        online = g_MpNetworkType == 3;
    }
    if (online)
    {
        void* networkState = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, networkState, 0)[0xB4 / 4])(
                networkState, nullptr);
    }

    session = reinterpret_cast<void*>(GetGlobalDword_73C020());
    NetSession_TryConnect(session, nullptr);
    if (CSF_AT(int, reinterpret_cast<void*>(GetGlobalDword_73C020()), 0x98) == 6)
    {
        CSF_AT(int, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xCC) =
            category;
        char alternate = g_OnlineMatchFlag == 0;
        session = reinterpret_cast<void*>(GetGlobalDword_73C020());
        NetSession_ApplyConnectionSettings(session, nullptr, alternate);

        if (GetGlobal8B0688())
        {
            GetGlobal8B0688();
            online = g_OnlineMatchFlag;
        }
        else
        {
            online = g_MpNetworkType == 3;
        }
        if (online)
        {
            void* networkState = reinterpret_cast<void*>(GetGlobalState_6A6D70());
            void** networkVtable = CSF_AT(void**, networkState, 0);
            teamCount = SpawnPoints_SumTeamCounts();
            settings = MpSpawnPoints_GetSettingsSelection(
                reinterpret_cast<int>(&selection), 0);
            reinterpret_cast<void (__fastcall*)(
                void*, void*, int, int, int, int, int, char*, char*, int)>(
                    networkVtable[0x5C / 4])(
                        networkState, nullptr, settings, category, sessionMode,
                        sessionOption, connectionOption, stationName,
                        secondaryName, teamCount);
        }

        self->field_0014 = 1;
        MenuBuffer_Init(menuBytes, nullptr);
        menuBytes[24] = 1;
        SceneLoader_NoOp();
        void* gameState = reinterpret_cast<void*>(GetGlobalState_50D190());
        GameBddLoader_DispatchMenuRequest(
            gameState, nullptr, 1129132342, 1, 0,
            reinterpret_cast<int>(menuBytes));
        SceneLoader_DestroyMenuBuffer(
            reinterpret_cast<std::uint32_t*>(menuBytes), nullptr);
    }

    if (CSF_AT(std::uint32_t, selectionBytes, 0x1B8) >= 0x10 &&
        CSF_AT(void*, selectionBytes, 0x1A4))
    {
        Mem_FreeDebug(CSF_AT(void*, selectionBytes, 0x1A4));
    }
    CSF_AT(std::uint32_t, selectionBytes, 0x1B8) = 15;
    CSF_AT(int, selectionBytes, 0x1B4) = 0;
    CSF_AT(std::uint8_t, selectionBytes, 0x1A4) = 0;
    SceneLoader_NoOp();
}

HOOK(0x00562420, CMenuMpSessions_ConfigureAndConnectSession, "auto");
