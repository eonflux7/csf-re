#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

// Base-class constructor step (C_0086c1e0 part), called before this class's own fields are set.
CSF_FUNC(uint32_t*, __fastcall, ResetBaseTransitionState, 0x0075B5A0, (void* self, void* edx));

// Constructs C_00840e38 in place: runs the base init, installs this class's vtable,
// and zeroes the three fields at +0x0C, +0x10, +0x14. Returns this.
static uint32_t* __fastcall GameBddLoader_ConstructMenuTransition(C_00840e38* self, void* /*edx*/) {
    ResetBaseTransitionState(self, nullptr);
    self->vtbl = CSF_VTBL(0x00840E38);
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0051BF20, GameBddLoader_ConstructMenuTransition, "auto");
