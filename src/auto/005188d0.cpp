#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Resets a S_00518780 menu member record to its default state and returns
// a pointer to its first field.
static uint32_t* __fastcall S_00518780_Construct(S_00518780* self, void* /*edx*/) {
    self->field_0000 = 1;
    self->field_0004 = 0xFFFFFFFFu;
    CSF_AT(uint8_t, self, 0x08) = 0;
    CSF_AT(uint8_t, self, 0x29) = 0;
    self->field_003a = 1;
    CSF_AT(uint8_t, self, 0x3B) = 1;
    CSF_AT(uint8_t, self, 0x3C) = 0;
    CSF_AT(uint8_t, self, 0x3F) = 0;
    CSF_AT(uint32_t, self, 0x40) = 0;
    CSF_AT(uint32_t, self, 0x44) = 0;
    CSF_AT(uint8_t, self, 0x48) = 0;
    self->field_0249 = 1;
    self->field_024a = 1;
    self->field_024b = 1;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x005188D0, S_00518780_Construct, "auto");
