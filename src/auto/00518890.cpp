#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Resets the S_00518780 state block to its default values and returns 0.
static int __fastcall S_00518780_ResetInnerObject(S_00518780* self, void* /*edx*/) {
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
    return 0;
}

HOOK(0x00518890, S_00518780_ResetInnerObject, "auto");
