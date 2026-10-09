#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Virtual-call slots on the state object (field_0008->field_0048), byte offset / 4:
//   +0x10 -> [4]    : bool OnQuery(CCharWeaponFsm* self)            (returns al)
//   +0x1C -> [7]    : void OnSetState(CCharWeaponFsm* self, int, int)
//   +0x68 -> [26]   : void* GetStateData()
// Slot +0x8C -> [35] on the owner object (field_0008): int SelectWeaponSlot(int a2, int, int, int)
// The packet's typed wrappers (vSlot8C() / vSlot68(a2)) do not match the arguments this code pushes,
// so the raw vtable slots are called with the exact argument lists from the disassembly.

static int __fastcall CCharWeaponFsm_UpdateWeaponSelection(CCharWeaponFsm* self, void* /*edx*/, int a2)
{
    self->field_0069 = 0;
    self->field_006c = 0.0f;

    void* stateObj = reinterpret_cast<void*>(reinterpret_cast<C_00833908*>(self->field_0008)->field_0048);
    uint8_t queryResult = ((uint8_t(__fastcall*)(void*, void*, CCharWeaponFsm*))(CSF_AT(void**, stateObj, 0)[4]))(stateObj, nullptr, self);
    if (!queryResult) {
        void* stateObjForSet = reinterpret_cast<void*>(reinterpret_cast<C_00833908*>(self->field_0008)->field_0048);
        ((void(__fastcall*)(void*, void*, CCharWeaponFsm*, int, int))(CSF_AT(void**, stateObjForSet, 0)[7]))(stateObjForSet, nullptr, self, 0, 1);
    }

    void* ownerObj = reinterpret_cast<void*>(self->field_0008);
    int slotIndex = ((int(__fastcall*)(void*, void*, int, int, int, int))(CSF_AT(void**, ownerObj, 0)[35]))(ownerObj, nullptr, a2, 0, -1, 0);
    if (slotIndex == -1)
        return -1;

    void* stateObjData = reinterpret_cast<void*>(reinterpret_cast<C_00833908*>(self->field_0008)->field_0048);
    void* stateData = ((void*(__fastcall*)(void*, void*))(CSF_AT(void**, stateObjData, 0)[26]))(stateObjData, nullptr);
    uint8_t needsFlag = CSF_AT(uint8_t, stateData, 0x58);
    if (needsFlag)
        self->field_0069 = 1;

    return slotIndex;
}

HOOK(0x0047F9C0, CCharWeaponFsm_UpdateWeaponSelection, "auto");
