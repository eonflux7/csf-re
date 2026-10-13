#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base constructor (C_0086c288 : C_0082d410), bound by address.
CSF_FUNC(C_0086c288*, __fastcall, C_0086c288_Construct_75E680, 0x0075E680, (C_0086c288* self, void*));

// Constructs a C_0083e0d8 (navigation mesh) object in place: runs the base
// constructor, installs this class's vtable, and zeroes/initializes its
// fields. Returns the object's address (the vtable slot sits at offset 0).
static uint32_t* __fastcall C_0083e0d8_Construct(C_0083e0d8* self, void* /*edx*/)
{
    C_0086c288_Construct_75E680(self, nullptr);

    self->vtbl = CSF_VTBL(0x0083E0D8);
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 10;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0050 = 10;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 10;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0070 = 0;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004DDC90, C_0083e0d8_Construct, "auto");
