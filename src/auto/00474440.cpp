#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies this object's state fields into the caller-supplied record at a2,
// then copies the trailing 12-byte block at +0x38. Returns a2 + 56.
static int __fastcall C_00831618_CopyStateTo(C_00831618* self, void* /*edx*/, int a2)
{
    uint8_t* out = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2));

    CSF_AT(uint32_t, out, 0x04) = self->field_0004;
    CSF_AT(uint32_t, out, 0x0C) = self->field_000c;
    CSF_AT(uint32_t, out, 0x10) = self->field_0010;
    CSF_AT(uint32_t, out, 0x18) = self->field_0018;
    CSF_AT(uint32_t, out, 0x1C) = self->field_001c;
    CSF_AT(uint32_t, out, 0x24) = self->field_0024;
    CSF_AT(uint32_t, out, 0x2C) = self->field_002c;
    CSF_AT(uint32_t, out, 0x20) = self->field_0020;
    CSF_AT(uint8_t, out, 0x30) = self->field_0030;
    CSF_AT(uint32_t, out, 0x28) = self->field_0028;
    CSF_AT(uint32_t, out, 0x34) = self->field_0034;

    CSF_AT(uint32_t, out, 0x38) = CSF_AT(uint32_t, self, 0x38);
    CSF_AT(uint32_t, out, 0x3C) = CSF_AT(uint32_t, self, 0x3C);
    CSF_AT(uint32_t, out, 0x40) = CSF_AT(uint32_t, self, 0x40);

    return a2 + 56;
}

HOOK(0x00474440, C_00831618_CopyStateTo, "auto");
