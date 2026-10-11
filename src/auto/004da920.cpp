#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, Vehicle_IsSlotEmptyAndNotLocked, 0x0041D160, (void* self, void*));
CSF_FUNC(int, __fastcall, C_0082e428_InitializeSteeringAlignment, 0x0041DAA0, (C_0082e428* self, void*));

// Reads the vehicle pointer stored at owner+0x4C; if its slot is empty and not locked,
// marks the flag bit 1 in this->field_0018, initializes steering alignment on the vehicle,
// and sets the state at +0x70 to 4. Returns the (char) result of the last call made.
static char __fastcall C_008672e4_TryInitSteeringWhenSlotEmpty(C_008672e4* self, void* /*edx*/)
{
    C_0082e428* vehicle = CSF_AT(C_0082e428*, reinterpret_cast<void*>(self->field_0014), 0x4C);
    char result = Vehicle_IsSlotEmptyAndNotLocked(vehicle, nullptr);
    if (result) {
        self->field_0018 |= 1u;
        result = (char)C_0082e428_InitializeSteeringAlignment(vehicle, nullptr);
        CSF_AT(uint32_t, self, 0x70) = 4;
    }
    return result;
}

HOOK(0x004DA920, C_008672e4_TryInitSteeringWhenSlotEmpty, "auto");
