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

// Callee: refreshes state after the stored value changes (0x006BFC70, __thiscall, no args).
CSF_FUNC(char, __fastcall, sub_6BFC70, 0x006BFC70, (void* self, void* edx));

// Stores a raw 32-bit value into field_0014 and then calls the refresh routine.
// The original writes the integer bits of a2 into the float slot (LODWORD), so the
// store is done as a 32-bit integer write to the same address.
static char __fastcall C_00860700_SetStoredValueAndUpdate(C_00860700* self, void* /*edx*/, int a2)
{
    *reinterpret_cast<int32_t*>(&self->field_0014) = a2;
    return sub_6BFC70(self, nullptr);
}

HOOK(0x006BFD40, C_00860700_SetStoredValueAndUpdate, "auto");
