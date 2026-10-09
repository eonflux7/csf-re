#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x00402350: __fastcall with ecx = object, no stack arguments.
// Initializes the S_00402350 (0x3C bytes) at `self` and returns a pointer to the default block it copies from.
CSF_FUNC(uint32_t*, __fastcall, InitS00402350Object, 0x00402350, (void* self, void* edx));

// Resets an S_00776ff0 to its default state. Returns the object itself.
static uint32_t* __fastcall S_00776ff0_InitDefaults(S_00776ff0* self, void* /*edx*/)
{
    self->field_0020 = 15;
    self->field_001c = 0;
    self->field_000c = 0;
    uint8_t* pad_0038 = self->pad_0038;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;

    // The original's call site references the S_00402350 vtable; the callee installs it, so this is a no-op reference.
    (void)CSF_VTBL(0x00402350);
    InitS00402350Object(pad_0038, nullptr);

    CSF_AT(uint32_t, self, 0x84) = 0;
    CSF_AT(uint32_t, self, 0x88) = 0;
    CSF_AT(uint32_t, self, 0x8C) = 0;
    CSF_AT(uint32_t, self, 0x94) = 0;
    CSF_AT(uint32_t, self, 0x98) = 0;
    CSF_AT(uint32_t, self, 0x9C) = 0;
    CSF_AT(uint32_t, self, 0xB4) = 0;
    CSF_AT(uint32_t, self, 0xB8) = 15;
    CSF_AT(uint8_t, self, 0xA4) = 0;
    CSF_AT(uint32_t, self, 0xD8) = 15;
    CSF_AT(uint32_t, self, 0xD4) = 0;
    CSF_AT(uint8_t, self, 0xC4) = 0;
    CSF_AT(uint32_t, self, 0xDC) = 0;
    CSF_AT(uint32_t, self, 0xE0) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0xE4) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x00) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x04) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x24) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x74) = 0;
    CSF_AT(uint32_t, self, 0x78) = 0;
    CSF_AT(uint8_t, self, 0x7C) = 1;
    CSF_AT(uint8_t, self, 0x28) = 0;

    // Local S_00402350 on the stack (0x3C bytes, ebp-0x3C); filled by the callee.
    uint8_t v4Storage[0x3C];
    (void)CSF_VTBL(0x00402350);
    uint32_t* defaultBlock = InitS00402350Object(v4Storage, nullptr);
    std::memcpy(pad_0038, defaultBlock, 0x3Cu);

    CSF_AT(uint8_t, self, 0xBC) = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00776FF0, S_00776ff0_InitDefaults, "auto");
