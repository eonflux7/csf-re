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

// Callee bindings (by address).
CSF_FUNC(int, __fastcall, sub_4809C0, 0x004809C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_498F50, 0x00498F50, (void* self, void* edx, int a2, char a3));

// Runs the weapon-FSM step; if the step did not change this object's
// field_0028, clears bit 0x200 in the owner's flags word at +0x2E4 and
// calls the owner's helper with (512, 0). Otherwise returns the step result.
static int __fastcall CCharActionFsm_RunStateIfUnchanged(CCharActionFsm* self, void* /*edx*/, int a2)
{
    uint32_t stateBefore = self->field_0028;
    int result = sub_4809C0(self, nullptr, a2);

    if (stateBefore == self->field_0028)
    {
        void* owner = reinterpret_cast<void*>(self->field_0008);
        uint32_t flags = CSF_AT(uint32_t, owner, 0x2E4);
        flags &= ~0x200u;
        CSF_AT(uint32_t, owner, 0x2E4) = flags;

        void* ownerForCall = reinterpret_cast<void*>(self->field_0008);
        return sub_498F50(ownerForCall, nullptr, 512, 0);
    }
    return result;
}

HOOK(0x4830E0, CCharActionFsm_RunStateIfUnchanged, "auto");
