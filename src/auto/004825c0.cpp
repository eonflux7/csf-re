#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharActionFsm_SetCharacterFlag, 0x00498F50,
         (void* self, void* edx, int a2, char a3));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40,
         (void* self, void* edx));

static int __fastcall CCharActionFsm_ProcessItemChange(
    CCharActionFsm* self, void* /*edx*/, int /*argument*/)
{
    void* item = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
    bool active =
        ((bool (__fastcall*)(void*, void*))
         CSF_AT(void**, item, 0)[0x2C / 4])(item, nullptr);
    if (!active)
        return 0;

    item = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
    if (CSF_AT(uint32_t, item, 0x70))
        return self->vBeginWeaponStateChange(static_cast<int>(self->field_0064), 0);

    item = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
    if (GetCurrentItemCount(item, nullptr))
        return self->vBeginWeaponStateChange(static_cast<int>(self->field_0064), 0);

    item = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
    if (!((bool (__fastcall*)(void*, void*))
          CSF_AT(void**, item, 0)[0x68 / 4])(item, nullptr))
        return self->vBeginWeaponStateChange(static_cast<int>(self->field_0064), 0);

    uint32_t pendingItem = self->field_0028;
    if (pendingItem != static_cast<uint32_t>(-1))
    {
        if (self->field_0024 != pendingItem)
            return self->vBeginWeaponStateChange(static_cast<int>(self->field_0064), 0);

        void* character = reinterpret_cast<void*>(self->field_0008);
        self->field_0028 = -1;
        CSF_AT(uint32_t, character, 0x2E4) &= ~0x200u;
        CharActionFsm_SetCharacterFlag(
            reinterpret_cast<void*>(self->field_0008), nullptr, 512, 0);
    }

    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x2E4) |= 0x80u;
    return self->vBeginWeaponStateChange(static_cast<int>(self->field_0064), 0);
}

HOOK(0x004825C0, CCharActionFsm_ProcessItemChange, "auto");
