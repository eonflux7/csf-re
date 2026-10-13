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

// Callee: refreshes state after the stored value changes (0x0050AAC0, __thiscall, no args).
CSF_FUNC(int, __fastcall, sub_50AAC0, 0x0050AAC0, (void* self, void* edx));

// Stores a 32-bit value into field_001c and then calls the refresh routine.
// The original is __thiscall with one stack argument (ret 4).
static int __fastcall S_0050aac0_SetFieldAndRefresh(S_0050aac0* self, void* /*edx*/, int a2)
{
    self->field_001c = static_cast<uint32_t>(a2);
    return sub_50AAC0(self, nullptr);
}

HOOK(0x0050B4E0, S_0050aac0_SetFieldAndRefresh, "auto");
