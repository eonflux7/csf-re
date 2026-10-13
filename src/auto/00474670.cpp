#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Copies the 0x54-byte state block of a C_00831678 into the caller-supplied
// destination buffer and returns the destination pointer.
static int __fastcall C_00831678_CopyStateTo(C_00831678* self, void* /*edx*/, int a2) {
    uint8_t* dst = reinterpret_cast<uint8_t*>(a2);

    CSF_AT(uint32_t, dst, 0x04) = self->field_0004;
    CSF_AT(uint32_t, dst, 0x0C) = self->field_000c;
    CSF_AT(uint32_t, dst, 0x10) = self->field_0010;
    CSF_AT(uint32_t, dst, 0x18) = self->field_0018;
    CSF_AT(uint32_t, dst, 0x1C) = self->field_001c;
    CSF_AT(uint32_t, dst, 0x24) = self->field_0024;
    CSF_AT(uint32_t, dst, 0x2C) = self->field_002c;
    CSF_AT(uint32_t, dst, 0x20) = self->field_0020;
    CSF_AT(uint8_t, dst, 0x30) = self->field_0030;
    CSF_AT(uint32_t, dst, 0x28) = self->field_0028;

    // 0x34..0x3C block
    CSF_AT(uint32_t, dst, 0x34) = self->field_0034;
    CSF_AT(uint32_t, dst, 0x38) = CSF_AT(uint32_t, self, 0x38);
    CSF_AT(uint32_t, dst, 0x3C) = CSF_AT(uint32_t, self, 0x3C);

    // 0x40..0x48 block
    CSF_AT(uint32_t, dst, 0x40) = self->field_0040;
    CSF_AT(uint32_t, dst, 0x44) = CSF_AT(uint32_t, self, 0x44);
    CSF_AT(uint32_t, dst, 0x48) = CSF_AT(uint32_t, self, 0x48);

    CSF_AT(uint32_t, dst, 0x4C) = self->field_004c;
    CSF_AT(uint32_t, dst, 0x50) = self->field_0050;
    CSF_AT(uint32_t, dst, 0x54) = self->field_0054;

    return a2;
}

HOOK(0x474670, C_00831678_CopyStateTo, "auto");
