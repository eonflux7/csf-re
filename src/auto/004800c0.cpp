#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Begins a new weapon action: calls the slot-0x13C hook with the new id, clears the
// progress float and counter, records the id, flags the FSM as active, and stores a
// parameter (or 0 when the id is unchanged). Then returns the result of the slot-0x140 step.
static int __fastcall CCharWeaponFsm_BeginWeaponStateChange(CCharWeaponFsm* self, void* /*edx*/, int newWeaponId, int stateParam)
{
    self->vDispatchWeaponAction(newWeaponId);
    self->field_006c = 0.0f;
    self->field_0010 = 0;
    const bool sameAsCurrent = (newWeaponId == (int)self->field_0064);
    self->field_0069 = 1;
    self->m_stateCode = (uint32_t)newWeaponId;
    self->field_0064 = sameAsCurrent ? 0u : (uint32_t)stateParam;
    return self->vDispatchStateVirtual();
}

HOOK(0x4800C0, CCharWeaponFsm_BeginWeaponStateChange, "auto");
