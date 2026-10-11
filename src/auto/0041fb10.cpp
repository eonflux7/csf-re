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

// Base-class constructor (TCharTask chain), called first with the same stack argument.
CSF_FUNC(uint32_t*, __fastcall, sub_41E1D0, 0x0041E1D0, (void* self, void* edx, int a2));

static int __fastcall C_0082e6e8_Initialize(C_0082e6e8* self, void* /*edx*/, int a2)
{
    sub_41E1D0(self, nullptr, a2);

    self->vtbl = CSF_VTBL(0x0082E6E8);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0082E6CC));
    self->field_0024 = 0.0f;
    self->field_0028 = 0.0f;
    self->field_002c = 0.0f;
    self->field_0030 = 0.0f;
    self->field_0034 = 0.0f;
    self->field_0038 = 0.0f;
    self->field_0020 = static_cast<uint32_t>(a2);
    self->field_0024 = 0.0f;
    self->field_0028 = 0.0f;
    self->field_002c = 0.0f;
    self->field_0030 = 0.0f;
    self->field_0034 = 0.0f;
    self->field_0038 = 0.0f;
    self->field_003c = 0;
    self->field_0040 = 0.0f;
    self->field_0044 = 0;

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x0041FB10, C_0082e6e8_Initialize, "auto");
