#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constructor of C_00834400 (hud_alert_icons): sets the vtable and default
// values. Returns the object pointer, as the original does.
static uint32_t* __fastcall C_00834400_Construct(C_00834400* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00834400);
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_0040 = 0x40A00000u; // 5.0f stored as bits
    self->field_003c = 0x40A00000u; // 5.0f stored as bits
    self->field_001c = 0;
    self->field_0020 = 0x3F800000u; // 1.0f stored as bits
    self->field_0024 = 0;
    self->field_0028 = 0x40400000u; // 3.0f stored as bits
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0044 = 1;
    self->field_0048 = 1;
    self->field_0014 = 0;
    self->field_0018 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004A1620, C_00834400_Construct, "auto");
