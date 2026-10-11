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

// Retries the object's virtual slot +0x50 up to three times (indices 0, 1, 2).
// Returns true as soon as a call returns something other than -1; false if all three return -1.
static bool __fastcall C_00860bb8_RetrySlot50ThreeTimes(C_00860bb8* self, void* /*edx*/, int a2) {
    int attemptIndex = 0;
    while (self->vSlot50(attemptIndex, reinterpret_cast<char*>(a2)) == -1) {
        if (++attemptIndex >= 3) {
            return false;
        }
    }
    return true;
}

HOOK(0x6CACD0, C_00860bb8_RetrySlot50ThreeTimes, "auto");
