#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, SceneLoader_DestroyMenuBuffer, 0x004FF3C0, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, MenuBuffer_Init, 0x00501870, (uint8_t* self, void*));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(unsigned int, __cdecl, ConvertWideTextToNarrow, 0x00695620, (std::uint16_t* source, char* destination, unsigned int length));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(int, __fastcall, GetIndexedTableValue_714C20, 0x00714C20, (int* self, void* , int rowIndex, int columnIndex));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, NetSession_CreateSessionInfo, 0x0073D150, (void* self, void* , int sessionIndex));
CSF_FUNC(void, __fastcall, C_0085839c_StartSessionByIndex, 0x0073DC30, (C_0085839c* self, void* , uint32_t entryIndex));
CSF_FUNC(bool, __cdecl, MenuMpSessions_EvaluateSessionNameFlag, 0x00749750, (int a1));

CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);
CSF_GLOBAL(int, g_MpSessionState, 0x008B0690);

static unsigned int __fastcall CMenuMpSessions_JoinSelectedSession(
    CMenuMpSessions* self, void* /*edx*/)
{
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

    unsigned int result;
    if (online)
    {
        int availability = GetIndexedTableValue_714C20(
            reinterpret_cast<int*>(self->field_0070), nullptr,
            static_cast<int>(self->field_01cc), 3);
        result = GetIndexedTableValue_714C20(
            reinterpret_cast<int*>(self->field_0070), nullptr,
            static_cast<int>(self->field_01cc), 1);

        if (self->field_0070 && availability != -1 && result)
        {
            int sessionIndex = GetIndexedTableValue_714C20(
                reinterpret_cast<int*>(self->field_0070), nullptr,
                static_cast<int>(self->field_01cc), 0);
            void* state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
            std::uint32_t session =
                reinterpret_cast<std::uint32_t(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, state, 0)[200 / 4])(
                        state, nullptr, sessionIndex);

            self->field_01c4 = session;
            self->field_0014 = 4;
            self->field_01dc = 0;

            bool nameFlag = MenuMpSessions_EvaluateSessionNameFlag(
                static_cast<int>(session + 3));
            void* network = reinterpret_cast<void*>(GetGlobalDword_73C020());
            CSF_AT(std::uint8_t, network, 0x114) = nameFlag;
            g_OnlineMatchFlag = 1;

            alignas(8) std::uint8_t menuBuffer[0x34];
            MenuBuffer_Init(menuBuffer, nullptr);
            menuBuffer[24] = 1;
            SceneLoader_NoOp();
            void* gameState = reinterpret_cast<void*>(GetGlobalState_50D190());
            GameBddLoader_DispatchMenuRequest(
                gameState, nullptr, 1330459184, 1, 0,
                reinterpret_cast<int>(menuBuffer));

            std::uint32_t selectedSession = self->field_01c4;
            self->field_01c8 = selectedSession ? selectedSession + 163 : 0;

            state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
            reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t)>(
                CSF_AT(void**, state, 0)[96 / 4])(
                    state, nullptr, self->field_01c4);
            g_MpSessionState = 0;

            result = SceneLoader_DestroyMenuBuffer(
                reinterpret_cast<std::uint32_t*>(menuBuffer), nullptr);
        }
    }
    else
    {
        int sessionIndex = -1;
        if (self->field_0070)
        {
            sessionIndex = GetIndexedTableValue_714C20(
                reinterpret_cast<int*>(self->field_0070), nullptr,
                static_cast<int>(self->field_01cc), 0);
        }

        int availability = GetIndexedTableValue_714C20(
            reinterpret_cast<int*>(self->field_0070), nullptr,
            static_cast<int>(self->field_01cc), 1);
        void* network = reinterpret_cast<void*>(GetGlobalDword_73C020());
        result = NetSession_CreateSessionInfo(network, nullptr, sessionIndex);

        if (result && availability)
        {
            void* sessionInfo = reinterpret_cast<void*>(result);
            alignas(8) std::uint8_t sessionNameBytes[0x88];
            std::memset(sessionNameBytes, 0, 128);
            ConvertWideTextToNarrow(
                reinterpret_cast<std::uint16_t*>(
                    static_cast<std::uint8_t*>(sessionInfo) + 4),
                reinterpret_cast<char*>(sessionNameBytes), 127);

            bool nameFlag = MenuMpSessions_EvaluateSessionNameFlag(
                reinterpret_cast<int>(sessionNameBytes));
            network = reinterpret_cast<void*>(GetGlobalDword_73C020());
            CSF_AT(std::uint8_t, network, 0x114) = nameFlag;
            Mem_Free(sessionInfo);
            g_OnlineMatchFlag = 0;

            alignas(8) std::uint8_t menuBuffer[0xB4];
            MenuBuffer_Init(menuBuffer, nullptr);
            menuBuffer[24] = 1;
            SceneLoader_NoOp();
            void* gameState = reinterpret_cast<void*>(GetGlobalState_50D190());
            GameBddLoader_DispatchMenuRequest(
                gameState, nullptr, 1330459184, 1, 0,
                reinterpret_cast<int>(menuBuffer));

            C_0085839c* sessionManager =
                reinterpret_cast<C_0085839c*>(GetGlobalDword_73C020());
            C_0085839c_StartSessionByIndex(
                sessionManager, nullptr, static_cast<std::uint32_t>(sessionIndex));
            self->field_0014 = 4;

            result = SceneLoader_DestroyMenuBuffer(
                reinterpret_cast<std::uint32_t*>(menuBuffer), nullptr);
        }
    }

    self->field_01cc = static_cast<std::uint32_t>(-1);
    return result;
}

HOOK(0x00562ED0, CMenuMpSessions_JoinSelectedSession, "auto");
