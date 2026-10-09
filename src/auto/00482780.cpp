#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CCharWeaponFsm_DispatchStateCode, 0x00498F50, (void* self, void* edx, int a2, char a3));

static int __fastcall CCharActionFsm_ClearFlag10UnlessCodeFifteen(CCharActionFsm* self, void* /*edx*/, int code)
{
    int result = 0;

    if (code != 15)
    {
        uint8_t* weaponFsm = reinterpret_cast<uint8_t*>(self->field_0008);
        CSF_AT(uint32_t, weaponFsm, 0x2E4) &= ~0x10u;
        result = CCharWeaponFsm_DispatchStateCode(reinterpret_cast<void*>(self->field_0008), nullptr, 16, 0);
    }

    return result;
}

HOOK(0x00482780, CCharActionFsm_ClearFlag10UnlessCodeFifteen, "auto");
