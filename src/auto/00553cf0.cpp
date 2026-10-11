#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));
CSF_FUNC(char*, __fastcall, MpSpawnPoints_GetSelectionData, 0x006B29A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Table_GetRecord, 0x0075CF00, (void* self, void* edx, int a2));

CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

static uint32_t* __fastcall S_00552b80_GetRandomSpawnPoint(
    S_00552b80* self, void* /*edx*/,
    uint32_t* position, uint32_t* spawnValue, uint32_t* spawnFlags)
{
    auto* table = CSF_AT(
        C_0083e0d8*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FD8);

    const uint32_t selection = CSF_AT(
        uint32_t, reinterpret_cast<void*>(g_CharacterSelectState8B0694), 0x4);

    bool useTeamSpawn = false;
    if (selection && selection != 776)
    {
        char* selectionData = MpSpawnPoints_GetSelectionData(
            reinterpret_cast<void*>(selection - 776), nullptr);
        const char selectionType = CSF_AT(char, selectionData, 0x6);
        useTeamSpawn =
            selectionType == 1 || selectionType == 2 || selectionType == 3;
    }

    int32_t recordIndex;
    if (useTeamSpawn)
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        if (GetStateValue(state, nullptr, 1) == 1)
            recordIndex = static_cast<int32_t>(self->field_13e4);
        else
            recordIndex = static_cast<int32_t>(self->field_13e8);
    }
    else
    {
        recordIndex = static_cast<int32_t>(self->field_13ec);
    }

    if (recordIndex < 0)
    {
        position[0] = 0;
        position[1] = 0;
        position[2] = 0;
        *spawnValue = 0;
        *spawnFlags = 0;
        return spawnValue;
    }

    void* record = reinterpret_cast<void*>(
        Table_GetRecord(table, nullptr, recordIndex));

    const uint32_t entriesBegin = CSF_AT(uint32_t, record, 0x38);
    int entryCount = 0;
    if (entriesBegin)
    {
        entryCount = static_cast<int32_t>(
            CSF_AT(uint32_t, record, 0x3C) - entriesBegin) / 44;
    }

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    const uint32_t entryOffset =
        44 * static_cast<uint32_t>(GetStateValue(state, nullptr, entryCount));

    void* entryPosition = reinterpret_cast<void*>(
        CSF_AT(uint32_t, record, 0x38) + entryOffset + 4);
    position[0] = CSF_AT(uint32_t, entryPosition, 0x0);
    position[1] = CSF_AT(uint32_t, entryPosition, 0x4);
    position[2] = CSF_AT(uint32_t, entryPosition, 0x8);

    *spawnValue = CSF_AT(
        uint32_t,
        reinterpret_cast<void*>(CSF_AT(uint32_t, record, 0x38) + entryOffset),
        0x10);
    *spawnFlags = CSF_AT(
        uint32_t,
        reinterpret_cast<void*>(CSF_AT(uint32_t, record, 0x38) + entryOffset),
        0x14);
    return spawnFlags;
}

HOOK(0x00553CF0, S_00552b80_GetRandomSpawnPoint, "auto");
