#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Updates the weapon FSM state: queries the current weapon holder through
// vtable slots 0xA8/0x78/0x28/0x34/0x24 on a second object, then refreshes the
// holder (slot 0x3C) and, when field_0071 is set, calls the FSM hook at 0x1EC.
// The original `ret 4` pops the one stack argument (a2, unused here).
static char __fastcall CCharWeaponFsm_UpdateWeaponState(CCharWeaponFsm* self, void* /*edx*/, int /*a2*/)
{
    int holderHandle = self->vSlotD8();
    if (holderHandle == 0)
        return 0;

    bool stateWasAbandoned = false;

    // Weapon object reached through the FSM's slot 0xD8 accessor.
    void* weaponObj = reinterpret_cast<void*>(self->vSlotD8());
    int weaponSubObj = ((int(__fastcall*)(void*, void*))CSF_AT(void**, weaponObj, 0)[42])(weaponObj, nullptr);

    if (weaponSubObj != 0) {
        void* subObj = reinterpret_cast<void*>(weaponSubObj);
        int actionObj = ((int(__fastcall*)(void*, void*))CSF_AT(void**, subObj, 0)[30])(subObj, nullptr);

        if (actionObj != 0) {
            void* action = reinterpret_cast<void*>(actionObj);
            bool canProceed = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, action, 0)[10])(action, nullptr);
            if (canProceed) {
                stateWasAbandoned = !((bool(__fastcall*)(void*, void*))CSF_AT(void**, action, 0)[13])(action, nullptr);
                ((void(__fastcall*)(void*, void*))CSF_AT(void**, action, 0)[9])(action, nullptr);
            }
        }
    }

    void* refreshObj = reinterpret_cast<void*>(self->vSlotD8());
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, refreshObj, 0)[15])(refreshObj, nullptr);

    char result = static_cast<char>(self->field_0071);
    if (self->field_0071 != 0) {
        result = ((char(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[123])(
            self, nullptr, stateWasAbandoned ? 1 : 0);
    }
    return result;
}

HOOK(0x0047FDF0, CCharWeaponFsm_UpdateWeaponState, "auto");
