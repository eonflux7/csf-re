#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Zeroes the scene-loader state block at this+0x04..+0x8C (the 4-byte slots the
// original touches) and returns `this` as a DWORD pointer.
static uint32_t* __fastcall S_00470ab0_ZeroAllFields(S_00470ab0* self, void* /*edx*/) {
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(uint32_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint32_t, self, 0x38) = 0;
    CSF_AT(uint32_t, self, 0x3C) = 0;
    CSF_AT(uint32_t, self, 0x44) = 0;
    CSF_AT(uint32_t, self, 0x48) = 0;
    CSF_AT(uint32_t, self, 0x4C) = 0;
    CSF_AT(uint32_t, self, 0x54) = 0;
    CSF_AT(uint32_t, self, 0x58) = 0;
    CSF_AT(uint32_t, self, 0x5C) = 0;
    CSF_AT(uint32_t, self, 0x64) = 0;
    CSF_AT(uint32_t, self, 0x68) = 0;
    CSF_AT(uint32_t, self, 0x6C) = 0;
    CSF_AT(uint32_t, self, 0x74) = 0;
    CSF_AT(uint32_t, self, 0x78) = 0;
    CSF_AT(uint32_t, self, 0x7C) = 0;
    CSF_AT(uint32_t, self, 0x84) = 0;
    CSF_AT(uint32_t, self, 0x88) = 0;
    CSF_AT(uint32_t, self, 0x8C) = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00503570, S_00470ab0_ZeroAllFields, "auto");
