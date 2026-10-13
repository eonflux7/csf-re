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

// Constructor-style initializer: resets a dispatch record to its default state.
// Two vtable-like pointers are stored (C_0082D448 at +0x08 and +0x14), the
// sentinel fields are set to -1 or 0, and the object's own vtable is C_00831678.
static uint32_t* __fastcall C_00831678_InitializeDispatchRecord(C_00831678* self, void* /*edx*/, int a2)
{
    const uint32_t baseVtableValue = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));

    self->field_000c = static_cast<uint32_t>(-1);
    self->field_0010 = static_cast<uint32_t>(-1);
    self->field_0008 = baseVtableValue;
    self->field_0018 = static_cast<uint32_t>(-1);
    self->field_001c = static_cast<uint32_t>(-1);
    self->field_0014 = baseVtableValue;
    self->field_0004 = static_cast<uint32_t>(a2);
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_002c = 0;
    self->field_0028 = 0;
    self->field_0030 = 0;
    self->vtbl = CSF_VTBL(0x00831678);
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;

    // Original returns &this->__vftable, which is the object address (vtable at +0).
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00474810, C_00831678_InitializeDispatchRecord, "auto");
