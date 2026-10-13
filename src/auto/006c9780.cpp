#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

// Base-class constructor, called before this class's own fields are set.
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void* edx));

// Constructs C_00860954 in place: runs the base init, installs this class's vtable,
// and zeroes the three fields at +0x08, +0x0C, +0x10. Returns this.
static uint32_t* __fastcall C_00860954_ConstructSphereShapeList(C_00860954* self, void* /*edx*/) {
    C_00860178_Construct(self, nullptr);
    self->vtbl = CSF_VTBL(0x00860954);
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x006C9780, C_00860954_ConstructSphereShapeList, "auto");
