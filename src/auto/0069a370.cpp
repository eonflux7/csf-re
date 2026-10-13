#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static uint32_t* __fastcall C_0085e760_Construct(C_0085e760* self, void* /*edx*/, int a2) {
    self->vtbl = CSF_VTBL(0x0085E760);
    self->field_0004 = static_cast<uint32_t>(a2);
    self->field_0008 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0069A370, C_0085e760_Construct, "auto");
