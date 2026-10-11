#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, MpSpawnPoints_GetPlayerIdentifier, 0x004A1980, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void*, __cdecl, MenuMpAccounts_CreateListSentinel, 0x00552DD0, ());
CSF_FUNC(int, __stdcall, MpSpawnPoints_UpdatePlayerAccount, 0x00552FB0, (int a1));
CSF_FUNC(char, __stdcall, MpSpawnPoints_UpdatePlayerStatus, 0x005531A0, (int a1));
CSF_FUNC(int, __stdcall, MpSpawnPoints_UpdatePlayerSelection, 0x00553270, (int a1));
CSF_FUNC(void*, __fastcall, MpSpawnPoints_SendPlayerEvent, 0x005567F0, (void* self, void* edx, int a2, int a3, int a4, int a5));
CSF_FUNC(char, __fastcall, MpSpawnPoints_ProcessSpawnRequests, 0x00556CF0, (void* self, void* edx, char a2));
CSF_FUNC(void, __fastcall, MpSpawnPoints_UpdateSpawnState, 0x00556E80, (void* self, void* edx));
CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char*, __fastcall, MpSpawnPoints_GetCharacterSelectionData, 0x006B29A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MenuMpAccounts_FillPlayerList, 0x0073F3B0, (void* self, void* edx, void* list));
CSF_FUNC(int, __fastcall, GetValueIfNotFlagged, 0x007408C0, (void* self, void*));
CSF_FUNC(uint32_t*, __cdecl, MpSpawnPoints_ProcessPlayerList, 0x00744010, (int a1));

CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

