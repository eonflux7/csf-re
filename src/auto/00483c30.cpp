#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound callees and data by original address.
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Resolves a handle stored in the weapon FSM's sub-object (or returns the
// embedded sub-object address when its slot is negative). If the handle
// resolves, increments a counter at +0x48 on the resolved entry's owner.
// The original tail-jumps to a bare `inc [ecx+0x48]; ret` block; that block is
// not a function start, so the increment is written inline here.
static int __fastcall CCharWeaponFsm_ResolveSceneHandleAndBumpCounter(CCharWeaponFsm* self, void* /*edx*/)
{
    C_00833908* weaponData = reinterpret_cast<C_00833908*>(self->field_0008);
    int slotValue = CSF_AT(int, weaponData, 0x2bc);

    // Address of the embedded handle record at +0x2b0 (the fallthrough result).
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(weaponData) + 0x2b0);

    if (slotValue >= 0)
    {
        // Handle is the record address + 4, looked up in the current scene's table.
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result + 4);
        if (result != 0)
        {
            // Owner object lives at +0x34 of the resolved entry; increment its counter at +0x48.
            uint8_t* resolvedEntry = reinterpret_cast<uint8_t*>(result);
            uint8_t* owner = CSF_AT(uint8_t*, resolvedEntry, 0x34);
            CSF_AT(uint32_t, owner, 0x48) += 1;
        }
    }
    return result;
}

HOOK(0x483C30, CCharWeaponFsm_ResolveSceneHandleAndBumpCounter, "auto");
