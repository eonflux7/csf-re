#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Park-Miller minimal-standard LCG step (multiplier 48271, modulus 2^31-1 via
// Schrage's method: 2^31-1 = 44488*48271 + 3399). The seed lives at +0xC4 of the
// object passed in ecx. Result is written back and returned.
static int __fastcall GlobalState_AdvanceRandomSeed(void* self, void* /*edx*/) {
    int32_t seed = CSF_AT(int32_t, self, 0xC4);
    int32_t quotient = seed / 44488;
    int32_t remainder = seed % 44488;
    int32_t next = 48271 * remainder - 3399 * quotient;
    CSF_AT(int32_t, self, 0xC4) = next;
    if (next <= 0) {
        CSF_AT(int32_t, self, 0xC4) = next + 0x7FFFFFFF;
    }
    return CSF_AT(int32_t, self, 0xC4);
}

HOOK(0x50DCF0, GlobalState_AdvanceRandomSeed, "auto");
