#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Copies an AutoTask order record from src into self (returns self).
// Layout as accessed by the original: dwords at +0x00, +0x04, +0x0C..+0x1C,
// bytes at +0x20..+0x22. Offset +0x08 is not touched.
static void* __fastcall AutoTask_CopyOrder(void* self, void* /*edx*/, void* src) {
    CSF_AT(uint32_t, self, 0x00) = CSF_AT(uint32_t, src, 0x00);
    CSF_AT(uint32_t, self, 0x04) = CSF_AT(uint32_t, src, 0x04);
    CSF_AT(uint32_t, self, 0x0C) = CSF_AT(uint32_t, src, 0x0C);
    CSF_AT(uint32_t, self, 0x10) = CSF_AT(uint32_t, src, 0x10);
    CSF_AT(uint32_t, self, 0x14) = CSF_AT(uint32_t, src, 0x14);
    CSF_AT(uint32_t, self, 0x18) = CSF_AT(uint32_t, src, 0x18);
    CSF_AT(uint32_t, self, 0x1C) = CSF_AT(uint32_t, src, 0x1C);
    CSF_AT(uint8_t, self, 0x20) = CSF_AT(uint8_t, src, 0x20);
    CSF_AT(uint8_t, self, 0x21) = CSF_AT(uint8_t, src, 0x21);
    CSF_AT(uint8_t, self, 0x22) = CSF_AT(uint8_t, src, 0x22);
    return self;
}

HOOK(0x489BB0, AutoTask_CopyOrder, "auto");
