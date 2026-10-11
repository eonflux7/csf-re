#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bind the clamp helper by address (cdecl, 3 args, no stack cleanup).
CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* lowerBound, float* upperBound));

// Sets the stored float at +0xE90 to the given value, then clamps it between
// two constants supplied by the object's virtual getters.
static void __fastcall C_008300c0_SetValueAndClamp(C_008300c0* self, void* /*edx*/, float newValue)
{
    float* storedValue = reinterpret_cast<float*>(&self->field_0e90);
    *storedValue = newValue;

    // Slot 0x24 / 4 = 9: vGetConstantFloat (no args, float result).
    float upperBound = ((float(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x24 / 4])(self, nullptr);

    // Slot 0x20 / 4 = 8: vGetConstant (no args, float result).
    float lowerBound = ((float(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x20 / 4])(self, nullptr);

    ClampFloatToBounds(storedValue, &lowerBound, &upperBound);
}

HOOK(0x00429840, C_008300c0_SetValueAndClamp, "auto");
