#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies the state block of a C_008310f8 object into the caller-supplied buffer
// (a2) and returns that buffer.
static int __fastcall C_008310f8_CopyStateToBuffer(C_008310f8* self, void* /*edx*/, int a2)
{
    void* out = reinterpret_cast<void*>(a2);

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

    // Three-dword block at +0x34..+0x3C.
    CSF_AT(uint32_t, out, 0x34) = self->field_0034;
    CSF_AT(uint32_t, out, 0x38) = CSF_AT(uint32_t, self, 0x38);
    CSF_AT(uint32_t, out, 0x3C) = CSF_AT(uint32_t, self, 0x3C);

    CSF_AT(uint32_t, out, 0x40) = self->field_0040;
    CSF_AT(uint32_t, out, 0x44) = self->field_0044;

    // Four-dword block at +0x48..+0x54 (0x48, 0x4C, 0x50 copied in order).
    CSF_AT(uint32_t, out, 0x48) = self->field_0048;
    CSF_AT(uint32_t, out, 0x4C) = CSF_AT(uint32_t, self, 0x4C);
    CSF_AT(uint32_t, out, 0x50) = CSF_AT(uint32_t, self, 0x50);

    CSF_AT(uint32_t, out, 0x54) = self->field_0054;
    CSF_AT(uint32_t, out, 0x58) = self->field_0058;
    CSF_AT(uint8_t, out, 0x5C) = self->field_005c;

    return a2;
}

HOOK(0x4744B0, C_008310f8_CopyStateToBuffer, "auto");
