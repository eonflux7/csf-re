#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Copies three 3-dword blocks from the source record (*a2 at +0x00, +0x10, +0x20)
// into the block referenced by self->field_0008 at +0x58, +0x40 and +0x4C, then
// sets bits 0x0C in the dword at +0x70 and returns the new value.
static int __fastcall C_00863ba8_CopyEffectParams(C_00863ba8* self, void* /*edx*/, uint32_t** a2)
{
    uint32_t* source = *a2;

    // Block at +0x40 <- source +0x10 (three dwords)
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x40) = source[4];
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x44) = source[5];
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x48) = source[6];

    // Block at +0x4C <- source +0x20 (three dwords)
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x4C) = source[8];
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x50) = source[9];
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x54) = source[10];

    // Block at +0x58 <- source +0x00 (three dwords)
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x58) = source[0];
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x5C) = source[1];
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x60) = source[2];

    // Set flag bits 0x0C in the dword at +0x70.
    uint32_t flags = CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x70) | 0xC;
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x70) = flags;
    return static_cast<int>(flags);
}

HOOK(0x710EB0, C_00863ba8_CopyEffectParams, "auto");
