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

// Stores the value at +0x2D8 (the menu state) and, when that state is 1,
// moves it to state 2, records the source's value at +8 <= 0 in +0x2CC, and
// returns the pair (eax=2, edx=that flag) packed into a 64-bit result.
static int64_t __fastcall MenuControls_LatchStateFromDelta(void* self, void* /*edx*/, void* source)
{
    if (*CSF_AT(int32_t*, self, 0x2D8) != 1)
        return 0;

    *CSF_AT(int32_t*, self, 0x2C8) = 2;
    int32_t sourceValue = *CSF_AT(int32_t*, source, 8);
    uint32_t lessOrEqualZero = (sourceValue <= 0) ? 1u : 0u;
    *CSF_AT(int32_t*, self, 0x2D8) = 2;
    *CSF_AT(uint32_t*, self, 0x2CC) = lessOrEqualZero;

    return (static_cast<int64_t>(lessOrEqualZero) << 32) | 2u;
}

HOOK(0x542DE0, MenuControls_LatchStateFromDelta, "auto");
