#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns true when the signed 32-bit value at +0x18 of the object referenced
// by this->field_00d4 is non-negative (the original uses setge, a signed compare).
static bool __fastcall CCharActionFsm_IsSubObjectCounterNonNegative(CCharActionFsm* self, void* /*edx*/) {
    void* subObject = reinterpret_cast<void*>(self->field_00d4);
    int counterValue = CSF_AT(int, subObject, 0x18);
    return counterValue >= 0;
}

HOOK(0x483FF0, CCharActionFsm_IsSubObjectCounterNonNegative, "auto");
