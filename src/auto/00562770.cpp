#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(C_0082d410*, __fastcall, C_0082d410_ConstructFromText, 0x00434150, (C_0082d410* self, void* , uint32_t* a2));
CSF_FUNC(void*, __fastcall, Profiles_AppendStringRange, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));
CSF_FUNC(void*, __fastcall, AppendSessionEllipsis, 0x0043F750, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetStringStorage, 0x00544310, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalDword_8B0680, 0x0054E520, ());
CSF_FUNC(char*, __fastcall, FindSessionMapDefinition, 0x0054E630, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __cdecl, SpawnPoints_SumTeamCounts, 0x00551BA0, ());
CSF_FUNC(char, __fastcall, CMenuMpSessions_MatchesNetworkSessionFilter, 0x00560FD0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, CMenuMpSessions_MatchesOnlineSessionFilter, 0x00561120, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, StringResize, 0x005623F0, (void* self, void* , unsigned int newSize));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(unsigned int, __cdecl, ConvertWideTextToNarrow, 0x00695620, (std::uint16_t* source, char* destination, unsigned int length));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(int, __fastcall, CGuiListBox_GetItemCount_714860, 0x00714860, (CGuiListBox* self, void*));
CSF_FUNC(int, __fastcall, GetStoredValueAtIndex, 0x00714CA0, (void* self, void* , int index));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_ClearStorageAndSendEvent, 0x00715660, (CGuiListBoxBase* self, void*));
CSF_FUNC(void, __fastcall, CGuiListBoxBase_AddSelectedItem, 0x00716A60, (CGuiListBoxBase* self, void* , int index));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, NetSession_CreateSessionInfo, 0x0073D150, (void* self, void* , int sessionIndex));
CSF_FUNC(int, __cdecl, ConvertSessionNameText, 0x00749720, (int a1));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

static CGuiListBox* CMenuMpSessions_GetSessionListBox(CMenuMpSessions* self)
{
    return reinterpret_cast<CGuiListBox*>(self->field_0070);
}

static std::uint8_t* GetSessionStringText(C_0082d410* text)
{
    if (CSF_AT(unsigned int, text, 0x18) < 0x10)
        return reinterpret_cast<std::uint8_t*>(&text->field_0004);
    return reinterpret_cast<std::uint8_t*>(text->field_0004);
}

