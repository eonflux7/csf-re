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

// TCharTask base update (forwarder; the single stack argument is passed through).
CSF_FUNC(void, __fastcall, TCharTask_BaseUpdate, 0x004DB0F0, (void* self, void* edx, int a2));

// Returns the byte-sized "weapon action blocked" flag in al.
CSF_FUNC(int, __fastcall, CharActionFsm_IsWeaponActionBlocked, 0x00492BD0, (void* self, void* edx));

// Looks up a task entry by offset within the char task.
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

// Runs the base update, then if the character's weapon action is blocked,
// resolves the task entry at (field_000C + 0x3C). Otherwise returns the raw
// blocked-check result.
static int __fastcall TCharTask_UpdateThenLookupIfWeaponBlocked(void* self, void* /*edx*/, int a2)
{
    TCharTask_BaseUpdate(self, nullptr, a2);

    // self->field_0008->field_0014 : the char action FSM pointer.
    void* actionFsm = CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14);
    int result = CharActionFsm_IsWeaponActionBlocked(actionFsm, nullptr);

    if (static_cast<uint8_t>(result) != 0) {
        int lookupOffset = CSF_AT(int, self, 0x0C) + 0x3C;
        return CharTask_LookupByOffset(self, nullptr, lookupOffset);
    }
    return result;
}

HOOK(0x0047B5C0, TCharTask_UpdateThenLookupIfWeaponBlocked, "auto");
