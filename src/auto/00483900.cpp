#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(int, __fastcall, sub_482DC0, 0x00482DC0, (void* self, void* edx, int a2, float a3, char a4));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(void, __fastcall, sub_525B30, 0x00525B30, (void* self, void* edx, int a3, int a4));

// Pushes the aim/weapon parameters read from the active handle and forwards them to sub_482DC0 when the
// weapon slot is set, then re-announces the owner through sub_525B30 and returns the vSlot11C(7) result.
static int __fastcall CCharActionFsm_UpdateWeaponHandleState(CCharActionFsm* self, void* /*edx*/)
{
    if (self->field_0024 != static_cast<uint32_t>(-1))
    {
        // Handle values at offsets +444 (int) and +448 (float) of the handle's value block.
        void* handleValueA = reinterpret_cast<void*>(Handle_GetValuePointer(
            reinterpret_cast<void*>(self->vSlotD8()), nullptr));
        int handleInt = CSF_AT(int, handleValueA, 444);

        void* handleValueB = reinterpret_cast<void*>(Handle_GetValuePointer(
            reinterpret_cast<void*>(self->vSlotD8()), nullptr));
        float handleFloat = CSF_AT(float, handleValueB, 448);

        C_008319c0* subObject = reinterpret_cast<C_008319c0*>(self->vSlotD8());
        int subObjectField74 = C_008319c0_GetSubObjectField74(subObject, nullptr);

        sub_482DC0(self, nullptr, handleInt, handleFloat, static_cast<char>(subObjectField74 == 2));
    }

    int ownerHandle = static_cast<int>(self->field_0008);
    void* ownerTarget = reinterpret_cast<void*>(Get_008b0678());
    sub_525B30(ownerTarget, nullptr, ownerHandle, 107);

    return self->vSlot11C(7);
}

HOOK(0x00483900, CCharActionFsm_UpdateWeaponHandleState, "auto");
