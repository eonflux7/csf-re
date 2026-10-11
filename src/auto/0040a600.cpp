#include <cstdint>
#include "globals.h"
#include "hook.h"

// Stores a2 at +0, a3 at +4 and +0xC, and clears +8. Returns a3.
// Callee pops 8 bytes (two stack args) per the original `ret 8`.
static int __fastcall StoreValuePairAndClearTail(void* self, void* /*edx*/, int a2, int a3) {
    CSF_AT(int, self, 0x0) = a2;
    CSF_AT(int, self, 0x4) = a3;
    CSF_AT(int, self, 0xC) = a3;
    CSF_AT(int, self, 0x8) = 0;
    return a3;
}

HOOK(0x0040A600, StoreValuePairAndClearTail, "auto");
