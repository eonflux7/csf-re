#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, CMenuMpScoreBar_ApplyChildDisplayMode, 0x005408F0, (CMenuMpScoreBar* self, void*));
CSF_FUNC(char, __fastcall, MpSpawnPoints_UpdateOnlineSpawn, 0x00551340, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, MpSpawnPoints_UpdateSceneLoadPhase, 0x00551A30, (void* self, void* edx));
CSF_FUNC(void, __fastcall, MpSpawnPoints_UpdateOnlineSpawnTimer, 0x005525C0, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, MpSpawnPoints_ResetSpawnState, 0x00552920, (void* self, void* edx));
CSF_FUNC(char, __cdecl, MpSpawnPoints_ResetSelectionState, 0x00552990, ());
CSF_FUNC(char, __fastcall, MpSpawnPoints_FinalizeSpawnUpdate, 0x00552C50, (void* self, void* edx));
CSF_FUNC(void, __fastcall, MpSpawnPoints_UpdateSpawnDisplay, 0x005537D0, (void* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, MpSpawnPoints_UpdateSpawnMenuState, 0x00554DD0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_UpdateSpawnOptions, 0x00554E30, (void* self, void* edx));
CSF_FUNC(void, __fastcall, MpSpawnPoints_UpdateSpawnMenuTimer, 0x00554FF0, (void* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, MpSpawnPoints_UpdateTeamSelection, 0x00555660, (void* self, void* edx));
CSF_FUNC(void**, __fastcall, MpSpawnPoints_UpdateTeamSelectionDisplay, 0x005556F0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, MpSpawnPoints_UpdateSpawnCountdown, 0x00556330, (void* self, void* edx, float arg0));
CSF_FUNC(int, __fastcall, MpSpawnPoints_UpdateCharacterSelection, 0x005568C0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, MpSpawnPoints_UpdateSpawnIndicators, 0x00556DB0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_UpdateSpawnIndicatorTimer, 0x00556E40, (void* self, void* edx, float a2));
CSF_FUNC(void**, __fastcall, MpSpawnPoints_UpdateSelectionDisplay, 0x00557160, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, MpSpawnPoints_UpdateSelectionTimer, 0x00557410, (void* self, void* edx, float arg0));
CSF_FUNC(char, __fastcall, MpSpawnPoints_CheckSpawnTransition, 0x00557B40, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(char*, __fastcall, MpSpawnPoints_GetCharacterSelectionData, 0x006B29A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(char, __fastcall, MpSpawnPoints_RefreshLocalSelection, 0x0073C8A0, (void* self, void* edx));

CSF_GLOBAL(const char*, g_pszMenu_P_TmPart, 0x00899B20);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);
CSF_GLOBAL(int, g_MpSessionState, 0x008B0690);
CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

static void* MpSpawnPoints_AsObject(uint32_t address)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(address));
}

static void __fastcall MpSpawnPoints_Update(void* self, void* /*edx*/, float deltaTime)
{
    char transition = 0;
    if (!CSF_AT(uint8_t, self, 0x1400))
        transition = MpSpawnPoints_CheckSpawnTransition(self, nullptr);

    if (!CSF_AT(uint8_t, self, 0x14A2) && !transition)
    {
        if (CSF_AT(uint8_t, self, 0x1400))
        {
            CSF_AT(uint8_t, self, 0x1400) = 0;
            MpSpawnPoints_ResetSpawnState(self, nullptr);
            CSF_AT(uint8_t, MpSpawnPoints_AsObject(GetGlobalDword_73C020()), 0x104) = 1;
            MpSpawnPoints_ResetSelectionState();

            void* clock = CSF_AT(void*, MpSpawnPoints_AsObject(GetGlobalState_50D190()), 0x90);
            void* state = MpSpawnPoints_AsObject(GetGlobalState_50D190());
            double time = ((double(__fastcall*)(void*, void*))
                CSF_AT(void**, clock, 0)[4])(clock, nullptr);
            int32_t ticks = static_cast<int32_t>(static_cast<int64_t>(time));
            CSF_AT(int32_t, state, 0xC4) = ticks <= 0 ? 1 : ticks;
            CSF_AT(int, self, 0x14A8) = 2;

            if (g_CharacterSelectState8B0694)
            {
                uint32_t linked = CSF_AT(uint32_t,
                    MpSpawnPoints_AsObject(g_CharacterSelectState8B0694), 4);
                if (linked && linked - 776)
                {
                    void* selectionInterface = MpSpawnPoints_AsObject(linked);
                    bool active = ((bool(__fastcall*)(void*, void*))
                        CSF_AT(void**, selectionInterface, 0)[2])(selectionInterface, nullptr);
                    if (active && g_CharacterSelectState8B0694)
                    {
                        linked = CSF_AT(uint32_t,
                            MpSpawnPoints_AsObject(g_CharacterSelectState8B0694), 4);
                        if (linked && linked != 776)
                        {
                            void* character = MpSpawnPoints_AsObject(linked - 776);
                            if (g_OnlineMatchFlag)
                            {
                                void* session = MpSpawnPoints_AsObject(GetGlobalState_6A6D70());
                                char* data = MpSpawnPoints_GetCharacterSelectionData(character, nullptr);
                                int selection = CSF_AT(int8_t, data, 5);
                                ((void(__fastcall*)(void*, void*, int))
                                    CSF_AT(void**, session, 0)[68])(session, nullptr, selection);
                            }
                            else
                            {
                                void* localState = MpSpawnPoints_AsObject(GetGlobalDword_73C020());
                                MpSpawnPoints_RefreshLocalSelection(localState, nullptr);
                            }
                        }
                    }
                }
            }
        }

        CSF_AT(float, self, 0x13D8) = CSF_AT(float, self, 0x13D8) - deltaTime;

        uint32_t selectionState = g_CharacterSelectState8B0694;
        void* character = nullptr;
        if (selectionState)
        {
            uint32_t linked = CSF_AT(uint32_t, MpSpawnPoints_AsObject(selectionState), 4);
            if (linked)
            {
                character = MpSpawnPoints_AsObject(linked - 776);
                if (character)
                {
                    CSF_AT(uint32_t, self, 0x1438) = CSF_AT(uint32_t, character, 0x44);
                    CSF_AT(uint32_t, self, 0x143C) = CSF_AT(uint32_t, character, 0x50);
                    selectionState = g_CharacterSelectState8B0694;
                }
            }
        }

        if (!CSF_AT(uint8_t, self, 0x13C8) && selectionState)
        {
            void* menu = CSF_AT(void*, self, 0x13BC);
            CMenuMpScoreBar* scoreBar = ((CMenuMpScoreBar*(__fastcall*)(void*, void*, const char*))
                CSF_AT(void**, menu, 0)[22])(menu, nullptr, g_pszMenu_P_TmPart);
            CSF_AT(CMenuMpScoreBar*, self, 0x13C0) = scoreBar;
            CSF_AT(uint8_t, self, 0x13C8) = 1;

            bool displayMode = true;
            char* data = MpSpawnPoints_GetCharacterSelectionData(character, nullptr);
            switch (CSF_AT(int8_t, data, 6))
            {
            case 0:
                data = MpSpawnPoints_GetCharacterSelectionData(character, nullptr);
                displayMode = CSF_AT(uint32_t, data, 0x188) != 0;
                break;
            case 1:
                data = MpSpawnPoints_GetCharacterSelectionData(character, nullptr);
                displayMode = CSF_AT(uint32_t, data, 0x190) != 0;
                break;
            case 2:
                displayMode = false;
                break;
            case 3:
                data = MpSpawnPoints_GetCharacterSelectionData(character, nullptr);
                displayMode = CSF_AT(uint32_t, data, 0x18C) != 0;
                break;
            default:
                break;
            }

            scoreBar = CSF_AT(CMenuMpScoreBar*, self, 0x13C0);
            scoreBar->field_0028 = displayMode;
            CMenuMpScoreBar_ApplyChildDisplayMode(scoreBar, nullptr);
            selectionState = g_CharacterSelectState8B0694;
        }

        void* spawnHandle = CSF_AT(void*, self, 0xC88);
        if (spawnHandle)
        {
            uint32_t linked = CSF_AT(uint32_t, spawnHandle, 4);
            uint32_t base = linked ? linked - 1088 : 0;
            void* spawnInterface = MpSpawnPoints_AsObject(base + 1088);
            ((void(__fastcall*)(void*, void*, float))
                CSF_AT(void**, spawnInterface, 0)[3])(spawnInterface, nullptr, deltaTime);
            selectionState = g_CharacterSelectState8B0694;
        }

        if (selectionState)
        {
            uint32_t linked = CSF_AT(uint32_t, MpSpawnPoints_AsObject(selectionState), 4);
            uint32_t base = linked ? linked - 776 : 0;
            void* selectionInterface = MpSpawnPoints_AsObject(base + 776);
            ((void(__fastcall*)(void*, void*, float))
                CSF_AT(void**, selectionInterface, 0)[3])(selectionInterface, nullptr, deltaTime);
        }

        MpSpawnPoints_UpdateSpawnOptions(self, nullptr);
        MpSpawnPoints_UpdateTeamSelection(self, nullptr);
        MpSpawnPoints_UpdateCharacterSelection(self, nullptr);
        MpSpawnPoints_UpdateSelectionDisplay(self, nullptr);
        MpSpawnPoints_UpdateTeamSelectionDisplay(self, nullptr);
        MpSpawnPoints_UpdateSpawnDisplay(self, nullptr, deltaTime);
        MpSpawnPoints_UpdateSpawnIndicators(self, nullptr);
        MpSpawnPoints_UpdateSelectionTimer(self, nullptr, deltaTime);
        MpSpawnPoints_UpdateSpawnMenuState(self, nullptr);
        MpSpawnPoints_UpdateSpawnIndicatorTimer(self, nullptr, deltaTime);
        MpSpawnPoints_UpdateSpawnMenuTimer(self, nullptr, deltaTime);
        MpSpawnPoints_UpdateSpawnCountdown(self, nullptr, deltaTime);

        void* auxiliary = CSF_AT(void*, self, 0x1484);
        if (auxiliary)
        {
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, auxiliary, 0)[3])(auxiliary, nullptr);
        }

        if (GetGlobalState_6A6D70() && g_OnlineMatchFlag)
        {
            void* session = MpSpawnPoints_AsObject(GetGlobalState_6A6D70());
            int connected = ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, session, 0)[7])(session, nullptr);
            if (connected)
            {
                int deltaBits;
                std::memcpy(&deltaBits, &deltaTime, sizeof(deltaBits));
                MpSpawnPoints_UpdateOnlineSpawn(self, nullptr, deltaBits);
                MpSpawnPoints_UpdateOnlineSpawnTimer(self, nullptr, deltaTime);

                session = MpSpawnPoints_AsObject(GetGlobalState_6A6D70());
                bool ready = ((bool(__fastcall*)(void*, void*))
                    CSF_AT(void**, session, 0)[44])(session, nullptr);
                if (ready)
                    g_MpSessionState = 4;
            }
            else
            {
                g_MpSessionState = 5;
            }
        }

        MpSpawnPoints_FinalizeSpawnUpdate(self, nullptr);
    }

    MpSpawnPoints_UpdateSceneLoadPhase(self, nullptr);
}

HOOK(0x00557DD0, MpSpawnPoints_Update, "auto");
