#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(int, __fastcall, CCharWeaponFsm_HandleItemActionFallback, 0x004834C0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CCharActionFsm_CanUseSelectedItem, 0x00487400, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_004950e0_GetField398, 0x004950E0, (void* self, void*));
CSF_FUNC(char, __fastcall, Hud_IsSuspended, 0x00495520, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CharActionFsm_ItemUsesActionParameter, 0x00761040, (int a1));
CSF_FUNC(int, __cdecl, CharActionFsm_ItemAllowsDirectAction, 0x00761130, (int a1));

static char __fastcall CCharActionFsm_RequestItemAction(
    CCharActionFsm* self, void* /*edx*/, uint8_t* actionParameter)
{
    char result = 0;
    C_008319c0* item = nullptr;

    if (self->field_0084 &&
        (result = Hud_IsSuspended(
             reinterpret_cast<void*>(self->field_0008), nullptr)) != 0)
    {
        item = reinterpret_cast<C_008319c0*>(self->field_0084);
    }
    else
    {
        const uint32_t itemIndex = self->field_0090;
        result = static_cast<char>(itemIndex);
        if (itemIndex != static_cast<uint32_t>(-1))
        {
            const uint32_t slotAddress = self->field_0078 + 4 * itemIndex;
            result = static_cast<char>(slotAddress);
            item = CSF_AT(C_008319c0*, reinterpret_cast<void*>(slotAddress), 0);
        }
    }

    if (!item)
        return result;

    result = CCharActionFsm_CanUseSelectedItem(
        self, nullptr, reinterpret_cast<uint32_t*>(item));
    if (!result)
        return result;

    if (self->field_000c == 4)
        self->vSlot1F4(0);

    if (C_004950e0_GetField398(
            reinterpret_cast<void*>(self->field_0008), nullptr) &&
        static_cast<uint8_t>(CharActionFsm_ItemAllowsDirectAction(
            C_008319c0_GetSubObjectField74(item, nullptr))) == 0)
    {
        return static_cast<char>(
            CCharWeaponFsm_HandleItemActionFallback(self, nullptr));
    }

    if (static_cast<uint8_t>(CharActionFsm_ItemUsesActionParameter(
            C_008319c0_GetSubObjectField74(item, nullptr))) != 0)
    {
        self->field_00a8 = *actionParameter;
    }

    return static_cast<char>(self->vSlot134(12, 0));
}

HOOK(0x00487710, CCharActionFsm_RequestItemAction, "auto");
