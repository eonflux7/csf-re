#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Default handler for states other than 27; tail-jumped to with the same args.
CSF_FUNC(int, __fastcall, CharActionFsm_DispatchDefault, 0x0047FD60, (void* self, void* edx, int a2));

static char __fastcall CCharActionFsm_ProcessStateTwentySeven(CCharActionFsm* self, void* /*edx*/, int a2)
{
    if (self->field_0010 != 0x1b)
        return (char)CharActionFsm_DispatchDefault(self, nullptr, a2);

    char result = (char)self->field_0069;
    if (result)
        return (char)self->vBeginWeaponStateChange(0, 0);
    return result;
}

HOOK(0x00484160, CCharActionFsm_ProcessStateTwentySeven, "auto");
