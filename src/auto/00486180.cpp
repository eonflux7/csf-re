#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __fastcall, CCharWeaponFsm_PrepareTargetAction, 0x00482AE0, (void* self, void* edx, void* a3, float a4));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(void, __fastcall, CharacterEvents_SendEvent, 0x00525B30, (void* self, void* edx, int a3, int a4));

static int __fastcall CCharWeaponFsm_StartWeaponInteraction(
    CCharWeaponFsm* self, void* /*edx*/)
{
    alignas(4) uint8_t record[0x3C]{};
    int action = -1;
    CSF_AT(int, record, 0x34) = -1;
    CSF_AT(float, record, 0x38) = 0.0f;

    int weaponType = 0;
    if (self->field_0024 != static_cast<uint32_t>(-1))
    {
        void* weapon = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero_37());
        weaponType =
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, weapon, 0)[0x50 / 4])(weapon, nullptr);
    }

    switch (weaponType)
    {
    case 5:
        action = 28;
        CSF_AT(int, record, 0x34) = 31;
        CSF_AT(float, record, 0x38) = 72.879997f;
        break;
    case 6:
        action = 29;
        CSF_AT(int, record, 0x34) = 32;
        CSF_AT(float, record, 0x38) = 72.879997f;
        break;
    case 7:
        action = 30;
        CSF_AT(int, record, 0x34) = 33;
        CSF_AT(float, record, 0x38) = 72.879997f;
        break;
    case 8:
        action = 40;
        CSF_AT(int, record, 0x34) = 41;
        CSF_AT(float, record, 0x38) = 72.879997f;
        break;
    default:
        break;
    }

    void* target = reinterpret_cast<void*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(&self->field_0040)));

    if (action == -1 || !target)
        return self->vBeginWeaponStateChange(0, 0);

    CCharWeaponFsm_PrepareTargetAction(
        self, nullptr, target, CSF_AT(float, record, 0x38));

    const int character = static_cast<int>(self->field_0008);
    void* eventManager = reinterpret_cast<void*>(Get_008b0678());
    CharacterEvents_SendEvent(eventManager, nullptr, character, 102);

    CSF_AT(int, record, 0x0C) = -1;
    CSF_AT(int, record, 0x10) = -1;
    CSF_AT(int, record, 0x18) = -1;
    CSF_AT(int, record, 0x1C) = -1;

    auto* characterHandle = reinterpret_cast<C_00833908*>(
        reinterpret_cast<uint8_t*>(self->field_0008) + 0x1C);
    CSF_AT(uint32_t, self, 0xAC) = 0;

    CSF_AT(void**, record, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(void**, record, 0x14) = CSF_VTBL(0x0082D448);
    CSF_AT(int, record, 0x20) = 0;
    CSF_AT(int, record, 0x24) = 0;
    CSF_AT(int, record, 0x2C) = 0;
    CSF_AT(int, record, 0x28) = 0;
    CSF_AT(char, record, 0x30) = 0;
    CSF_AT(void**, record, 0x00) = CSF_VTBL(0x0082D478);
    CSF_AT(int, record, 0x04) = 35;

    CSF_AT(uint32_t, record, 0x0C) = characterHandle->field_0004;
    const uint32_t characterHandleSecond =
        CSF_AT(uint32_t, characterHandle, 0x08);
    const uint32_t targetHandleFirst = CSF_AT(uint32_t, target, 0x20);
    CSF_AT(uint32_t, record, 0x10) = characterHandleSecond;
    const uint32_t targetHandleSecond = CSF_AT(uint32_t, target, 0x24);
    CSF_AT(uint32_t, record, 0x18) = targetHandleFirst;
    CSF_AT(uint32_t, record, 0x1C) = targetHandleSecond;

    void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
    DispatchRecordToGlobalObject(dispatcher, nullptr, record, 0);

    const int actionParameter = self->vUpdateWeaponSelection(action);
    return reinterpret_cast<int (__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, target, 0)[0x8C / 4])(
            target, nullptr, CSF_AT(int, record, 0x34),
            0, actionParameter, 0);
}

HOOK(0x00486180, CCharWeaponFsm_StartWeaponInteraction, "auto");
