#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(void*, __cdecl, MenuMpAccounts_CreateListSentinel, 0x00552DD0, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(void*, __fastcall, MpSpawnPoints_GetSessionInterface, 0x0073C050,
         (void* self, void* edx, int arg0, int a2));
CSF_FUNC(int, __fastcall, MenuMpAccounts_FillPlayerList, 0x0073F3B0,
         (void* self, void* edx, void* list));
CSF_FUNC(void*, __fastcall, MpSpawnPoints_ConstructSessionObserver, 0x0073F700,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_SetObservedSession, 0x0073F730,
         (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, MpSpawnPoints_ResetPlayerCheck, 0x00749B30,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_ClearRegisteredPlayers, 0x00749FE0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_RegisterPlayer, 0x0074A020,
         (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, MpSpawnPoints_CheckPlayer, 0x0074A070,
         (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, MpSpawnPoints_IsPlayerCheckComplete, 0x0074A0B0,
         (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(int, g_MpSessionState, 0x008B0690);

static void __fastcall MpSpawnPoints_UpdatePlayerChecks(
    void* self, void* /*edx*/, float elapsed)
{
    void* session = CSF_AT(void*, self, 0x1470);
    if (session)
    {
        void* interfaceObject = static_cast<uint8_t*>(session) + 0x154;
        ((void(__fastcall*)(void*, void*, float))
            CSF_AT(void**, interfaceObject, 0)[3])(
                interfaceObject, nullptr, elapsed);
    }

    // The original also takes this branch for an unordered comparison.
    if (!(CSF_AT(float, self, 0x147C) <= g_fZero))
    {
        CSF_AT(float, self, 0x147C) =
            CSF_AT(float, self, 0x147C) - elapsed;
        return;
    }

    float remaining = CSF_AT(float, self, 0x1478) - elapsed;
    CSF_AT(float, self, 0x1478) = remaining;
    if (!(remaining < g_fZero))
        return;

    CSF_AT(float, self, 0x1478) = 1.0f;

    uint32_t playerList[5]{};
    CSF_AT(void*, playerList, 0x04) = MenuMpAccounts_CreateListSentinel();
    CSF_AT(uint32_t, playerList, 0x08) = 0;
    MenuMpAccounts_FillPlayerList(
        reinterpret_cast<void*>(GetGlobalDword_73C020()), nullptr, playerList);

    if (!CSF_AT(void*, self, 0x1470))
    {
        void* interfaceObject = MpSpawnPoints_GetSessionInterface(
            reinterpret_cast<void*>(GetGlobalDword_73C020()), nullptr, 4, 0);
        CSF_AT(void*, self, 0x1470) = interfaceObject
            ? reinterpret_cast<void*>(
                  reinterpret_cast<uintptr_t>(interfaceObject) - 340)
            : nullptr;

        void* storage = Mem_Alloc(8);
        void* observer = storage
            ? MpSpawnPoints_ConstructSessionObserver(storage, nullptr)
            : nullptr;

        session = CSF_AT(void*, self, 0x1470);
        CSF_AT(void*, self, 0x1474) = observer;
        uintptr_t observedInterface = session
            ? reinterpret_cast<uintptr_t>(session) + 340
            : 0;
        MpSpawnPoints_SetObservedSession(
            observer, nullptr, static_cast<int>(observedInterface));
    }

    session = CSF_AT(void*, self, 0x1470);
    if (session && CSF_AT(uint8_t, self, 0x14A1))
    {
        CSF_AT(uint8_t, self, 0x14A1) = 0;
        MpSpawnPoints_ClearRegisteredPlayers(
            static_cast<uint8_t*>(session) + 0x44, nullptr);

        for (void* player = CSF_AT(
                 void*, CSF_AT(void*, playerList, 0x04), 0);
             player != CSF_AT(void*, playerList, 0x04);
             player = CSF_AT(void*, player, 0))
        {
            int playerId = CSF_AT(int, CSF_AT(void*, player, 0x08), 0x0C);
            MpSpawnPoints_RegisterPlayer(
                reinterpret_cast<void*>(
                    reinterpret_cast<uintptr_t>(
                        CSF_AT(void*, self, 0x1470)) + 68),
                nullptr, playerId);
        }
        SceneLoader_NoOp();
    }

    void* entry = CSF_AT(void*, CSF_AT(void*, self, 0x1468), 0);
    bool complete = false;
    if (entry != CSF_AT(void*, self, 0x1468))
    {
        while (!complete)
        {
            void* observer = CSF_AT(void*, entry, 0x08);
            void* interfaceObject = observer
                ? CSF_AT(void*, observer, 0x04)
                : nullptr;
            uintptr_t sessionAddress = interfaceObject
                ? reinterpret_cast<uintptr_t>(interfaceObject) - 340
                : 0;

            if (observer && interfaceObject && sessionAddress)
            {
                void* playerCheck =
                    reinterpret_cast<void*>(sessionAddress + 68);
                if (playerCheck)
                {
                    MpSpawnPoints_ResetPlayerCheck(playerCheck, nullptr);
                    MpSpawnPoints_ResetPlayerCheck(playerCheck, nullptr);

                    for (void* player = CSF_AT(
                             void*, CSF_AT(void*, playerList, 0x04), 0);
                         player != CSF_AT(void*, playerList, 0x04);
                         player = CSF_AT(void*, player, 0))
                    {
                        int playerId =
                            CSF_AT(int, CSF_AT(void*, player, 0x08), 0x0C);
                        MpSpawnPoints_CheckPlayer(
                            playerCheck, nullptr, playerId);
                    }
                    complete = MpSpawnPoints_IsPlayerCheckComplete(
                        playerCheck, nullptr);
                }
                entry = CSF_AT(void*, entry, 0);
            }
            else
            {
                bool isSentinel = entry == CSF_AT(void*, self, 0x1468);
                void* next = CSF_AT(void*, entry, 0);
                if (!isSentinel)
                {
                    CSF_AT(void*, CSF_AT(void*, entry, 0x04), 0) = next;
                    CSF_AT(void*, CSF_AT(void*, entry, 0), 0x04) =
                        CSF_AT(void*, entry, 0x04);
                    Mem_FreeDebug(entry);
                    --CSF_AT(uint32_t, self, 0x146C);
                }
                entry = next;
                if (observer)
                {
                    ((void(__fastcall*)(void*, void*, int))
                        CSF_AT(void**, observer, 0)[0])(
                            observer, nullptr, 1);
                }
            }

            if (entry == CSF_AT(void*, self, 0x1468))
                break;
        }

        if (complete)
            g_MpSessionState = 3;
    }

    void* player = CSF_AT(void*, CSF_AT(void*, playerList, 0x04), 0);
    CSF_AT(void*, CSF_AT(void*, playerList, 0x04), 0) =
        CSF_AT(void*, playerList, 0x04);
    CSF_AT(void*, CSF_AT(void*, playerList, 0x04), 0x04) =
        CSF_AT(void*, playerList, 0x04);
    CSF_AT(uint32_t, playerList, 0x08) = 0;

    while (player != CSF_AT(void*, playerList, 0x04))
    {
        void* next = CSF_AT(void*, player, 0);
        Mem_FreeDebug(player);
        player = next;
    }
    if (CSF_AT(void*, playerList, 0x04))
        Mem_FreeDebug(CSF_AT(void*, playerList, 0x04));
}

HOOK(0x00554FF0, MpSpawnPoints_UpdatePlayerChecks, "auto");
