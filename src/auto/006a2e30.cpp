#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Post-construction helper of C_0085ee28 (bound by address; original name sub_6A2BB0).
CSF_FUNC(int, __fastcall, C_0085ee28_FinishSetup, 0x006A2BB0, (void* self, void* edx));

// Constructor: stores the four constructor arguments, sets the vtable, zeroes the
// state fields, stores a5 at +0x2420, calls the post-setup helper, and returns this.
static C_0085ee28* __fastcall C_0085ee28_Construct(C_0085ee28* self, void* /*edx*/,
                                                   int a2, int a3, int a4, int a5) {
    self->field_0004 = static_cast<uint32_t>(a2);
    self->field_0008 = static_cast<uint32_t>(a3);
    self->field_0014 = static_cast<uint32_t>(a3);
    self->field_000c = static_cast<uint32_t>(a4);
    self->vtbl = CSF_VTBL(0x0085EE28);
    self->field_0018 = 0;
    self->field_0010 = 0;
    self->field_001c = 0;
    self->field_2424 = 0;
    self->field_2420 = static_cast<uint32_t>(a5);
    C_0085ee28_FinishSetup(self, nullptr);
    return self;
}

HOOK(0x006A2E30, C_0085ee28_Construct, "auto");
