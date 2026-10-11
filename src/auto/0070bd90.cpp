#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Initializes a C_0086379c water parameter block from a 6-dword source record.
// The three float fields receive the raw dword bits (plain mov copies in the original),
// not converted values.
static C_0086379c* __fastcall C_0086379c_Construct(C_0086379c* self, void* /*edx*/, const uint32_t* a2)
{
    CSF_AT(uint32_t, self, 0x4) = CSF_ADDR_DATA(0x82D428);
    self->vtbl = CSF_VTBL(0x86379C);
    CSF_AT(uint32_t, self, 0x4) = CSF_ADDR_DATA(0x863780);

    self->field_0014 = 0.0f;
    self->field_0018 = 0.0f;
    self->field_001c = 0.0f;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0.0f;
    self->field_0018 = 0.0f;
    self->field_001c = 0.0f;

    self->field_0008 = a2[0];
    self->field_000c = a2[1];
    self->field_0010 = a2[2];
    std::memcpy(&self->field_0014, &a2[3], sizeof(uint32_t));
    std::memcpy(&self->field_0018, &a2[4], sizeof(uint32_t));
    std::memcpy(&self->field_001c, &a2[5], sizeof(uint32_t));

    return self;
}

HOOK(0x0070BD90, C_0086379c_Construct, "auto");
