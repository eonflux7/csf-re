#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// __thiscall with one stack argument (ret 4). Increments the signed 16-bit
// counter at this+0x208 when it is non-negative and returns the counter value.
static int __fastcall IncrementControlActionCounter(void* self, void* /*edx*/, int /*a2*/) {
    int16_t result = CSF_AT(int16_t, self, 0x208);
    if (result >= 0) {
        CSF_AT(int16_t, self, 0x208) = ++result;
    }
    // The original zeroes eax and loads ax, so the 16-bit value is zero-extended.
    return static_cast<int>(static_cast<uint16_t>(result));
}

HOOK(0x4921D0, IncrementControlActionCounter, "auto");
