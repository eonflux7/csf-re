#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CCharActionFsm_UpdateSelectedWeaponSlot(CCharActionFsm* self, void* /*edx*/) {
    int result = self->vSlotBC(32);
    if (result == -1) {
        return result;
    }

    uint32_t requestedSlot = static_cast<uint32_t>(result);

    if (requestedSlot == self->field_0024) {
        uint32_t pendingSlot = self->field_0028;
        if (pendingSlot != 0xFFFFFFFFu && pendingSlot != requestedSlot) {
            self->field_0098 = pendingSlot;
        }
        self->field_0028 = 0xFFFFFFFFu;
        return self->vSlot134(0, 0);
    }

    uint32_t heldObject = self->field_009c;
    if (heldObject != 0) {
        self->field_00a0 = heldObject;
        self->field_009c = 0;
        void* owner = reinterpret_cast<void*>(heldObject);
        // vtable slot 0x30 / 4 = 12, called with ecx = owner and one argument (0)
        auto releaseFn = reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, owner, 0)[12]);
        releaseFn(owner, nullptr, 0);
    }

    self->field_0098 = self->field_0024;
    self->field_0028 = requestedSlot;
    return self->vSlot134(1, 0);
}

HOOK(0x4833C0, CCharActionFsm_UpdateSelectedWeaponSlot, "auto");
