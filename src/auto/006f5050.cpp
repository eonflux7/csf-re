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

// Callee: refreshes the particle atomic state after the stored value changes (0x006F4420, __thiscall, no args).
CSF_FUNC(char, __fastcall, RpPrtStd_RefreshAtomicState, 0x006F4420, (void* self, void* edx));

// Stores a 32-bit value into field_0028, then calls the refresh routine and returns its result.
static char __fastcall C_00862748_SetFieldAndRefresh(C_00862748* self, void* /*edx*/, int a2)
{
    self->field_0028 = static_cast<uint32_t>(a2);
    return RpPrtStd_RefreshAtomicState(self, nullptr);
}

HOOK(0x006F5050, C_00862748_SetFieldAndRefresh, "auto");
