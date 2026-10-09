#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original: 0x0047FBA0, __thiscall, ret 4 (a2 is a stack argument that is never read).
static char __fastcall CCharWeaponFsm_ProcessPendingWeaponAction(CCharWeaponFsm* self, void* /*edx*/, int /*a2*/)
{
    char result = (char)self->field_0069;

    if (self->field_0069 != 0)
    {
        uint32_t weapon = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0008), 0x48);
        if (weapon != 0)
        {
            void* weaponObj = reinterpret_cast<void*>(weapon);
            auto weaponReady = reinterpret_cast<int(__fastcall*)(void*, void*, CCharWeaponFsm*)>(
                CSF_AT(void**, weaponObj, 0)[4]);
            result = (char)weaponReady(weaponObj, nullptr, self);

            if (result != 0)
            {
                uint32_t weaponAgain = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0008), 0x48);
                void* weaponAgainObj = reinterpret_cast<void*>(weaponAgain);
                auto weaponAction = reinterpret_cast<int(__fastcall*)(void*, void*, CCharWeaponFsm*)>(
                    CSF_AT(void**, weaponAgainObj, 0)[8]);
                result = (char)weaponAction(weaponAgainObj, nullptr, self);
            }
        }
    }

    if (self->field_0028 != 0xFFFFFFFFu)
    {
        self->field_0068 = 1;
        result = (char)self->vSlot134(6, 1);
    }

    return result;
}

HOOK(0x0047FBA0, CCharWeaponFsm_ProcessPendingWeaponAction, "auto");
