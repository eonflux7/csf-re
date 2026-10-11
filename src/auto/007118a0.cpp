#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// C_00863d18 is a 12-byte object derived from C_00863d04. This is its
// initializer: it installs the class vtable, then clears the two value
// fields at +0x4 and +0x8. The original returns `this` (eax), so the
// result is the object's address.
static uint32_t* __fastcall C_00863d18_Initialize(C_00863d18* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x00863D18);
    self->field_0004 = 0;
    self->field_0008 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x7118A0, C_00863d18_Initialize, "auto");
