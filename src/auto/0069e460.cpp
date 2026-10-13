#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// __thiscall: ecx = this, edx unused. Zeroes the tokenizer state record in the original's store order
// and returns this (&field_0000).
static uint32_t* __fastcall S_0069e460_ResetState(S_0069e460* self, void* /*edx*/) {
    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x00) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint8_t, self, 0x1C) = 0;
    CSF_AT(uint8_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    return &CSF_AT(uint32_t, self, 0x00);
}

HOOK(0x0069E460, S_0069e460_ResetState, "auto");
