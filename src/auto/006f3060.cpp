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

// Callee: refreshes derived state after the stored value changes (0x006F2F00, __thiscall, no args, returns char).
CSF_FUNC(char, __fastcall, sub_6F2F00, 0x006F2F00, (void* self, void* edx));

// Stores a 32-bit value into field_0024, then refreshes state and returns the refresh result.
// The original is __thiscall with one stack argument (ret 4).
static char __fastcall C_00862640_SetValueAndRefresh(C_00862640* self, void* /*edx*/, int a2)
{
    self->field_0024 = static_cast<uint32_t>(a2);
    return sub_6F2F00(self, nullptr);
}

HOOK(0x006F3060, C_00862640_SetValueAndRefresh, "auto");
