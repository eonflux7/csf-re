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

// Stores the raw 32-bit value of a2 into field_001c (the original writes the integer
// bits with a plain dword mov), then calls the refresh routine and returns its result.
static char __fastcall C_00860700_SetRawValueAndRefresh(C_00860700* self, void* /*edx*/, int a2)
{
    *reinterpret_cast<int32_t*>(&self->field_001c) = a2;
    return sub_6BFC70(self, nullptr);
}

HOOK(0x006BFD80, C_00860700_SetRawValueAndRefresh, "auto");
