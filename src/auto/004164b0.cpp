#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resets a C_0082e0f4 spawn record to its default state. Stores `ownerId`
// (the single stack argument) in field_0004, sets the 0x0082D448 table
// pointer in two slots, and fills the remaining fields with -1 or 0.
// Returns the record's own address (its vtable slot at offset 0).
static uint32_t* __fastcall C_0082e0f4_InitializeSpawnRecord(C_0082e0f4* self, void* /*edx*/, int ownerId)
{
    self->field_000c = 0xFFFFFFFFu;
    self->field_0010 = 0xFFFFFFFFu;
    self->field_0008 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
    self->field_0014 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
    self->field_0018 = 0xFFFFFFFFu;
    self->field_001c = 0xFFFFFFFFu;
    self->field_0004 = static_cast<uint32_t>(ownerId);
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_002c = 0;
    self->field_0028 = 0;
    self->field_0030 = 0;
    self->vtbl = CSF_VTBL(0x0082E0F4);
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0050 = 0;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 0xFFFFFFFFu;
    self->field_0060 = 0xFFFFFFFFu;
    self->field_0064 = 0;
    self->field_0068 = 0xFFFFFFFFu;
    self->field_006c = 0;
    self->field_0070 = 0;
    self->field_0071 = 0;
    return reinterpret_cast<uint32_t*>(&self->vtbl);
}

HOOK(0x004164B0, C_0082e0f4_InitializeSpawnRecord, "auto");
