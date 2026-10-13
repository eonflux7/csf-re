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

// Stores four 32-bit values into the input object's device-parameter slots
// and returns the third argument (a4), as the original does.
static int __fastcall CInputDirectInput_StoreDeviceParams(CInputDirectInput* self, void* /*edx*/,
                                                          int a2, int a3, int a4, int a5) {
    self->field_0ca4 = static_cast<uint32_t>(a2);
    self->field_0ca8 = static_cast<uint32_t>(a3);
    self->field_0cac = static_cast<uint32_t>(a4);
    self->field_0cb0 = static_cast<uint32_t>(a5);
    return a4;
}

HOOK(0x712FC0, CInputDirectInput_StoreDeviceParams, "auto");
