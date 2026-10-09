#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copies a 0x37-byte sound-instance state block from src into self, installs the
// sound-instance vtable (0x82D448) at +0x24, and returns self.
static void* __fastcall SoundInstance_CopyFromSource(void* self, void* /*edx*/, void* src)
{
    CSF_AT(uint32_t, self, 0x00) = CSF_AT(uint32_t, src, 0x00);
    CSF_AT(uint32_t, self, 0x04) = CSF_AT(uint32_t, src, 0x04);
    CSF_AT(uint32_t, self, 0x08) = CSF_AT(uint32_t, src, 0x08);
    CSF_AT(uint32_t, self, 0x0C) = CSF_AT(uint32_t, src, 0x0C);
    CSF_AT(uint32_t, self, 0x10) = CSF_AT(uint32_t, src, 0x10);
    CSF_AT(uint32_t, self, 0x14) = CSF_AT(uint32_t, src, 0x14);

    CSF_AT(uint32_t, self, 0x18) = CSF_AT(uint32_t, src, 0x18);
    CSF_AT(uint32_t, self, 0x1C) = CSF_AT(uint32_t, src, 0x1C);
    CSF_AT(uint32_t, self, 0x20) = CSF_AT(uint32_t, src, 0x20);

    CSF_AT(void*, self, 0x24) = CSF_VTBL(0x0082D448);

    CSF_AT(uint32_t, self, 0x28) = CSF_AT(uint32_t, src, 0x28);
    CSF_AT(uint32_t, self, 0x2C) = CSF_AT(uint32_t, src, 0x2C);
    CSF_AT(uint32_t, self, 0x30) = CSF_AT(uint32_t, src, 0x30);

    CSF_AT(uint8_t, self, 0x34) = CSF_AT(uint8_t, src, 0x34);
    CSF_AT(uint8_t, self, 0x35) = CSF_AT(uint8_t, src, 0x35);
    CSF_AT(uint8_t, self, 0x36) = CSF_AT(uint8_t, src, 0x36);

    return self;
}

HOOK(0x522E20, SoundInstance_CopyFromSource, "auto");