static int __fastcall CMenuMpSessions_RefreshOptionState(
    CMenuMpSessions* self, void* /*edx*/, char requirePing, char requireTeamCount)
{
    if (!CMenuMpSessions_GetSessionListBox(self))
        return 0;

    int selectedValue = 0;
    if (CGuiListBox_GetItemCount_714860(CMenuMpSessions_GetSessionListBox(self), nullptr))
        selectedValue = GetStoredValueAtIndex(CMenuMpSessions_GetSessionListBox(self), nullptr, 0);

    CMenuMpSessions_GetSessionListBox(self)->vClearRowsAndResetLinkedControl();

    bool anySessions = false;
    int onlineMode;
    if (GetGlobal8B0688())
    {
        GetGlobal8B0688();
        onlineMode = g_OnlineMatchFlag;
    }
    else
    {
        onlineMode = g_MpNetworkType == 3;
    }

    alignas(8) std::uint8_t sessionNameBytes[0x1C];
    alignas(8) std::uint8_t mapNameBytes[0x1C];
    alignas(8) std::uint8_t mapKeyBytes[0x1C];
    C_0082d410& sessionName = *reinterpret_cast<C_0082d410*>(sessionNameBytes);
    C_0082d410& mapName = *reinterpret_cast<C_0082d410*>(mapNameBytes);
    C_0082d410& mapKey = *reinterpret_cast<C_0082d410*>(mapKeyBytes);
    alignas(8) char numberBuffer[0x34];

    int rowCount = 0;
    if (onlineMode)
    {
        int sessionIndex = 0;
        void* session;
        do
        {
            void* state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
            session = reinterpret_cast<void*>(
                reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, state, 0)[200 / 4])(state, nullptr, sessionIndex));

            if (session
                && CMenuMpSessions_MatchesOnlineSessionFilter(
                    self, nullptr, reinterpret_cast<int>(session))
                && (!requirePing || CSF_AT(int, session, 0x2AC) >= 0)
                && (!requireTeamCount
                    || SpawnPoints_SumTeamCounts() == CSF_AT(int, session, 0x2B0))
                && CSF_AT(int, session, 0x2A8) > 0)
            {
                anySessions = true;
                auto* convertedName = reinterpret_cast<uint32_t*>(
                    ConvertSessionNameText(
                        reinterpret_cast<int>(static_cast<std::uint8_t*>(session) + 3)));
                C_0082d410_ConstructFromText(&sessionName, nullptr, convertedName);
                if (CSF_AT(unsigned int, &sessionName, 0x14) > 0xF)
                {
                    StringResize(&sessionName, nullptr, 0xF);
                    AppendSessionEllipsis(
                        &sessionName, nullptr,
                        const_cast<char*>(CSF_STR(0x0084319C, "...")));
                }

                CMenuMpSessions_GetSessionListBox(self)->vForwardConvertedText(
                    rowCount, 0, GetSessionStringText(&sessionName), sessionIndex);

                C_0082d410_ConstructFromText(
                    &mapKey, nullptr,
                    reinterpret_cast<uint32_t*>(static_cast<std::uint8_t*>(session) + 0xA3));
                void* mapRegistry = reinterpret_cast<void*>(GetGlobalDword_8B0680());
                char* mapDefinition = FindSessionMapDefinition(
                    mapRegistry, nullptr, reinterpret_cast<int>(&mapKey));
                CMenuSaveGame_ResetStringStorage(&mapKey, nullptr);
                C_0082d410_ConstructFromText(
                    &mapName, nullptr, reinterpret_cast<uint32_t*>(mapDefinition + 0x100));
                if (CSF_AT(unsigned int, &mapName, 0x14) > 0xD)
                {
                    StringResize(&mapName, nullptr, 0xD);
                    AppendSessionEllipsis(
                        &mapName, nullptr,
                        const_cast<char*>(CSF_STR(0x0084319C, "...")));
                }

                CMenuMpSessions_GetSessionListBox(self)->vForwardConvertedText(
                    rowCount, 2, GetSessionStringText(&mapName), -1);

                if (requirePing)
                {
                    _sprintf(numberBuffer, CSF_STR(0x00845A04, "%d/%d"),
                        CSF_AT(int, session, 0x2A4), CSF_AT(int, session, 0x2A8));
                    CMenuMpSessions_GetSessionListBox(self)->vForwardConvertedText(
                        rowCount, 1, reinterpret_cast<std::uint8_t*>(numberBuffer),
                        CSF_AT(int, session, 0x2A8) != CSF_AT(int, session, 0x2A4));

                    _sprintf(numberBuffer, CSF_STR(0x0083090C, "%d"),
                        CSF_AT(int, session, 0x2AC));
                    CMenuMpSessions_GetSessionListBox(self)->vForwardConvertedText(
                        rowCount, 3, reinterpret_cast<std::uint8_t*>(numberBuffer),
                        CSF_AT(int, session, 0x2AC));

                    const int ping = CSF_AT(int, session, 0x2AC);
                    alignas(8) uint32_t color[4];
                    if (ping < 0 || ping >= 301)
                    {
                        color[0] = 1128792064;
                        color[1] = 0;
                        color[2] = 1112014848;
                        color[3] = 1132396544;
                    }
                    else if (ping >= 151)
                    {
                        color[0] = 1130430464;
                        color[1] = 1130430464;
                        color[2] = 0;
                        color[3] = 1132396544;
                    }
                    else
                    {
                        color[0] = 1132396544;
                        color[1] = 1132396544;
                        color[2] = 1132396544;
                        color[3] = 1132396544;
                    }
                    CMenuMpSessions_GetSessionListBox(self)->vSetRowColor(rowCount, color);
                }

                ++rowCount;
                CMenuSaveGame_ResetStringStorage(&mapName, nullptr);
                CMenuSaveGame_ResetStringStorage(&sessionName, nullptr);
            }

            if (requireTeamCount && session
                && SpawnPoints_SumTeamCounts() != CSF_AT(int, session, 0x2B0))
                SpawnPoints_SumTeamCounts();

            ++sessionIndex;
        } while (session);
    }
    else
    {
        void* session = nullptr;
        int sessionIndex = 0;
        bool haveSession;
        alignas(8) char narrowName[0x80];
        do
        {
            if (session)
                Mem_Free(session);
            void* network = reinterpret_cast<void*>(GetGlobalDword_73C020());
            session = reinterpret_cast<void*>(
                NetSession_CreateSessionInfo(network, nullptr, sessionIndex));
            haveSession = session != nullptr;

            if (session
                && CMenuMpSessions_MatchesNetworkSessionFilter(
                    self, nullptr, reinterpret_cast<int>(session))
                && (!requireTeamCount
                    || SpawnPoints_SumTeamCounts() == CSF_AT(int, session, 0x18C))
                && CSF_AT(int, session, 0x188) > 0)
            {
                std::memset(narrowName, 0, 0x80);
                anySessions = true;
                ConvertWideTextToNarrow(
                    reinterpret_cast<std::uint16_t*>(static_cast<std::uint8_t*>(session) + 4),
                    narrowName, 127);
                auto* convertedName = reinterpret_cast<uint32_t*>(
                    ConvertSessionNameText(reinterpret_cast<int>(narrowName)));
                C_0082d410_ConstructFromText(&sessionName, nullptr, convertedName);
                if (CSF_AT(unsigned int, &sessionName, 0x14) > 0xF)
                {
                    StringResize(&sessionName, nullptr, 0xF);
                    Profiles_AppendStringRange(&sessionName, 0,
                        const_cast<char*>(CSF_STR(0x0084319C, "...")), 3);
                }
                CMenuMpSessions_GetSessionListBox(self)->vForwardConvertedText(
                    rowCount, 0, GetSessionStringText(&sessionName), sessionIndex);

                C_0082d410_ConstructFromText(
                    &mapName, nullptr,
                    reinterpret_cast<uint32_t*>(static_cast<std::uint8_t*>(session) + 0x104));
                if (CSF_AT(unsigned int, &mapName, 0x14) > 0xD)
                {
                    StringResize(&mapName, nullptr, 0xD);
                    Profiles_AppendStringRange(&mapName, 0,
                        const_cast<char*>(CSF_STR(0x0084319C, "...")), 3);
                }
                CMenuMpSessions_GetSessionListBox(self)->vForwardConvertedText(
                    rowCount, 2, GetSessionStringText(&mapName), -1);

                _sprintf(numberBuffer, CSF_STR(0x00845A04, "%d/%d"),
                    CSF_AT(int, session, 0x184), CSF_AT(int, session, 0x188));
                CMenuMpSessions_GetSessionListBox(self)->vForwardConvertedText(
                    rowCount, 1, reinterpret_cast<std::uint8_t*>(numberBuffer),
                    CSF_AT(int, session, 0x184) != CSF_AT(int, session, 0x188));
                ++rowCount;

                if (CSF_AT(unsigned int, &mapName, 0x18) >= 0x10 && mapName.field_0004)
                    Mem_FreeDebug(reinterpret_cast<void*>(mapName.field_0004));
                CSF_AT(unsigned int, &mapName, 0x18) = 15;
                CSF_AT(unsigned int, &mapName, 0x14) = 0;
                reinterpret_cast<std::uint8_t*>(&mapName.field_0004)[0] = 0;

                if (CSF_AT(unsigned int, &sessionName, 0x18) >= 0x10 && sessionName.field_0004)
                    Mem_FreeDebug(reinterpret_cast<void*>(sessionName.field_0004));
                CSF_AT(unsigned int, &sessionName, 0x18) = 15;
                CSF_AT(unsigned int, &sessionName, 0x14) = 0;
                reinterpret_cast<std::uint8_t*>(&sessionName.field_0004)[0] = 0;
            }

            if (requireTeamCount && session
                && SpawnPoints_SumTeamCounts() != CSF_AT(int, session, 0x18C))
                SpawnPoints_SumTeamCounts();

            ++sessionIndex;
        } while (haveSession);
    }

    CGuiListBoxBase_ClearStorageAndSendEvent(CMenuMpSessions_GetSessionListBox(self), nullptr);
    CGuiListBoxBase_AddSelectedItem(CMenuMpSessions_GetSessionListBox(self), nullptr, selectedValue);
    self->field_01d0 = 1;
    return anySessions ? 1 : 0;
}

HOOK(0x00562770, CMenuMpSessions_RefreshOptionState, "auto");
