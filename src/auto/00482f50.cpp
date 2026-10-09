#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_unknown_GetLinkedDword, 0x00476080, (void* self, void*));
CSF_FUNC(int, __fastcall, CCharActionFsm_UpdateAction, 0x00480AB0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, Hud_IsSuspended, 0x00495520, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));

static char __fastcall CCharActionFsm_UpdateItemAction(
    CCharActionFsm* self, void* /*edx*/, int action)
{
    char result = static_cast<char>(self->vIsField000cNull());
    if (!result)
        return result;

    if (self->vHasUsableWeapon())
    {
        void* hud = reinterpret_cast<void*>(self->field_0008);
        self->field_00bd = 0;
        self->field_00c0 = 0;
        self->field_005e = Hud_IsSuspended(hud, nullptr);
        CCharActionFsm_UpdateAction(self, nullptr, action);

        void* linkedObject = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
        result = C_unknown_GetLinkedDword(linkedObject, nullptr) != 2;
        self->m_pendingActionFlag = result;
        return result;
    }

    result = static_cast<char>(self->field_00c4);
    if (result)
        return result;

    int item = self->vGetIndexedEntryOrZero();
    if (!item)
        return static_cast<char>(item);

    void* currentItem = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
    int count = GetCurrentItemCount(currentItem, nullptr);
    if (count)
        return static_cast<char>(count);

    self->field_00c4 = 1;
    void* recipient = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
    void** recipientVtable = CSF_AT(void**, recipient, 0);

    void* handle = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
    void* value = reinterpret_cast<void*>(Handle_GetValuePointer(handle, nullptr));

    return reinterpret_cast<char(__fastcall*)(
        void*, void*, uint32_t, int, int, int)>(
            recipientVtable[0x7C / 4])(
                recipient, nullptr, CSF_AT(uint32_t, value, 0x134),
                1000001, 1, 0);
}

HOOK(0x00482F50, CCharActionFsm_UpdateItemAction, "auto");
