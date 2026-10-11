#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base-class initializer bound by address (C_0082e598 chain).
CSF_FUNC(uint32_t*, __fastcall, sub_41E1D0, 0x0041E1D0, (void* self, void* edx, int a2));

// Constructor of C_0082e7c8: runs the base initializer, then sets this class's
// vtable, the secondary base vtable at +4, and the default field values.
static uint32_t* __fastcall C_0082e7c8_Construct(C_0082e7c8* self, void* /*edx*/, int a2) {
    sub_41E1D0(self, nullptr, a2);

    self->field_0024 = 0;
    self->field_002c = 0.0f;
    self->vtbl = CSF_VTBL(0x0082E7C8);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082E7AC));
    self->field_0020 = 1;
    self->field_0030 = 0xFFFFFFFFu;

    return reinterpret_cast<uint32_t*>(&self->vtbl);
}

HOOK(0x0041FE60, C_0082e7c8_Construct, "auto");
