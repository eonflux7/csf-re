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

// Callee: S_00444180 method taking a pointer to a packed 0x28-byte record (thiscall, 1 stack arg).
CSF_FUNC(int, __fastcall, sub_444180, 0x00444180, (void* self, void* edx, int a2));

// Builds a 10-dword record on the stack (flag byte, C_0082d448 vtable, two words copied from
// the source record at a2, five words copied from the array at a3, trailing zero) and forwards
// it to S_00444180's worker at 0x444180.
static int __fastcall S_00444180_ForwardWithPackedArgs(S_00444180* self, void* /*edx*/, int a2, int* a3)
{
    // Stack record: 0x28 bytes. Room is exactly the frame object at [ebp-0x28].
    alignas(8) uint32_t recordDwords[10];

    int* sourceRecord = reinterpret_cast<int*>(a2);
    recordDwords[2] = static_cast<uint32_t>(CSF_AT(int, sourceRecord, 4));
    recordDwords[3] = static_cast<uint32_t>(CSF_AT(int, sourceRecord, 8));

    recordDwords[4] = static_cast<uint32_t>(a3[0]);
    recordDwords[5] = static_cast<uint32_t>(a3[1]);
    recordDwords[6] = static_cast<uint32_t>(a3[2]);
    recordDwords[7] = static_cast<uint32_t>(a3[3]);
    recordDwords[8] = static_cast<uint32_t>(a3[4]);

    recordDwords[1] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
    recordDwords[9] = 0;
    recordDwords[0] = 1; // LOBYTE(v6[0]) = 1 (flag byte)

    return sub_444180(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(recordDwords)));
}

HOOK(0x00444270, S_00444180_ForwardWithPackedArgs, "auto");
