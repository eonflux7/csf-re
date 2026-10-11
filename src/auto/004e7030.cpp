#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Writes the result record at a2: result = -1 by default, or 0 with a secondary
// value of 1 when the state object is not blocked and the mode is not 1/2/3 and
// the virtual probe at slot 0x9C succeeds. Returns whether the result is not -1.
static int __fastcall C_0083eb70_ResolveTargetResult(C_0083eb70* self, void* /*edx*/, int a2) {
    uint8_t* result = reinterpret_cast<uint8_t*>(a2);

    CSF_AT(int, result, 0x04) = -1;

    uint8_t* stateObj = reinterpret_cast<uint8_t*>(self->field_0008);
    uint32_t mode = CSF_AT(uint32_t, stateObj, 0x1d8);

    if ((CSF_AT(uint8_t, stateObj, 0x1f0) & 0x40) == 0 &&
        mode != 3 && mode != 2 && mode != 1 &&
        self->vSlot9C(a2 + 0x14, 1)) {
        CSF_AT(int, result, 0x04) = 0;
        CSF_AT(int, result, 0x08) = 1;
    }

    return CSF_AT(int, result, 0x04) != -1;
}

HOOK(0x4E7030, C_0083eb70_ResolveTargetResult, "auto");
