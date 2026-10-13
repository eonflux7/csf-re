#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static C_0085e65c* __fastcall C_0085e65c_Construct(C_0085e65c* self, void*) {
    self->vtbl = CSF_VTBL(0x0085E65C);
    self->field_0004 = 1;
    return self;
}
HOOK(0x00692390, C_0085e65c_Construct, "auto");
