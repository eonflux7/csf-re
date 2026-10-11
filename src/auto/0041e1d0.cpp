#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// TCharTask base constructor (bound by address).
CSF_FUNC(uint32_t*, __fastcall, TCharTask_BaseConstruct, 0x004DB4C0, (void* self, void* edx));

// Constructor of C_0082e598 (derived from TCharTask), one stack argument, ret 4.
static uint32_t* __fastcall C_0082e598_Construct(C_0082e598* self, void* /*edx*/, int initialValue) {
    TCharTask_BaseConstruct(self, nullptr);
    self->vtbl = CSF_VTBL(0x0082E598);
    CSF_AT(uint32_t, self, 0x4) = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0082E57C));
    self->field_0018 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0014 = static_cast<uint32_t>(initialValue);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0041E1D0, C_0082e598_Construct, "auto");
