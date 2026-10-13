#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, ComputeSpawnPointBalance, 0x005516C0, (uint16_t total, uint16_t first_deduction, uint16_t second_deduction));
CSF_FUNC(uint32_t*, __cdecl, MenuMpAccounts_CreateListSentinel, 0x00552DD0, ());
CSF_FUNC(int64_t, __fastcall, MpSpawn_CommitSpawnRecord, 0x006472B0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, MpSpawn_FetchSpawnRecord, 0x006483D0, (uint32_t* a1));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MenuMpAccounts_FillPlayerList, 0x0073F3B0, (void* self, void* edx, void* list));
CSF_FUNC(int, __fastcall, GetValueIfNotFlagged, 0x007408C0, (void* self, void*));
CSF_FUNC(int16_t, __fastcall, MpSpawnPoints_GetLoadoutThreshold, 0x00740CB0, (void* self, void* , int index));

static int __fastcall S_00747f20_ComputeStateMode0(
    S_00747f20* self, void* /*edx*/)
{
    alignas(8) uint8_t spawnRecordBytes[0xC];
    uint32_t* spawnRecord = MpSpawn_FetchSpawnRecord(
        reinterpret_cast<uint32_t*>(spawnRecordBytes));
    const uint64_t committedRecord = static_cast<uint64_t>(
        MpSpawn_CommitSpawnRecord(spawnRecord, nullptr));

    bool recordAdvanced = false;
    if (self->field_01ec) {
        const uint32_t committedHigh =
            static_cast<uint32_t>(committedRecord >> 32);
        recordAdvanced =
            self->field_02ec < committedHigh ||
            (self->field_02ec == committedHigh &&
             self->field_02e8 < static_cast<uint32_t>(committedRecord));
    }

    alignas(8) uint8_t playerListBytes[0x10];
    CSF_AT(void*, playerListBytes, 0x4) =
        MenuMpAccounts_CreateListSentinel();
    CSF_AT(uint32_t, playerListBytes, 0x8) = 0;

    const int accounts = GetGlobalDword_73C020();
    MenuMpAccounts_FillPlayerList(
        reinterpret_cast<void*>(accounts), nullptr, playerListBytes);

    int mode = 0;
    int maximumBalance = -30000;
    void* sentinel = CSF_AT(void*, playerListBytes, 0x4);
    void* node = CSF_AT(void*, sentinel, 0x0);

    while (node != sentinel) {
        void* player = CSF_AT(void*, node, 0x8);
        if (player) {
            if (GetValueIfNotFlagged(
                    static_cast<uint8_t*>(player) + 0x6B0, nullptr)) {
                const int16_t secondDeduction =
                    MpSpawnPoints_GetLoadoutThreshold(player, nullptr, 4);
                const int16_t firstDeduction =
                    MpSpawnPoints_GetLoadoutThreshold(player, nullptr, 1);
                const int16_t total =
                    MpSpawnPoints_GetLoadoutThreshold(player, nullptr, 0);
                const int balance = ComputeSpawnPointBalance(
                    static_cast<uint16_t>(total),
                    static_cast<uint16_t>(firstDeduction),
                    static_cast<uint16_t>(secondDeduction));
                if (balance > maximumBalance)
                    maximumBalance = balance;
            }
            sentinel = CSF_AT(void*, playerListBytes, 0x4);
        }
        node = CSF_AT(void*, node, 0x0);
    }

    if (recordAdvanced) {
        mode = 4;
    } else {
        const uint16_t threshold = CSF_AT(uint16_t, self, 0x1FA);
        if (threshold != 0 && maximumBalance >= threshold)
            mode = 4;
    }

    node = CSF_AT(void*, sentinel, 0x0);
    CSF_AT(void*, sentinel, 0x0) = sentinel;
    sentinel = CSF_AT(void*, playerListBytes, 0x4);
    CSF_AT(void*, sentinel, 0x4) = sentinel;
    sentinel = CSF_AT(void*, playerListBytes, 0x4);
    CSF_AT(uint32_t, playerListBytes, 0x8) = 0;

    while (node != sentinel) {
        void* next = CSF_AT(void*, node, 0x0);
        Mem_FreeDebug(node);
        sentinel = CSF_AT(void*, playerListBytes, 0x4);
        node = next;
    }
    if (sentinel)
        Mem_FreeDebug(sentinel);

    return mode;
}

HOOK(0x007489A0, S_00747f20_ComputeStateMode0, "auto");
