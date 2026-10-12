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

// Callee: 0x415D70, __thiscall with two stack args (ret 8).
CSF_FUNC(int, __fastcall, C_0082e128_ProcessEntryCheck, 0x00415D70, (void* self, void* edx, int a2, int a3));

// Calls the entry check; if it accepts (low byte of result nonzero), stores the
// entry's id at +8 into field_00bc and clears the two status bytes.
static int __fastcall C_0082e128_StoreEntryIdAndClearFlags(C_0082e128* self, void* /*edx*/, int a2, int a3)
{
    int result = C_0082e128_ProcessEntryCheck(self, nullptr, a2, a3);
    if ((uint8_t)result != 0)
    {
        void* entry = reinterpret_cast<void*>(static_cast<uintptr_t>(a3));
        self->field_00bc = CSF_AT(uint32_t, entry, 0x8);
        self->field_01f2 = 0;
        self->field_01f3 = 0;
    }
    return result;
}

HOOK(0x417DA0, C_0082e128_StoreEntryIdAndClearFlags, "auto");
