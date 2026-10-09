#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_479310, 0x00479310, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_480BB0, 0x00480BB0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));

// Returns non-zero when the action FSM has an active weapon/item state and the
// current item in the slot reported by vSlotEC still holds items.
static char __fastcall CCharActionFsm_IsActionAvailable(CCharActionFsm* self, void* /*edx*/)
{
    char result = self->field_0024 != 0xFFFFFFFFu;
    if (!result)
        return result;

    int weaponState;
    if (self->field_009c) {
        void* owner = reinterpret_cast<void*>(self->field_009c);
        // vtable slot 0x4C -> index 19
        weaponState = ((int(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[19])(owner, nullptr);
    } else {
        weaponState = 0;
    }

    int stateCode = weaponState - 11;
    if (stateCode != 0) {
        if (stateCode != 2)
            return sub_480BB0(self, nullptr);
        return 0;
    }

    int itemSlot = sub_479310(reinterpret_cast<void*>(self->field_009c), nullptr);
    C_0086754c* item = reinterpret_cast<C_0086754c*>(self->vSlotEC(itemSlot));
    if (!item || !GetCurrentItemCount(item, nullptr))
        return 0;

    return result;
}

HOOK(0x00484310, CCharActionFsm_IsActionAvailable, "auto");
