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

// Callee: follow-up step run after the options value is stored (0x0050AB20, __thiscall, no args).
CSF_FUNC(void, __fastcall, sub_50AB20, 0x0050AB20, (void* self, void* edx));

// Stores the incoming 32-bit value into field_0028 and then calls the follow-up routine.
static void __fastcall S_0050aac0_SetStoredValueAndApply(S_0050aac0* self, void* /*edx*/, int a2)
{
    self->field_0028 = static_cast<uint32_t>(a2);
    sub_50AB20(self, nullptr);
}

HOOK(0x0050B500, S_0050aac0_SetStoredValueAndApply, "auto");
