#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Initializes a C_00831658 record: sets the two sentinel link fields to -1,
// stores the C_0082d448 vtable in the two pointer fields, stores the event
// argument, zeroes the remaining fields, and installs the C_00831658 vtable.
// Returns the object address (the original returns &this->__vftable).
static uint32_t* __fastcall C_00831658_InitializeEventRecord(C_00831658* self, void* /*edx*/, int a2)
{
    self->field_000c = static_cast<uint32_t>(-1);
    self->field_0010 = static_cast<uint32_t>(-1);
    self->field_0008 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
    self->field_0018 = static_cast<uint32_t>(-1);
    self->field_001c = static_cast<uint32_t>(-1);
    self->field_0014 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
    self->field_0004 = static_cast<uint32_t>(a2);
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_002c = 0;
    self->field_0028 = 0;
    self->field_0030 = 0;
    self->vtbl = CSF_VTBL(0x00831658);
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x4747B0, C_00831658_InitializeEventRecord, "auto");
