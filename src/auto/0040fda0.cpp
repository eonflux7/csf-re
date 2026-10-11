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

// NormalizeVector3(this): normalizes the 3-float vector stored in the object.
// Original return value is ignored by the caller.
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));

// Initializes a C_00833b88 from a 3-component vector (a2, uint32_t[3]) and a
// vtable/header triple (a3, uint32_t[3]), then normalizes it.
// Stack args: a2 at [ebp+8], a3 at [ebp+0xC]; callee pops 8 bytes (ret 8).
static C_00833b88* __fastcall SpatialQuery_BuildContext(C_00833b88* self, void* /*edx*/, uint32_t* a2, uint32_t* a3)
{
    self->vtbl = nullptr;
    self->field_0004 = 0;
    CSF_AT(uint32_t, self, 0x8) = 0;
    self->field_000c = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    self->field_0014 = 0;

    self->vtbl = reinterpret_cast<decltype(self->vtbl)>(a3[0]);
    self->field_0004 = a3[1];
    CSF_AT(uint32_t, self, 0x8) = a3[2];

    self->field_000c = a2[0];
    CSF_AT(uint32_t, self, 0x10) = a2[1];
    self->field_0014 = a2[2];

    NormalizeVector3(self, nullptr);
    return self;
}

HOOK(0x0040FDA0, SpatialQuery_BuildContext, "auto");
