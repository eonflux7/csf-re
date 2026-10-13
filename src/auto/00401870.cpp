#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resets a state record: installs vtables, sets the two link slots to the
// C_0082d448 vtable, marks indices as -1, zeroes counters and flags, and
// stores a2 in field_0004. Returns the object's address (vtable slot 0).
static uint32_t* __fastcall C_0082d478_ResetState(C_0082d478* self, void* /*edx*/, int a2) {
    self->vtbl = CSF_VTBL(0x0082D478);
    self->field_000c = 0xFFFFFFFFu;
    self->field_0010 = 0xFFFFFFFFu;
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_0018 = 0xFFFFFFFFu;
    self->field_001c = 0xFFFFFFFFu;
    self->field_0014 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_0004 = static_cast<uint32_t>(a2);
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_002c = 0;
    self->field_0028 = 0;
    self->field_0030 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x401870, C_0082d478_ResetState, "auto");
