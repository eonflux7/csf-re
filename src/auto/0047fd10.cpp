#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Dispatches the weapon state transition. If the object at field_0008->field_0048
// exists and its vtable slot 7 check passes, it calls the vSlot134 handler with
// field_0064 as the first argument; otherwise it calls it with 0.
static int __fastcall CCharWeaponFsm_DispatchWithReloadCheck(CCharWeaponFsm* self, void* /*edx*/, int /*a2*/)
{
    bool isFlagClear = (self->field_0068 == 0);
    uint32_t reloadHandler = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0008), 0x48);

    if (reloadHandler != 0) {
        void* handlerObj = reinterpret_cast<void*>(static_cast<uintptr_t>(reloadHandler));
        auto checkFn = reinterpret_cast<bool(__fastcall*)(void*, void*, CCharWeaponFsm*, bool, int)>(
            CSF_AT(void**, handlerObj, 0)[0x1c / 4]);
        if (checkFn(handlerObj, nullptr, self, isFlagClear, 1)) {
            return self->vSlot134(self->field_0064, 0);
        }
    }

    return self->vSlot134(0, 0);
}

HOOK(0x0047FD10, CCharWeaponFsm_DispatchWithReloadCheck, "auto");
