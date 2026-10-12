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

// Stores the raw 32-bit value a2 into field_0018 (the original writes the integer bits
// into the float slot), then calls the refresh routine and returns its result unchanged.
static char __fastcall C_00860700_StoreRawValueAndRefresh(C_00860700* self, void* /*edx*/, int a2)
{
    *reinterpret_cast<int32_t*>(&self->field_0018) = a2;
    return sub_6BFC70(self, nullptr);
}

HOOK(0x006BFD60, C_00860700_StoreRawValueAndRefresh, "auto");
