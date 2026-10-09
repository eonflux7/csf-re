#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharWeaponFsm_BeginWeaponActionFromParams, 0x00480A30, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, Hud_IsSuspended, 0x00495520, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));

static char __fastcall CCharActionFsm_BeginActionOrRequestItem(
    CCharActionFsm* self, void* /*edx*/, int params)
{
    char result = static_cast<char>(self->vIsField000cNull());
    if (!result)
        return result;

    if (self->vSlot10C()) {
        void* owner = reinterpret_cast<void*>(self->field_0008);
        self->field_00bd = 0;
        self->field_00c0 = 0;
        self->field_005e = Hud_IsSuspended(owner, nullptr);
        return static_cast<char>(
            CCharWeaponFsm_BeginWeaponActionFromParams(self, nullptr, params));
    }

    result = static_cast<char>(self->field_00c4);
    if (result)
        return result;

    int item = self->vSlotD8();
    if (!item)
        return static_cast<char>(item);

    int count = GetCurrentItemCount(
        reinterpret_cast<void*>(self->vSlotD8()), nullptr);
    if (count)
        return static_cast<char>(count);

    self->field_00c4 = 1;
    void* recipient = reinterpret_cast<void*>(self->vSlotD8());
    void** recipientVtable = CSF_AT(void**, recipient, 0);

    void* handle = reinterpret_cast<void*>(self->vSlotD8());
    void* value = reinterpret_cast<void*>(
        Handle_GetValuePointer(handle, nullptr));

    // The three earlier pushes belong to this call, not to the getters.
    return static_cast<char>(
        reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t, int, int, int)>(
            recipientVtable[0x7C / 4])(
                recipient, nullptr, CSF_AT(uint32_t, value, 0x134),
                1000001, 1, 0));
}

HOOK(0x00482E80, CCharActionFsm_BeginActionOrRequestItem, "auto");
