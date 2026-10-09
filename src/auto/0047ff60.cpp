#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, CharActionFsm_IsWeaponActionBlocked, 0x00492BD0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, IncrementCounterAt3E4, 0x00495150, (void* self, void*));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));

static int __fastcall CCharWeaponFsm_UpdateWeaponReadiness(
    CCharWeaponFsm* self, void* /*edx*/, int /*argument*/)
{
    const uint32_t requestedWeapon = self->field_0028;
    if (requestedWeapon != UINT32_MAX && self->field_0024 != requestedWeapon)
    {
        self->field_0068 = 1;
        return self->vBeginWeaponStateChange(6, 1);
    }

    if (static_cast<int32_t>(self->field_0048) >= 0)
    {
        void* object = reinterpret_cast<void*>(
            CHandleTable_Resolve(
                g_pCurrentScene, nullptr,
                reinterpret_cast<int>(&self->field_0040)));
        if (object && CSF_AT(uint32_t, object, 0x50) == 0)
            IncrementCounterAt3E4(object, nullptr);
    }

    void* inventory = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
    const int ready =
        ((bool(__fastcall*)(void*, void*))
            CSF_AT(void**, inventory, 0)[0x2C / 4])(inventory, nullptr);

    if (!ready)
        return ready;

    inventory = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
    if (GetCurrentItemCount(inventory, nullptr) == 0 ||
        static_cast<uint8_t>(CharActionFsm_IsWeaponActionBlocked(
            reinterpret_cast<void*>(self->field_0008), nullptr)) != 0)
    {
        return self->vBeginWeaponStateChange(0, 0);
    }

    self->vUpdateWeaponSelection(8);
    self->vNotifyWeaponTargetIfBound();
    return self->vDispatchIfFlagged();
}

HOOK(0x0047FF60, CCharWeaponFsm_UpdateWeaponReadiness, "auto");