static void** __fastcall S_00555e40_UpdatePlayersAndSpawnState(
    S_00555e40* self, void* /*edx*/)
{
    const uint32_t countdown = self->field_14a8;
    if (countdown)
    {
        self->field_14a8 = countdown - 1;
        void* manager = reinterpret_cast<void*>(GetGlobalDword_73C020());
        const uint8_t request = CSF_AT(uint8_t, manager, 0x14E);
        CSF_AT(uint8_t, manager, 0x14E) = 0;
        self->field_14a0 = request;
        MpSpawnPoints_ProcessSpawnRequests(self, nullptr, 0);
        self->field_14a0 = 0;
    }
    else
    {
        void* manager = reinterpret_cast<void*>(GetGlobalDword_73C020());
        const uint8_t request = CSF_AT(uint8_t, manager, 0x14E);
        CSF_AT(uint8_t, manager, 0x14E) = 0;
        const uint8_t pending = request | self->field_14a1;
        self->field_14a0 = request;
        self->field_14a1 = pending;
        MpSpawnPoints_ProcessSpawnRequests(self, nullptr, 1);
    }

    MpSpawnPoints_UpdateSpawnState(self, nullptr);

    uint32_t playerList[3] = {};
    playerList[1] = reinterpret_cast<uint32_t>(MenuMpAccounts_CreateListSentinel());
    playerList[2] = 0;
    void* manager = reinterpret_cast<void*>(GetGlobalDword_73C020());
    MenuMpAccounts_FillPlayerList(manager, nullptr, playerList);

    void** node = CSF_AT(void**, reinterpret_cast<void*>(playerList[1]), 0);
    bool selectionChanged = false;
    while (node != reinterpret_cast<void**>(playerList[1]))
    {
        C_00858180* player = CSF_AT(C_00858180*, node, 0x8);
        if (player && GetValueIfNotFlagged(
                          reinterpret_cast<uint8_t*>(player) + 0x6B0, nullptr))
        {
            MpSpawnPoints_UpdatePlayerAccount(reinterpret_cast<int>(player));
            MpSpawnPoints_UpdatePlayerSelection(reinterpret_cast<int>(player));
            MpSpawnPoints_UpdatePlayerStatus(reinterpret_cast<int>(player));

            const uint8_t selectionRequest = CSF_AT(uint8_t, player, 0x724);
            CSF_AT(uint8_t, player, 0x724) = 0;
            if (selectionRequest)
            {
                bool allowSelection = true;
                const uint32_t state = g_CharacterSelectState8B0694;
                if (state)
                {
                    const uint32_t selection = CSF_AT(
                        uint32_t, reinterpret_cast<void*>(state), 0x4);
                    if (selection && selection != 776)
                    {
                        char* data = MpSpawnPoints_GetCharacterSelectionData(
                            reinterpret_cast<void*>(selection - 776), nullptr);
                        allowSelection = CSF_AT(int8_t, data, 0x6) != 0;
                    }
                }

                if (allowSelection)
                {
                    const int selectionType = CSF_AT(int8_t, player, 0x443);
                    selectionChanged = true;
                    switch (selectionType)
                    {
                    case 0:
                    {
                        uint32_t identifierStorage = 0;
                        uint32_t* identifier = MpSpawnPoints_GetPlayerIdentifier(
                            player, nullptr, &identifierStorage);
                        MpSpawnPoints_SendPlayerEvent(
                            self, nullptr, 1398353973,
                            static_cast<int>(*identifier), -1, -1);
                        break;
                    }
                    case 1:
                    {
                        uint32_t identifierStorage = 0;
                        uint32_t* identifier = MpSpawnPoints_GetPlayerIdentifier(
                            player, nullptr, &identifierStorage);
                        MpSpawnPoints_SendPlayerEvent(
                            self, nullptr, 1398353971,
                            static_cast<int>(*identifier), -1, -1);
                        break;
                    }
                    case 2:
                    {
                        uint32_t identifierStorage[3] = {};
                        uint32_t* identifier = MpSpawnPoints_GetPlayerIdentifier(
                            player, nullptr, identifierStorage);
                        MpSpawnPoints_SendPlayerEvent(
                            self, nullptr, 1398353972,
                            static_cast<int>(*identifier), -1, -1);
                        break;
                    }
                    }
                }
            }

            const uint8_t statusRequest = CSF_AT(uint8_t, player, 0x748);
            CSF_AT(uint8_t, player, 0x748) = 0;
            if (statusRequest)
            {
                int identifier = static_cast<int>(player->field_002c);
                if ((identifier & 0x3FFFFF) == 0)
                {
                    Game_RaiseStatusNotice(0, 0, -536870898, 0);
                    identifier = 0;
                }
                MpSpawnPoints_SendPlayerEvent(
                    self, nullptr, 1398353974, identifier, -1, -1);
            }
        }
        node = CSF_AT(void**, node, 0);
    }

    uint32_t context[4] = {};
    const uint32_t owner = self->field_0c8c;
    if (owner)
    {
        const uint32_t embedded = CSF_AT(
            uint32_t, reinterpret_cast<void*>(owner), 0x18);
        context[0] = embedded ? embedded - 1712 : 0;
    }
    context[1] = reinterpret_cast<uint32_t>(playerList);
    context[2] = self->field_1484;

    int selectionStatus = -1;
    const uint32_t state = g_CharacterSelectState8B0694;
    if (state)
    {
        const uint32_t selection = CSF_AT(
            uint32_t, reinterpret_cast<void*>(state), 0x4);
        if (selection && selection != 776)
        {
            char* data = MpSpawnPoints_GetCharacterSelectionData(
                reinterpret_cast<void*>(selection - 776), nullptr);
            selectionStatus = CSF_AT(int8_t, data, 0x6);
        }
    }
    CSF_AT(uint8_t, context, 0xC) = selectionStatus != 0;

    bool resetSelection = selectionChanged;
    if (!resetSelection)
    {
        void* selectionObject = reinterpret_cast<void*>(self->field_1484);
        if (selectionObject)
        {
            resetSelection =
                reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, selectionObject, 0)[56 / 4])(
                        selectionObject, nullptr);
        }
    }
    if (resetSelection)
    {
        void* selectionObject = reinterpret_cast<void*>(self->field_1484);
        if (selectionObject)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, selectionObject, 0)[52 / 4])(
                    selectionObject, nullptr, 0);
        }
    }

    MpSpawnPoints_ProcessPlayerList(reinterpret_cast<int>(context));

    void** result = CSF_AT(void**, reinterpret_cast<void*>(playerList[1]), 0);
    CSF_AT(void*, reinterpret_cast<void*>(playerList[1]), 0) =
        reinterpret_cast<void*>(playerList[1]);
    CSF_AT(void*, reinterpret_cast<void*>(playerList[1]), 0x4) =
        reinterpret_cast<void*>(playerList[1]);
    void* sentinel = reinterpret_cast<void*>(playerList[1]);
    playerList[2] = 0;

    while (result != sentinel)
    {
        void** next = CSF_AT(void**, result, 0);
        Mem_FreeDebug(result);
        sentinel = reinterpret_cast<void*>(playerList[1]);
        result = next;
    }

    if (sentinel)
        return reinterpret_cast<void**>(Mem_FreeDebug(sentinel));
    return result;
}

HOOK(0x00557160, S_00555e40_UpdatePlayersAndSpawnState, "auto");
