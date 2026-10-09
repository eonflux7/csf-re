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

// Returns true when the sub-task reported by the current child object is done
// (or when there is no child object). The child is fetched through the
// class's slot 0xE4 accessor; its own slot 0x58 is queried for completion.
static int __fastcall CCharWeaponFsm_IsCurrentSubtaskDone(CCharWeaponFsm* self, void* /*edx*/, int a2)
{
    int childValue = self->vLookupEntryOrZero(a2);
    if (childValue == 0) {
        return 1;
    }

    void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(childValue));
    // Child's vtable slot 0x58 (byte offset 88 / 4 = 22); child type is not declared.
    using IsDoneFn = uint8_t(__fastcall*)(void* self, void* edx);
    IsDoneFn isDone = reinterpret_cast<IsDoneFn>(CSF_AT(void**, child, 0)[0x58 / 4]);
    uint8_t reportsDone = isDone(child, nullptr);
    if (reportsDone != 0) {
        return 1;
    }
    return 0;
}

HOOK(0x0047FA40, CCharWeaponFsm_IsCurrentSubtaskDone, "auto");
