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

// Original 0x00519510: one stack argument (a2), ecx = this, edx = a2. Returns in eax; ret 4.
CSF_FUNC(unsigned int, __fastcall, C_00840cf8_ProcessFlaggedEntry, 0x00519510,
         (void* self, void* edx, int a2));

// __thiscall(this, a2, a3) with ret 8: if the byte at a3+0x1D is nonzero, forward
// to ProcessFlaggedEntry (ecx = this, edx = a2, stack = a2); otherwise return a3.
static unsigned int __fastcall C_00840cf8_ForwardWhenFlagSet(C_00840cf8* self, void* /*edx*/,
                                                             int a2, unsigned int a3) {
    unsigned int result = a3;
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(static_cast<uintptr_t>(a3)), 0x1D)) {
        return C_00840cf8_ProcessFlaggedEntry(self,
                                              reinterpret_cast<void*>(static_cast<uintptr_t>(a2)),
                                              a2);
    }
    return result;
}

HOOK(0x519620, C_00840cf8_ForwardWhenFlagSet, "auto");
