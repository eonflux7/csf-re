#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, sub_551CF0, 0x00551CF0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, MenuMpAccounts_CreateListSentinel, 0x00552DD0, ());
CSF_FUNC(int, __cdecl, sub_552EF0, 0x00552EF0, (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(bool, __stdcall, sub_554CD0, 0x00554CD0, (float a1, float a2, float a3));
CSF_FUNC(std::uint32_t, __fastcall, AppendDwordCopies_555B70, 0x00555B70, (void* self, void* edx, std::uint32_t* position, std::uint32_t count, std::int32_t* value));
CSF_FUNC(unsigned int, __fastcall, AppendSelectionIndex, 0x005570F0, (void* indices, void* edx, int* index));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char*, __fastcall, sub_6B29A0, 0x006B29A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MenuMpAccounts_FillPlayerList, 0x0073F3B0, (void* self, void* edx, void* list));
CSF_FUNC(int, __fastcall, sub_740B90, 0x00740B90, (void* self, void* edx, char a2, int a3));
CSF_FUNC(char, __fastcall, sub_740F80, 0x00740F80, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, sub_7411A0, 0x007411A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_7453D0, 0x007453D0, (void* self, void* edx, char a2, char a3));
CSF_FUNC(char, __fastcall, sub_7480A0, 0x007480A0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, sub_748130, 0x00748130, (void* self, void* edx, char a2, int a3));
CSF_FUNC(char, __fastcall, sub_7481B0, 0x007481B0, (void* self, void* edx, char a2, int a3));
CSF_FUNC(char, __fastcall, sub_748260, 0x00748260, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, sub_748310, 0x00748310, (void* self, void* edx, char a2, int a3));
CSF_FUNC(int, __fastcall, sub_748340, 0x00748340, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, Table_GetRecord, 0x0075CF00, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(std::uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

static int MpSpawnPoints_CountSpawnRecords(std::uint32_t byteCount)
{
    const std::int64_t product =
        0x2E8BA2E9LL * static_cast<std::int32_t>(byteCount);
    const std::int32_t quotient =
        static_cast<std::int32_t>(product >> 32) >> 3;
    return quotient + static_cast<int>(
        static_cast<std::uint32_t>(quotient) >> 31);
}

static std::uint32_t* __fastcall MpSpawnPoints_UpdatePlayerSpawns(
    void* self, void* /*edx*/, float elapsed)
{
    alignas(4) std::uint8_t playerList[0x1C]{};
    CSF_AT(void*, playerList, 0x04) = MenuMpAccounts_CreateListSentinel();
    CSF_AT(int, playerList, 0x08) = 0;

    void* selection = nullptr;
    if (g_CharacterSelectState8B0694) {
        void* member = CSF_AT(void*,
            reinterpret_cast<void*>(g_CharacterSelectState8B0694), 0x04);
        if (member)
            selection = static_cast<std::uint8_t*>(member) - 776;
    }

    void* selectionInterface = static_cast<std::uint8_t*>(selection) + 776;
    if (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            CSF_AT(void**, selectionInterface, 0)[8 / 4])(
                selectionInterface, nullptr)) {
        if (!CSF_AT(std::uint32_t, selection, 0x350))
            sub_7480A0(selection, nullptr);

        void* accounts = reinterpret_cast<void*>(GetGlobalDword_73C020());
        MenuMpAccounts_FillPlayerList(accounts, nullptr, playerList);

        void* node = CSF_AT(void*,
            CSF_AT(void*, playerList, 0x04), 0);
        while (node != CSF_AT(void*, playerList, 0x04)) {
            void* player = CSF_AT(void*, node, 0x08);
            const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x1498);
            int trackedIndex = -1;
            for (std::uint32_t index = 0; index < count; ++index) {
                if (CSF_AT(void*, CSF_AT(void*, self, 0x1490), 8 * index)
                    == player) {
                    trackedIndex = static_cast<int>(index);
                    break;
                }
            }

            bool attemptSpawn = false;
            if (CSF_AT(std::uint8_t, player, 0x453) & 1) {
                if (trackedIndex == -1) {
                    if (CSF_AT(std::uint32_t, self, 0x1498)
                        >= CSF_AT(std::uint32_t, self, 0x1494)) {
                        sub_551CF0(static_cast<std::uint8_t*>(self) + 0x1490,
                            nullptr, CSF_AT(int, self, 0x149C));
                    }
                    const int oldCount = CSF_AT(int, self, 0x1498);
                    void* entry = static_cast<std::uint8_t*>(
                        CSF_AT(void*, self, 0x1490)) + 8 * oldCount;
                    CSF_AT(int, self, 0x1498) = oldCount + 1;
                    CSF_AT(void*, entry, 0) = player;
                    CSF_AT(float, entry, 0x04) = 25.0f;
                }
            } else if (trackedIndex != -1) {
                if (static_cast<std::uint8_t>(sub_7411A0(
                        CSF_AT(void*, CSF_AT(void*, self, 0x1490),
                            8 * trackedIndex), nullptr))) {
                    const int lastIndex = CSF_AT(int, self, 0x1498) - 1;
                    CSF_AT(int, self, 0x1498) = lastIndex;
                    if (trackedIndex != lastIndex) {
                        auto* destination = static_cast<std::uint8_t*>(
                            CSF_AT(void*, self, 0x1490)) + 8 * trackedIndex;
                        _memcpy(destination, destination + 8,
                            8 * (lastIndex - trackedIndex));
                    }
                } else {
                    CSF_AT(float, CSF_AT(void*, self, 0x1490),
                        8 * trackedIndex + 4) =
                        CSF_AT(float, CSF_AT(void*, self, 0x1490),
                            8 * trackedIndex + 4) - elapsed;
                    if (CSF_AT(float, CSF_AT(void*, self, 0x1490),
                            8 * trackedIndex + 4) <= g_fZero) {
                        const int lastIndex = CSF_AT(int, self, 0x1498) - 1;
                        CSF_AT(int, self, 0x1498) = lastIndex;
                        if (trackedIndex != lastIndex) {
                            auto* destination = static_cast<std::uint8_t*>(
                                CSF_AT(void*, self, 0x1490)) + 8 * trackedIndex;
                            _memcpy(destination, destination + 8,
                                8 * (lastIndex - trackedIndex));
                        }
                        attemptSpawn = true;
                    }
                }
            }

            if (!attemptSpawn) {
                if (static_cast<std::uint8_t>(sub_7411A0(player, nullptr)))
                    attemptSpawn = true;
                else
                    sub_740F80(player, nullptr, 0);
            }

            if (attemptSpawn
                && CSF_AT(std::uint8_t, player, 0x443)
                && CSF_AT(std::uint16_t, player, 0x446)
                && !CSF_AT(std::uint8_t, player, 0x6F1)) {
                void* table = CSF_AT(void*,
                    reinterpret_cast<void*>(GetValue8B0554()), 0x1FD8);
                char team = CSF_AT(char, player, 0x446);
                const int requiredCount = CSF_AT(int,
                    reinterpret_cast<void*>(GetGlobalDword_73C020()), 0x100);

                void* member = CSF_AT(void*,
                    reinterpret_cast<void*>(g_CharacterSelectState8B0694), 0x04);
                bool teamMode = false;
                if (member) {
                    void* owner = static_cast<std::uint8_t*>(member) - 776;
                    if (owner) {
                        const char mode = sub_6B29A0(owner, nullptr)[6];
                        teamMode = mode == 1 || mode == 2 || mode == 3;
                    }
                }

                int recordId;
                int spawnCount;
                if (teamMode) {
                    if (team == 1) {
                        void* record = reinterpret_cast<void*>(
                            Table_GetRecord(table, nullptr,
                                CSF_AT(int, self, 0x13E4)));
                        const std::uint32_t begin =
                            CSF_AT(std::uint32_t, record, 0x38);
                        spawnCount = begin
                            ? MpSpawnPoints_CountSpawnRecords(
                                CSF_AT(std::uint32_t, record, 0x3C) - begin)
                            : 0;
                        recordId = CSF_AT(int, self, 0x13E4);
                    } else {
                        void* record = reinterpret_cast<void*>(
                            Table_GetRecord(table, nullptr,
                                CSF_AT(int, self, 0x13E8)));
                        const std::uint32_t begin =
                            CSF_AT(std::uint32_t, record, 0x38);
                        spawnCount = begin
                            ? static_cast<std::int32_t>(
                                CSF_AT(std::uint32_t, record, 0x3C) - begin) / 44
                            : 0;
                        recordId = CSF_AT(int, self, 0x13E8);
                    }
                } else {
                    void* record = reinterpret_cast<void*>(
                        Table_GetRecord(table, nullptr,
                            CSF_AT(int, self, 0x13EC)));
                    const std::uint32_t begin =
                        CSF_AT(std::uint32_t, record, 0x38);
                    spawnCount = begin
                        ? MpSpawnPoints_CountSpawnRecords(
                            CSF_AT(std::uint32_t, record, 0x3C) - begin)
                        : 0;
                    recordId = CSF_AT(int, self, 0x13EC);
                    team = 1;
                }

                int chosenIndex;
                if (spawnCount >= requiredCount) {
                    S_00553fa0 candidates{};
                    std::memset(&candidates.field_0004, 0, 12);
                    int iterationStorage[0x10 / sizeof(int)]{};
                    std::int32_t spawnIndexStorage[0xC / sizeof(std::int32_t)]{};

                    for (int index = 0; index < spawnCount; ) {
                        if (!sub_748130(selection, nullptr, team, index)) {
                            void* record = reinterpret_cast<void*>(
                                Table_GetRecord(table, nullptr, recordId));
                            auto* position = static_cast<std::uint8_t*>(
                                CSF_AT(void*, record, 0x38)) + 44 * index + 4;
                            if (!sub_554CD0(
                                    CSF_AT(float, position, 0),
                                    CSF_AT(float, position, 4),
                                    CSF_AT(float, position, 8))) {
                                AppendSelectionIndex(
                                    &candidates, nullptr, iterationStorage);
                            }
                        }
                        iterationStorage[0] = ++index;
                    }

                    void* allocation;
                    if (candidates.field_0004
                        && (static_cast<std::int32_t>(
                            candidates.field_0008 - candidates.field_0004) >> 2)) {
                        const int countAvailable = static_cast<std::int32_t>(
                            candidates.field_0008 - candidates.field_0004) >> 2;
                        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
                        const int choice = GetStateValue(state, nullptr, countAvailable);
                        chosenIndex = CSF_AT(int,
                            reinterpret_cast<void*>(candidates.field_0004), 4 * choice);
                        sub_7481B0(selection, nullptr, team, chosenIndex);
                        allocation = reinterpret_cast<void*>(candidates.field_0004);
                    } else {
                        iterationStorage[0] = 0;
                        if (sub_748340(selection, nullptr, team) > 0) {
                            do {
                                spawnIndexStorage[0] = sub_748310(selection, nullptr,
                                    team, iterationStorage[0]);
                                void* record = reinterpret_cast<void*>(
                                    Table_GetRecord(table, nullptr, recordId));
                                auto* position = static_cast<std::uint8_t*>(
                                    CSF_AT(void*, record, 0x38))
                                    + 44 * spawnIndexStorage[0] + 4;
                                if (!sub_554CD0(
                                        CSF_AT(float, position, 0),
                                        CSF_AT(float, position, 4),
                                        CSF_AT(float, position, 8))) {
                                    const std::uint32_t oldEnd = candidates.field_0008;
                                    if (candidates.field_0004
                                        && static_cast<std::uint32_t>(
                                            static_cast<std::int32_t>(
                                                candidates.field_0008
                                                - candidates.field_0004) >> 2)
                                        < static_cast<std::uint32_t>(
                                            static_cast<std::int32_t>(
                                                candidates.field_000c
                                                - candidates.field_0004) >> 2)) {
                                        sub_552EF0(static_cast<int>(oldEnd), 1,
                                            reinterpret_cast<int>(spawnIndexStorage),
                                            reinterpret_cast<int>(&candidates), 0);
                                        candidates.field_0008 = oldEnd + 4;
                                    } else {
                                        AppendDwordCopies_555B70(&candidates, nullptr,
                                            reinterpret_cast<std::uint32_t*>(
                                                candidates.field_0008),
                                            1u, spawnIndexStorage);
                                    }
                                }
                                ++iterationStorage[0];
                            } while (iterationStorage[0]
                                < sub_748340(selection, nullptr, team));
                        }

                        allocation = reinterpret_cast<void*>(candidates.field_0004);
                        if (candidates.field_0004
                            && (static_cast<std::int32_t>(
                                candidates.field_0008 - candidates.field_0004) >> 2)) {
                            chosenIndex = CSF_AT(int,
                                reinterpret_cast<void*>(candidates.field_0004), 0);
                            sub_748260(selection, nullptr, team);
                        } else {
                            void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
                            chosenIndex = GetStateValue(state, nullptr, spawnCount);
                        }
                    }
                    if (allocation)
                        Mem_FreeDebug(allocation);
                    std::memset(&candidates.field_0004, 0, 12);
                } else {
                    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
                    chosenIndex = GetStateValue(state, nullptr, spawnCount);
                }

                void* playerInterface = static_cast<std::uint8_t*>(player) + 0x6B0;
                if (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, playerInterface, 0)[8 / 4])(
                            playerInterface, nullptr)) {
                    sub_740B90(player, nullptr,
                        CSF_AT(char, player, 0x443), chosenIndex);
                } else {
                    sub_7453D0(player, nullptr,
                        CSF_AT(char, player, 0x443),
                        static_cast<char>(chosenIndex));
                }
                sub_740F80(player, nullptr, 1);

                for (std::uint32_t index = 0;
                     index < CSF_AT(std::uint32_t, self, 0x1498); ++index) {
                    if (CSF_AT(void*, CSF_AT(void*, self, 0x1490), 8 * index)
                        == player) {
                        const int lastIndex = CSF_AT(int, self, 0x1498) - 1;
                        CSF_AT(int, self, 0x1498) = lastIndex;
                        if (static_cast<int>(index) != lastIndex) {
                            auto* destination = static_cast<std::uint8_t*>(
                                CSF_AT(void*, self, 0x1490)) + 8 * index;
                            _memcpy(destination, destination + 8,
                                8 * (lastIndex - static_cast<int>(index)));
                        }
                        break;
                    }
                }
            }

            node = CSF_AT(void*, node, 0);
        }
    }

    void* sentinel = CSF_AT(void*, playerList, 0x04);
    void* result = CSF_AT(void*, sentinel, 0);
    CSF_AT(void*, sentinel, 0) = sentinel;
    CSF_AT(void*, CSF_AT(void*, playerList, 0x04), 4) =
        CSF_AT(void*, playerList, 0x04);
    CSF_AT(int, playerList, 0x08) = 0;

    while (result != CSF_AT(void*, playerList, 0x04)) {
        void* next = CSF_AT(void*, result, 0);
        Mem_FreeDebug(result);
        result = next;
    }
    sentinel = CSF_AT(void*, playerList, 0x04);
    if (sentinel)
        return reinterpret_cast<std::uint32_t*>(Mem_FreeDebug(sentinel));
    return static_cast<std::uint32_t*>(result);
}

HOOK(0x00557410, MpSpawnPoints_UpdatePlayerSpawns, "auto");
