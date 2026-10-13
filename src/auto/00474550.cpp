#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies this object's state fields into the caller-supplied buffer a2 and returns a2.
// Field-by-field copy in the original's order; offsets 0x38/0x3C/0x44/0x48 are not
// declared fields, so they are accessed through CSF_AT.
static int __fastcall C_00831638_CopyStateToOut(C_00831638* self, void* /*edx*/, int a2) {
    uint8_t* out = reinterpret_cast<uint8_t*>(a2);

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

    // 12-byte block at +0x34 (three dwords)
    CSF_AT(uint32_t, out, 0x34) = CSF_AT(uint32_t, self, 0x34);
    CSF_AT(uint32_t, out, 0x38) = CSF_AT(uint32_t, self, 0x38);
    CSF_AT(uint32_t, out, 0x3C) = CSF_AT(uint32_t, self, 0x3C);

    // 12-byte block at +0x40 (three dwords)
    CSF_AT(uint32_t, out, 0x40) = self->field_0040;
    CSF_AT(uint32_t, out, 0x44) = CSF_AT(uint32_t, self, 0x44);
    CSF_AT(uint32_t, out, 0x48) = CSF_AT(uint32_t, self, 0x48);

    CSF_AT(uint32_t, out, 0x4C) = self->field_004c;

    return a2;
}

HOOK(0x00474550, C_00831638_CopyStateToOut, "auto");
