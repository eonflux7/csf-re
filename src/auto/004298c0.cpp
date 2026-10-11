#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bind the clamp helper by address (cdecl, 3 args, no stack cleanup).
CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* lowerBound, float* upperBound));

// Stores the given value in the field at +0xE94, then clamps that stored value
// between two bounds supplied by the object's virtual getters (slot 0x30 gives the
// upper bound, slot 0x2C gives the lower bound).
static void __fastcall C_008300c0_StoreAndClampToRange(C_008300c0* self, void* /*edx*/, float newValue)
{
    float* storedValue = reinterpret_cast<float*>(&self->field_0e94);
    *storedValue = newValue;

    // Slot 0x30 / 4 = 12: vGetConstantFloat (no args, float result). Result goes into the arg slot.
    newValue = ((float(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x30 / 4])(self, nullptr);

    // Slot 0x2C / 4 = 11: vGetConstant (no args, float result).
    float lowerBound = ((float(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x2C / 4])(self, nullptr);

    ClampFloatToBounds(storedValue, &lowerBound, &newValue);
}

HOOK(0x004298C0, C_008300c0_StoreAndClampToRange, "auto");
