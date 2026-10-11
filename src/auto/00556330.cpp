#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __cdecl, MenuMpAccounts_CreateListSentinel, 0x00552DD0, ());
CSF_FUNC(void*, __fastcall, MpSpawnPoints_RefreshStatus, 0x00555E40, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (void* self, void* edx, uint8_t* arg));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MenuMpAccounts_FillPlayerList, 0x0073F3B0, (void* self, void* edx, void* list));
CSF_FUNC(int, __fastcall, GetValueIfNotFlagged, 0x007408C0, (void* self, void*));
CSF_FUNC(int16_t, __fastcall, MpAccounts_GetStatistic, 0x00740CB0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, MenuMpAccounts_GetPlayerName, 0x00741010, (void* self, void* edx));

CSF_GLOBAL(float, g_ScoreboardRefreshInterval, 0x0082DAF0);
CSF_GLOBAL(float, g_LevelScaleFactor, 0x0082E048);
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

// The packet also identifies the direct status-refresh callee as a vtable.
[[maybe_unused]] static const auto g_StatusRefreshTable = CSF_VTBL(0x00555E40);

static void __fastcall MpSpawnPoints_UpdateScoreboard(
    void* self, void* /*edx*/, float elapsed)
{
    auto* object = static_cast<uint8_t*>(self);
    uint8_t* selection = nullptr;
    if (g_CharacterSelectState8B0694) {
        auto* state = reinterpret_cast<void*>(g_CharacterSelectState8B0694);
        auto* member = CSF_AT(uint8_t*, state, 0x4);
        if (member)
            selection = member - 776;
    }

    const int team = CSF_AT(int8_t, selection, 0x300);
    if (team == 1)
        CSF_AT(int, object, 0x13B4) = 1;
    else if (team == 2)
        CSF_AT(int, object, 0x13B4) = 2;
    else
        CSF_AT(int, object, 0x13B4) = -1;

    const uint8_t mode = CSF_AT(uint8_t, selection, 0x5C);
    if (mode == 2 || mode == 1 ||
        CSF_AT(float, object, 0x13C4) < g_LevelScaleFactor) {
        CSF_AT(uint16_t, object, 0x13A8) = CSF_AT(uint16_t, selection, 0x2D6);
        CSF_AT(uint16_t, object, 0x13AA) = CSF_AT(uint16_t, selection, 0x2DC);
        CSF_AT(uint16_t, object, 0x13AC) = CSF_AT(uint16_t, selection, 0x44);
        CSF_AT(uint16_t, object, 0x13AE) = CSF_AT(uint16_t, selection, 0x50);
        CSF_AT(int, object, 0x13B0) = -1;

        alignas(4) uint8_t playerList[0x24] = {};
        CSF_AT(void*, playerList, 0x4) = MenuMpAccounts_CreateListSentinel();
        CSF_AT(int, playerList, 0x8) = 0;
        const int accounts = GetGlobalDword_73C020();
        MenuMpAccounts_FillPlayerList(reinterpret_cast<void*>(accounts), nullptr, playerList);

        const int playerCount = CSF_AT(int, playerList, 0x8);
        if (CSF_AT(int, object, 0x13E0) != playerCount)
            CSF_AT(uint8_t, object, 0xC9C) = 1;
        CSF_AT(int, object, 0x13E0) = playerCount;

        void* node = CSF_AT(void*, CSF_AT(void*, playerList, 0x4), 0);
        int rowCount = 0;
        auto* row = object + 0xCA8;
        while (node != CSF_AT(void*, playerList, 0x4)) {
            auto* player = CSF_AT(uint8_t*, node, 0x8);
            if (player && GetValueIfNotFlagged(player + 0x6B0, nullptr)) {
                void* name = MenuMpAccounts_GetPlayerName(player, nullptr);
                AssignStringRange_488830(row + 4, nullptr, name, 0, 0xFFFFFFFF);

                const int value = GetValueIfNotFlagged(player + 0x6B0, nullptr);
                auto* information = value ? reinterpret_cast<uint8_t*>(value) - 20 : nullptr;
                CSF_AT(int, row, 0x34) = CSF_AT(int, information, 0x54);

                const int playerTeam = CSF_AT(int8_t, player, 0x443);
                const int oldTeam = CSF_AT(int, row, 0);
                CSF_AT(int, row, 0) = playerTeam;
                if (oldTeam != playerTeam)
                    CSF_AT(uint8_t, object, 0xC9C) = 1;

                auto* statistic = row + 0x20;
                for (int index = 0; index < 6; ++index, statistic += 2) {
                    const uint16_t oldValue = CSF_AT(uint16_t, statistic, 0);
                    const uint16_t newValue =
                        static_cast<uint16_t>(MpAccounts_GetStatistic(player, nullptr, index));
                    CSF_AT(uint16_t, statistic, 0) = newValue;
                    if (oldValue != newValue)
                        CSF_AT(uint8_t, object, 0xC9C) = 1;
                }

                const int firstValue = CSF_AT(int, player, 0x74C);
                CSF_AT(int, row, 0x2C) = firstValue;
                CSF_AT(int, row, 0x30) = CSF_AT(int, player, 0x750);
                if (firstValue == -1)
                    CSF_AT(int, row, 0x2C) = 0;
                if (CSF_AT(int, row, 0x30) == -1)
                    CSF_AT(int, row, 0x30) = 0;
            }
            node = CSF_AT(void*, node, 0);
            ++rowCount;
            row += 56;
        }

        if (rowCount < 32) {
            auto* buffer = object + 0xCB0 + 56 * rowCount;
            int remaining = 32 - rowCount;
            do {
                void* text = CSF_AT(uint32_t, buffer, 0x14) < 8u
                    ? buffer : CSF_AT(void*, buffer, 0);
                if (CSF_AT(uint16_t, text, 0)) {
                    CSF_AT(uint8_t, object, 0xC9C) = 1;
                    TextInputSubobject_Init(buffer - 4, nullptr, &g_byte_82E65C);
                }
                buffer += 56;
            } while (--remaining);
        }

        row = object + 0xCA8;
        for (int first = 1; first <= rowCount; ++first, row += 56) {
            auto* other = row + 56;
            for (int remaining = rowCount - first; remaining > 0;
                 --remaining, other += 56) {
                const int score = CSF_AT(uint16_t, row, 0x20)
                    - CSF_AT(uint16_t, row, 0x28)
                    - CSF_AT(uint16_t, row, 0x22);
                const int otherScore = CSF_AT(uint16_t, other, 0x20)
                    - CSF_AT(uint16_t, other, 0x28)
                    - CSF_AT(uint16_t, other, 0x22);

                bool swap = score < otherScore;
                if (score == otherScore) {
                    const int kills = CSF_AT(uint16_t, row, 0x20);
                    const int otherKills = CSF_AT(uint16_t, other, 0x20);
                    swap = kills < otherKills ||
                        (kills == otherKills &&
                         CSF_AT(uint16_t, row, 0x24) > CSF_AT(uint16_t, other, 0x24));
                }
                if (!swap)
                    continue;

                alignas(4) uint8_t saved[56] = {};
                CSF_AT(int, saved, 0) = CSF_AT(int, row, 0);
                CSF_AT(uint16_t, saved, 0x8) = 0;
                CSF_AT(uint32_t, saved, 0x18) = 0;
                CSF_AT(uint32_t, saved, 0x1C) = 7;
                AssignStringRange_488830(saved + 4, nullptr, row + 4, 0, 0xFFFFFFFF);

                const int statisticPair1 = CSF_AT(int, row, 0x24);
                const int statisticPair2 = CSF_AT(int, row, 0x28);
                CSF_AT(int, saved, 0x20) = CSF_AT(int, row, 0x20);
                const int firstValue = CSF_AT(int, row, 0x2C);
                CSF_AT(int, saved, 0x28) = statisticPair2;
                const int information = CSF_AT(int, row, 0x34);
                CSF_AT(int, saved, 0x24) = statisticPair1;
                const int secondValue = CSF_AT(int, row, 0x30);
                CSF_AT(int, saved, 0x2C) = firstValue;
                const int otherTeam = CSF_AT(int, other, 0);
                CSF_AT(int, saved, 0x34) = information;
                CSF_AT(int, saved, 0x30) = secondValue;

                CSF_AT(int, row, 0) = otherTeam;
                AssignStringRange_488830(row + 4, nullptr, other + 4, 0, 0xFFFFFFFF);
                auto* destination = row + 0x20;
                auto* source = other + 0x20;
                for (int count = 6; count; --count, destination += 2, source += 2)
                    CSF_AT(uint16_t, destination, 0) = CSF_AT(uint16_t, source, 0);
                CSF_AT(int, row, 0x2C) = CSF_AT(int, other, 0x2C);
                const int savedTeam = CSF_AT(int, saved, 0);
                CSF_AT(int, row, 0x30) = CSF_AT(int, other, 0x30);
                CSF_AT(int, row, 0x34) = CSF_AT(int, other, 0x34);
                CSF_AT(int, other, 0) = savedTeam;
                AssignStringRange_488830(other + 4, nullptr, saved + 4, 0, 0xFFFFFFFF);

                CSF_AT(int, other, 0x20) = CSF_AT(int, saved, 0x20);
                CSF_AT(int, other, 0x24) = CSF_AT(int, saved, 0x24);
                CSF_AT(int, other, 0x28) = CSF_AT(int, saved, 0x28);
                CSF_AT(int, other, 0x30) = CSF_AT(int, saved, 0x30);
                CSF_AT(int, other, 0x2C) = CSF_AT(int, saved, 0x2C);
                CSF_AT(int, other, 0x34) = CSF_AT(int, saved, 0x34);

                if (CSF_AT(uint32_t, saved, 0x1C) >= 8u &&
                    CSF_AT(void*, saved, 0x8))
                    Mem_FreeDebug(CSF_AT(void*, saved, 0x8));
                CSF_AT(uint32_t, saved, 0x1C) = 7;
                CSF_AT(uint32_t, saved, 0x18) = 0;
                CSF_AT(uint16_t, saved, 0x8) = 0;
            }
        }

        void* sentinel = CSF_AT(void*, playerList, 0x4);
        node = CSF_AT(void*, sentinel, 0);
        CSF_AT(void*, sentinel, 0) = sentinel;
        CSF_AT(void*, CSF_AT(void*, playerList, 0x4), 0x4) =
            CSF_AT(void*, playerList, 0x4);
        CSF_AT(int, playerList, 0x8) = 0;
        while (node != CSF_AT(void*, playerList, 0x4)) {
            void* next = CSF_AT(void*, node, 0);
            Mem_FreeDebug(node);
            node = next;
        }
        if (CSF_AT(void*, playerList, 0x4))
            Mem_FreeDebug(CSF_AT(void*, playerList, 0x4));
    }

    const float timer = elapsed + CSF_AT(float, object, 0x141C);
    CSF_AT(float, object, 0x141C) = timer;
    if (timer > g_ScoreboardRefreshInterval) {
        alignas(4) uint8_t request[0x40] = {};
        CSF_AT(uint32_t, request, 0x20) = 7;
        CSF_AT(uint32_t, request, 0x1C) = 0;
        CSF_AT(uint16_t, request, 0xC) = 0;
        CSF_AT(uint8_t, request, 0x28) = 0;
        TextInputSubobject_Init(request + 8, nullptr, &g_byte_82E65C);
        CSF_AT(int, request, 0x2C) = 0;
        CSF_AT(int, request, 0x30) = -1;
        CSF_AT(int, request, 0x34) = -1;
        CSF_AT(int, request, 0) = -1;
        CSF_AT(int, request, 0x4) = -1;
        CSF_AT(int, request, 0x38) = -1;
        CSF_AT(uint8_t, request, 0x28) = 0;
        CSF_AT(uint32_t, request, 0x24) = 1;
        CSF_AT(int16_t, request, 0x3C) = -19740;
        CSF_AT(uint8_t, request, 0x3E) = 98;
        MpSpawnPoints_RefreshStatus(object, nullptr, reinterpret_cast<int>(request));
        if (CSF_AT(uint32_t, request, 0x20) >= 8u &&
            CSF_AT(void*, request, 0xC))
            Mem_FreeDebug(CSF_AT(void*, request, 0xC));
    }
}

HOOK(0x00556330, MpSpawnPoints_UpdateScoreboard, "auto");
