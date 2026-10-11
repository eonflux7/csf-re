#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee: sub_506B00, bound as given in the packet.
CSF_FUNC(uint32_t*, __fastcall, sub_506B00, 0x00506B00, (void* self, void* edx));

// Fills the default video/config block at `self` and returns `self`.
// Original: __thiscall, ret 0.
static uint8_t* __fastcall AppMain_InitVideoConfigDefaults(uint8_t* self, void* /*edx*/) {
    CSF_AT(uint8_t, self, 0x14) = 1;
    CSF_AT(int32_t, self, 0x20) = 1;
    CSF_AT(uint8_t, self, 0x24) = 1;
    CSF_AT(uint8_t, self, 0x25) = 1;

    CSF_AT(int32_t, self, 0x00) = 800;
    CSF_AT(int32_t, self, 0x04) = 600;
    CSF_AT(int32_t, self, 0x08) = 640;
    CSF_AT(int32_t, self, 0x0C) = 480;
    CSF_AT(int32_t, self, 0x10) = 32;

    CSF_AT(int32_t, self, 0x18) = 0;
    CSF_AT(int32_t, self, 0x1C) = 0;
    CSF_AT(uint8_t, self, 0x26) = 0;
    CSF_AT(uint8_t, self, 0x27) = 0;
    CSF_AT(int32_t, self, 0x28) = 0;

    CSF_AT(uint8_t, self, 0x30) = 1;
    CSF_AT(int32_t, self, 0x2C) = 100;

    sub_506B00(self + 0x34, nullptr);

    return self;
}

HOOK(0x506B70, AppMain_InitVideoConfigDefaults, "auto");
