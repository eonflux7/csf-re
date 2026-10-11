#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns 32 when the inner state word at +0x1C4 is 0, 69 when it is 1,
// and otherwise the object's own address (as an int).
static int __fastcall C_00834598_GetStatusCodeFromInnerState(C_00834598* self, void* /*edx*/)
{
    uint8_t* innerState = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0008));
    uint32_t stateValue = CSF_AT(uint32_t, innerState, 0x1c4);

    if (stateValue == 0)
        return 32;
    if (stateValue == 1)
        return 69;
    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x4A45D0, C_00834598_GetStatusCodeFromInnerState, "auto");
