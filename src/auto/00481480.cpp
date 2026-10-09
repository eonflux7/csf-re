#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Substate dispatcher reached by tail jump; forwards (child, a2) unchanged.
CSF_FUNC(char, __fastcall, WeaponFsm_DispatchToSubstate, 0x00476500, (void* self, void* edx, char a2));

// Returns the result of the active substate's handler. When no substate is active
// (field_0024 == -1) it returns the low byte of that index, as the original does.
static char __fastcall CCharWeaponFsm_DispatchToActiveSubstate(CCharWeaponFsm* self, void* /*edx*/, char a2)
{
    uint32_t activeIndex = self->field_0024;
    if (activeIndex == 0xFFFFFFFFu)
        return static_cast<char>(activeIndex);

    uint32_t* substateSlots = reinterpret_cast<uint32_t*>(self->field_0018);
    void* activeSubstate = reinterpret_cast<void*>(substateSlots[activeIndex]);
    return WeaponFsm_DispatchToSubstate(activeSubstate, nullptr, a2);
}

HOOK(0x00481480, CCharWeaponFsm_DispatchToActiveSubstate, "auto");
