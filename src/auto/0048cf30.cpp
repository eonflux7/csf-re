#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// C_00833834 constructor: installs the vtables, sets default state, and stores a2 at +0x04.
// __thiscall with one stack argument, returns this (&this->__vftable), ret 4.
static uint32_t* __fastcall C_00833834_Construct(C_00833834* self, void* /*edx*/, int a2)
{
    self->vtbl = CSF_VTBL(0x00833834);
    CSF_AT(void*, self, 0x8) = CSF_VTBL(0x0082D448);
    self->field_000c = 0xFFFFFFFFu;
    self->field_0010 = 0xFFFFFFFFu;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 10;
    self->field_0004 = static_cast<uint32_t>(a2);
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0038 = 0;
    self->field_0039 = 0;
    self->field_003c = 0x40000000u;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0048CF30, C_00833834_Construct, "auto");
