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
#include "legacy/string.h"

// Calls the weapon-state object returned by slot 0xE4 (with a2 as the argument),
// then invokes that object's slot 0x58 with no arguments and returns its result.
// Returns 0 when slot 0xE4 yields nothing.
static int __fastcall CCharWeaponFsm_ActivateWeaponFromSlotE4(CCharWeaponFsm* self, void* /*edx*/, int a2)
{
    int resolved = self->vLookupEntryOrZero(a2);
    if (resolved != 0)
    {
        // Slot 0x58 takes only `this` here: the original pushes no argument before this call.
        auto slot58 = reinterpret_cast<int (__fastcall*)(void*, void*)>(CSF_AT(void**, resolved, 0)[0x58 / 4]);
        return slot58(reinterpret_cast<void*>(resolved), nullptr);
    }
    return 0;
}

HOOK(0x0047FA80, CCharWeaponFsm_ActivateWeaponFromSlotE4, "auto");
