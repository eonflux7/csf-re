#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetOwnerValueAt0x104, 0x00476070, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsMagazineUsable, 0x0070E390, (void* self, void* edx));

// Picks the follow-up weapon state (4 or 3) after checking the current item count
// against the owner value and whether the magazine is usable.
static char __fastcall CCharWeaponFsm_ChooseReloadOrAttackState(CCharWeaponFsm* self, void* /*edx*/, int a2)
{
    (void)a2;

    char result = static_cast<char>(self->field_0069);
    if (result)
    {
        int ammoObjRaw = self->vGetIndexedEntryOrZero();
        void* ammoObj = reinterpret_cast<void*>(static_cast<uintptr_t>(ammoObjRaw));
        ((void(__fastcall*)(void*, void*, int, int))CSF_AT(void**, ammoObj, 0)[0x6C / 4])(
            ammoObj, nullptr, 1, static_cast<int>(self->field_0070));

        C_0086754c* itemSlot = reinterpret_cast<C_0086754c*>(static_cast<uintptr_t>(self->vGetIndexedEntryOrZero()));
        int currentItemCount = GetCurrentItemCount(itemSlot, nullptr);

        int ownerValue = GetOwnerValueAt0x104(
            reinterpret_cast<void*>(static_cast<uintptr_t>(self->vGetIndexedEntryOrZero())), nullptr);
        if (currentItemCount == ownerValue)
            return static_cast<char>(self->vSetStateAndDispatch(4));

        C_008639b0* magazine = reinterpret_cast<C_008639b0*>(static_cast<uintptr_t>(self->vGetIndexedEntryOrZero()));
        if (!IsMagazineUsable(magazine, nullptr))
            return static_cast<char>(self->vSetStateAndDispatch(4));

        return static_cast<char>(self->vSetStateAndDispatch(3));
    }
    return result;
}

HOOK(0x0047FEE0, CCharWeaponFsm_ChooseReloadOrAttackState, "auto");
