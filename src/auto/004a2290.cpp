#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CScene_AddObjectFlags, 0x00452800, (CScene* self, void* , int handleAddress, int flags));
CSF_FUNC(int, __fastcall, CScene_RemoveObjectFromCategories, 0x004528D0, (CScene* self, void* , int handleAddress, int categoryMask));
CSF_FUNC(void, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, float heading));
CSF_FUNC(int, __fastcall, HudAlertIcons_SetObjectOrientation, 0x00494B20, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, HudAlertIcons_RefreshObjectOrientation, 0x00494E30, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SetValueAt44, 0x004E26C0, (void* self, void* , int value));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __cdecl, MpSpawn_GetSelectedObjectFlagByte, 0x00551490, ());
CSF_FUNC(bool, __cdecl, MpSpawn_IsCharacterSelectStateActive, 0x005515A0, ());
CSF_FUNC(char, __fastcall, MpSpawnPoints_RebuildSpawnLoadout, 0x005543A0, (void* self, void*));
CSF_FUNC(char, __fastcall, NetCogibleDDL_ApplyRotationSnapshot, 0x007409D0, (void* self, void* , float a3, float a4, float a5));
CSF_FUNC(char, __fastcall, CMenuMpStats_ProcessStatsUpdate, 0x00740A60, (CMenuMpStats* self, void*));
CSF_FUNC(char, __fastcall, CMenuMpStats_StoreValueIfReady, 0x00740A80, (CMenuMpStats* self, void* , int16_t value));
CSF_FUNC(char, __fastcall, MpSpawnPoints_SavePlayerState, 0x00740DE0, (void* self, void* , uint32_t* outPlayerSlot, uint32_t* outSavedValue));
CSF_FUNC(char, __fastcall, MpSpawnPoints_SetPlayerStateFlag, 0x00740F80, (void* self, void* , char value));
CSF_FUNC(void, __fastcall, CMenuMpStats_UpdateAimDirection, 0x00741460, (void* self, void* , int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __fastcall, C_0083e0d8_GetRecordAddress, 0x0075CF00, (C_0083e0d8* self, void* , int recordIndex));
CSF_FUNC(int, __fastcall, WaypointGraph_GetSize, 0x00774C10, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_RadiansToDegrees, 0x0083441C);

static int __fastcall HudAlertIcons_ApplySelectedSpawnPoint(
    void* self, void* /*edx*/, CMenuMpStats* stats, int /*unused*/)
{
    int result = GetGlobalState_50D190();
    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0xD0))
        return result;

    CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x90C) = 0;

    result = CSF_AT(int, self, 0x08);
    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0x3A4))
        return result;

    GetGlobal8B0688();
    result = MpSpawn_GetSelectedObjectFlagByte();
    if (result != 2)
        return result;

    result = GetGlobal8B0688();
    if (CSF_AT(int, reinterpret_cast<void*>(result), 0x13D4) == -1)
        return result;

    CScene_AddObjectFlags(
        static_cast<CScene*>(g_pCurrentScene), nullptr,
        reinterpret_cast<int>(
            CSF_AT(uint8_t*, self, 0x08) + 0x1C),
        1);

    void* object = CSF_AT(void*, self, 0x08);
    const int categoryMask = CSF_AT(int, object, 0x94);
    CScene_RemoveObjectFromCategories(
        static_cast<CScene*>(g_pCurrentScene), nullptr,
        reinterpret_cast<int>(static_cast<uint8_t*>(object) + 0x1C),
        categoryMask);
    CScene_AddObjectFlags(
        static_cast<CScene*>(g_pCurrentScene), nullptr,
        reinterpret_cast<int>(CSF_AT(uint8_t*, self, 0x08) + 0x1C),
        categoryMask);

    alignas(8) uint8_t playerSlotBytes[0x4] = {};
    alignas(8) uint8_t savedIndexBytes[0x4] = {};
    uint32_t& playerSlot = *reinterpret_cast<uint32_t*>(playerSlotBytes);
    uint32_t& savedIndex = *reinterpret_cast<uint32_t*>(savedIndexBytes);

    result = MpSpawnPoints_SavePlayerState(
        stats, nullptr, &playerSlot, &savedIndex);
    if (static_cast<int32_t>(playerSlot) != CSF_AT(int8_t, stats, 0x443))
        return result;

    C_0083e0d8* records = CSF_AT(
        C_0083e0d8*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FD8);

    GetGlobal8B0688();
    int recordIndex;
    if (MpSpawn_IsCharacterSelectStateActive())
    {
        if (CSF_AT(uint8_t, stats, 0x443) == 1)
            recordIndex = CSF_AT(int, self, 0x14);
        else
            recordIndex = CSF_AT(int, self, 0x18);
    }
    else
    {
        recordIndex = CSF_AT(int, self, 0x1C);
    }

    void* record = reinterpret_cast<void*>(
        C_0083e0d8_GetRecordAddress(records, nullptr, recordIndex));
    const uint32_t size = static_cast<uint32_t>(
        WaypointGraph_GetSize(static_cast<uint8_t*>(record) + 0x34, nullptr));
    if (savedIndex >= size)
        savedIndex = 0;

    uint8_t* entry = CSF_AT(uint8_t*, record, 0x38) + 44 * savedIndex;
    const float heading = CSF_AT(float, entry, 0x14) * g_RadiansToDegrees;

    object = CSF_AT(void*, self, 0x08);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, object, 0)[0x50 / 4])(object, nullptr, entry + 4);

    DispatchSceneObjectWithPayload(
        CSF_AT(void*, self, 0x08), nullptr, heading);

    alignas(8) uint8_t orientationBytes[0x8];
    uint32_t* orientation = reinterpret_cast<uint32_t*>(orientationBytes);
    entry = CSF_AT(uint8_t*, record, 0x38) + 44 * savedIndex;
    orientation[0] = CSF_AT(uint32_t, entry, 0x14);
    orientation[1] = 0;
    HudAlertIcons_SetObjectOrientation(
        CSF_AT(void*, self, 0x08), nullptr, orientation);
    HudAlertIcons_RefreshObjectOrientation(
        CSF_AT(void*, self, 0x08), nullptr);

    void* attachment = CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x4C);
    if (attachment)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, attachment, 0)[0x2C / 4])(attachment, nullptr, 1);
    }

    void* controller = CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x38);
    if (controller)
        SetValueAt44(controller, nullptr, 1077936128);

    CMenuMpStats_ProcessStatsUpdate(stats, nullptr);

    entry = CSF_AT(uint8_t*, record, 0x38) + 44 * savedIndex;
    const int z = CSF_AT(int, entry, 0x0C);
    const int y = CSF_AT(int, entry, 0x08);
    const int x = CSF_AT(int, entry, 0x04);
    CMenuMpStats_UpdateAimDirection(stats, nullptr, x, y, z);
    NetCogibleDDL_ApplyRotationSnapshot(stats, nullptr, heading, 0.0f, 0.0f);
    CMenuMpStats_StoreValueIfReady(stats, nullptr, 0);
    MpSpawnPoints_SetPlayerStateFlag(stats, nullptr, 0);

    void* spawnState = reinterpret_cast<void*>(GetGlobal8B0688());
    result = MpSpawnPoints_RebuildSpawnLoadout(spawnState, nullptr);
    CSF_AT(uint8_t, self, 0x11) = 0;
    CSF_AT(uint8_t, self, 0x04) = 0;
    return result;
}

HOOK(0x004A2290, HudAlertIcons_ApplySelectedSpawnPoint, "auto");
