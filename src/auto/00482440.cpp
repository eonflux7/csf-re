#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_unknown_GetLinkedDword, 0x00476080, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_480250, 0x00480250, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, sub_4950F0, 0x004950F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_498F50, 0x00498F50, (void* self, void* edx, int a2, char a3));

static int __fastcall CCharActionFsm_RefreshWeaponState(CCharActionFsm* self, void* /*edx*/)
{
    if (self->vGetIndexedEntryOrZero())
    {
        C_008319c0* linkedObj = reinterpret_cast<C_008319c0*>(self->vGetIndexedEntryOrZero());
        if (C_unknown_GetLinkedDword(linkedObj, nullptr) == 2)
            CSF_AT(uint8_t, reinterpret_cast<void*>(self->vGetIndexedEntryOrZero()), 0x16) = 1;
    }

    if (!sub_4950F0(reinterpret_cast<void*>(self->field_0008), nullptr))
    {
        void* weaponObj = reinterpret_cast<void*>(self->field_0008);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, weaponObj, 0)[27])(weaponObj, nullptr, 0);
    }

    void* actionObj = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
    int stateCode = ((int(__fastcall*)(void*, void*))CSF_AT(void**, actionObj, 0)[18])(actionObj, nullptr);
    if (stateCode > 0 && stateCode <= 3)
        sub_498F50(reinterpret_cast<void*>(self->field_0008), nullptr, 16, 1);

    return sub_480250(self, nullptr);
}

HOOK(0x00482440, CCharActionFsm_RefreshWeaponState, "auto");
