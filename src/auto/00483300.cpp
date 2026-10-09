#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, Hud_IsSuspended, 0x00495520, (void* self, void* edx));
CSF_FUNC(bool, __stdcall, IsStanceChangeBlocked, 0x004831C0, (void* a1));

// Chooses the idle/stance animation for the weapon FSM. Returns 1 when it issued the
// animation request, 0 when the HUD is suspended, the target id is -1, or the blocked check passes.
static char __fastcall CCharActionFsm_TrySelectWeaponStance(CCharActionFsm* self, void* /*edx*/)
{
    if (Hud_IsSuspended(reinterpret_cast<void*>(self->field_0008), nullptr) ||
        self->field_0094 == static_cast<uint32_t>(-1) ||
        IsStanceChangeBlocked(reinterpret_cast<void*>(self->field_009c)))
    {
        return 0;
    }

    if (self->field_009c)
        self->vSlot134(12, 0);
    else
        self->vSlot134(11, 0);

    return 1;
}

HOOK(0x483300, CCharActionFsm_TrySelectWeaponStance, "auto");
