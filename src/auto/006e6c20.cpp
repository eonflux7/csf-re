#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Sets the C_00861d48 vtable and resets every field to its default (zero, or 10 for two counters).
static uint32_t* __fastcall C_00861d48_InitDefaults(C_00861d48* self, void* /*edx*/) {
    *(void***)self = CSF_VTBL(0x00861D48);
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(uint32_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint32_t, self, 0x34) = 10;
    CSF_AT(uint32_t, self, 0x3C) = 0;
    CSF_AT(uint32_t, self, 0x40) = 0;
    CSF_AT(uint32_t, self, 0x44) = 0;
    CSF_AT(uint32_t, self, 0x48) = 10;
    self->field_0008 = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint8_t, self, 0x38) = 0;
    CSF_AT(uint8_t, self, 0x39) = 0;
    self->field_0004 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x006E6C20, C_00861d48_InitDefaults, "auto");
