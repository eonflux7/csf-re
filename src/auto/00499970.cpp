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

// Callee bindings (by original address)
CSF_FUNC(int, __fastcall, ProcessFlaggedUpdate, 0x004993E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, DispatchToBaseHandler, 0x0048FD00, (void* self, void* edx, int a2));

// Original: 0x499970, __thiscall, ret 4
// If flag 0x2000 (bit in the high byte of field_02e0) is set, run the flagged
// update first, then forward the request to the base handler and return its result.
static int __fastcall CPlayerCharacter_ConditionalFlushThenForward(CPlayerCharacter* self, void* /*edx*/, int a2)
{
    if ((self->field_02e0 & 0x2000) != 0)
        ProcessFlaggedUpdate(self, nullptr);

    return DispatchToBaseHandler(self, nullptr, a2);
}

HOOK(0x499970, CPlayerCharacter_ConditionalFlushThenForward, "auto");
