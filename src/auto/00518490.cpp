#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Stores value in the dword at +4 of the object only when it is in the range 0..9
// (signed compare, as in the original). Returns the input value unchanged.
static uint32_t __fastcall NetProfile_SetSlotIfInRange(void* self, void* /*edx*/, uint32_t value) {
    int32_t signedValue = static_cast<int32_t>(value);
    if (signedValue >= 0 && signedValue < 10) {
        CSF_AT(uint32_t, self, 0x04) = value;
    }
    return value;
}

HOOK(0x518490, NetProfile_SetSlotIfInRange, "auto");
