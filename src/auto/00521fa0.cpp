#include <cstdint>
#include "globals.h"
#include "hook.h"

// Copies the 0x37-byte sound playback parameter block from src into self.
// Returns self (the original leaves `this` in eax).
static void* __fastcall SoundManager_CopyPlaybackParams(void* self, void* /*edx*/, void* src) {
    CSF_AT(uint32_t, self, 0x00) = CSF_AT(uint32_t, src, 0x00);
    CSF_AT(uint32_t, self, 0x04) = CSF_AT(uint32_t, src, 0x04);
    CSF_AT(uint32_t, self, 0x08) = CSF_AT(uint32_t, src, 0x08);
    CSF_AT(uint32_t, self, 0x0C) = CSF_AT(uint32_t, src, 0x0C);
    CSF_AT(uint32_t, self, 0x10) = CSF_AT(uint32_t, src, 0x10);
    CSF_AT(uint32_t, self, 0x14) = CSF_AT(uint32_t, src, 0x14);
    CSF_AT(uint32_t, self, 0x18) = CSF_AT(uint32_t, src, 0x18);
    CSF_AT(uint32_t, self, 0x1C) = CSF_AT(uint32_t, src, 0x1C);
    CSF_AT(uint32_t, self, 0x20) = CSF_AT(uint32_t, src, 0x20);
    CSF_AT(uint32_t, self, 0x28) = CSF_AT(uint32_t, src, 0x28);
    CSF_AT(uint32_t, self, 0x2C) = CSF_AT(uint32_t, src, 0x2C);
    CSF_AT(uint32_t, self, 0x30) = CSF_AT(uint32_t, src, 0x30);
    CSF_AT(uint8_t, self, 0x34) = CSF_AT(uint8_t, src, 0x34);
    CSF_AT(uint8_t, self, 0x35) = CSF_AT(uint8_t, src, 0x35);
    CSF_AT(uint8_t, self, 0x36) = CSF_AT(uint8_t, src, 0x36);
    return self;
}

HOOK(0x521FA0, SoundManager_CopyPlaybackParams, "auto");
