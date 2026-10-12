#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, SceneLoader_DestroyMenuBuffer, 0x004FF3C0, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, MenuBuffer_Init, 0x00501870, (uint8_t* self, void*));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, Object_IsFieldF0NonZero, 0x0050E210, (void* self, void*));
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __cdecl, SpawnPoints_SumTeamCounts, 0x00551BA0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(char, __stdcall, GlobalObject_ApplyMode, 0x0073C7F0, (char mode));
CSF_FUNC(int*, __cdecl, MenuMpSessions_CreateSessionRequest, 0x00749820, (char a1, int a2, int a3, int a4, char a5));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(uintptr_t, __cdecl, __beginthread, 0x007B07F5, (void (__cdecl* startAddress)(void*), unsigned stackSize, void* arg));

CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);
CSF_GLOBAL(uint8_t, g_SessionThreadStarting, 0x008B0903);

static int __fastcall CMenuMpSessions_StartSession(
    CMenuMpSessions* self, void* /*edx*/)
{
    alignas(8) uint8_t menuBytes[0xB8];

    self->field_0014 = 3;
    MenuBuffer_Init(menuBytes, nullptr);
    menuBytes[0x18] = 1;
    SceneLoader_NoOp();

    void* loader = reinterpret_cast<void*>(GetGlobalState_50D190());
    if (static_cast<std::uint8_t>(
            Object_IsFieldF0NonZero(loader, nullptr)) == 0)
    {
        loader = reinterpret_cast<void*>(GetGlobalState_50D190());
        GameBddLoader_DispatchMenuRequest(
            loader, nullptr, 1330458933, 1, 0,
            reinterpret_cast<int>(menuBytes));
    }

    int useNetworkSession;
    if (GetGlobal8B0688())
    {
        GetGlobal8B0688();
        useNetworkSession = g_OnlineMatchFlag;
    }
    else
    {
        useNetworkSession = g_MpNetworkType == 3;
    }

    if (useNetworkSession)
    {
        char alternateNetworkMode;
        if (GetGlobal8B0688())
        {
            GetGlobal8B0688();
            alternateNetworkMode = g_OnlineMatchFlag == 0;
        }
        else
        {
            alternateNetworkMode = g_MpNetworkType == 2;
        }

        void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        const char sessionOption = CSF_AT(char, settings, 0x1D6C);
        CSF_AT(int*, menuBytes, 0xB4) =
            MenuMpSessions_CreateSessionRequest(
                alternateNetworkMode,
                static_cast<int>(self->field_01c0),
                static_cast<int>(self->field_0018),
                static_cast<int>(self->field_001c),
                sessionOption);

        void* networkState = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        void** networkVtable = CSF_AT(void**, networkState, 0);
        const int spawnPointCount = SpawnPoints_SumTeamCounts();

        const bool handled =
            reinterpret_cast<bool (__fastcall*)(
                void*, void*, std::uint32_t, std::uint32_t, int)>(
                    networkVtable[0xC4 / 4])(
                        networkState, nullptr,
                        self->field_0018, self->field_001c, spawnPointCount);

        if (!handled)
        {
            networkState = reinterpret_cast<void*>(GetGlobalState_6A6D70());
            const std::uint32_t attempt = self->field_01d8;
            networkVtable = CSF_AT(void**, networkState, 0);
            reinterpret_cast<void (__fastcall*)(
                void*, void*, int*, std::uint32_t)>(
                    networkVtable[0xC0 / 4])(
                        networkState, nullptr,
                        CSF_AT(int*, menuBytes, 0xB4), attempt);
        }

        ++self->field_01d8;
    }
    else
    {
        GetGlobalDword_73C020();
        GlobalObject_ApplyMode(0);
        g_SessionThreadStarting = 1;
        __beginthread(
            reinterpret_cast<void (__cdecl*)(void*)>(
                CSF_ADDR_FN(0x00560070)),
            0, nullptr);
    }

    return static_cast<int>(SceneLoader_DestroyMenuBuffer(
        reinterpret_cast<std::uint32_t*>(menuBytes), nullptr));
}

HOOK(0x00562D50, CMenuMpSessions_StartSession, "auto");
