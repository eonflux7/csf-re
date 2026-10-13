#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns 1 when the menu's outer state (+0x1E0) is 1 or 2 and the inner state
// (+0x080) is one of 1, 2, 3, 4, 23, 5, 6. Otherwise returns 0.
static int __fastcall C_00833908_IsStandbyReady(C_00833908* self, void* /*edx*/)
{
    int outerState = CSF_AT(int, self, 0x1E0);
    if (outerState != 1 && outerState != 2)
        return 0;

    uint32_t innerState = self->field_0080;
    if (innerState == 1 || innerState == 2 || innerState == 3 || innerState == 4 ||
        innerState == 23 || innerState == 5 || innerState == 6)
        return 1;

    return 0;
}

HOOK(0x4929C0, C_00833908_IsStandbyReady, "auto");
