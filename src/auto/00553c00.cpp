#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, Table_GetRecord, 0x0075CF00, (void* self, void* edx, int a2));

static uint32_t* __fastcall MpSpawnPoints_GetTeamSpawnPosition(
    void* self, void* /*edx*/, uint32_t* position, int team, uint32_t* orientation)
{
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t z = 0;
    const int spawnIndex = team != 1 && team == 2;

    void* table = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FD8);
    if (CSF_AT(int, self, 0x13F0) != -1)
    {
        void* record = reinterpret_cast<void*>(
            Table_GetRecord(table, nullptr, CSF_AT(int, self, 0x13F0)));
        const uint32_t begin = CSF_AT(uint32_t, record, 0x38);
        if (begin)
        {
            const uint32_t end = CSF_AT(uint32_t, record, 0x3C);
            const int32_t count = static_cast<int32_t>(end - begin) / 44;
            if (static_cast<uint32_t>(count) >= 2)
            {
                const int spawnOffset = 44 * spawnIndex;
                record = reinterpret_cast<void*>(
                    Table_GetRecord(table, nullptr, CSF_AT(int, self, 0x13F0)));
                auto* spawn = reinterpret_cast<uint8_t*>(
                    CSF_AT(uint32_t, record, 0x38)) + spawnOffset;

                x = CSF_AT(uint32_t, spawn, 0x04);
                y = CSF_AT(uint32_t, spawn, 0x08);
                z = CSF_AT(uint32_t, spawn, 0x0C);

                record = reinterpret_cast<void*>(
                    Table_GetRecord(table, nullptr, CSF_AT(int, self, 0x13F0)));
                spawn = reinterpret_cast<uint8_t*>(
                    CSF_AT(uint32_t, record, 0x38)) + spawnOffset;
                *orientation = CSF_AT(uint32_t, spawn, 0x14);
            }
        }
    }

    position[0] = x;
    position[1] = y;
    position[2] = z;
    return position;
}

HOOK(0x00553C00, MpSpawnPoints_GetTeamSpawnPosition, "auto");
