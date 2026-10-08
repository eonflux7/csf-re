#include <cstdint>
#include "globals.h"
#include "hook.h"

// Reads a[0]->[+4] (a record pointer), returns it, and when its byte at +1 is zero,
// resets this->[+0x18] to 10 and this->[+0x82] to 0.
// a2 and a3 are unused by the original. Stack args: 3 (ret 12).
static int __fastcall ResolveNestedFlagRecord(void* self, void* /*edx*/,
                                              int a2, int a3, uintptr_t a4) {
    (void)a2;
    (void)a3;

    uintptr_t outerRecord = *reinterpret_cast<uintptr_t*>(a4);
    uintptr_t record = *reinterpret_cast<uintptr_t*>(outerRecord + 4);

    if (*reinterpret_cast<uint8_t*>(record + 1) == 0) {
        CSF_AT(int32_t, self, 0x18) = 10;
        CSF_AT(uint8_t, self, 0x82) = 0;
    }
    return static_cast<int>(record);
}

HOOK(0x0045B030, ResolveNestedFlagRecord, "auto");
