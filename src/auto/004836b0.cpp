#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_004950e0_GetField398, 0x004950E0, (void* self, void*));

// Returns the value from the object held in field_0008; when that is zero,
// falls back to the virtual slot 0x11C with argument 12.
static int __fastcall CCharActionFsm_ResolveActionState(CCharActionFsm* self, void* /*edx*/)
{
    int result = C_004950e0_GetField398(reinterpret_cast<void*>(self->field_0008), nullptr);
    if (result == 0)
    {
        return self->vUpdateWeaponSelection(12);
    }
    return result;
}

HOOK(0x004836B0, CCharActionFsm_ResolveActionState, "auto");
