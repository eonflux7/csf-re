#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee bindings (names and signatures per the packet).
CSF_FUNC(uint32_t*, __fastcall, sub_500E90, 0x00500E90, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_500F10, 0x00500F10, (void* self, void* edx));

// Resets the scene-loader inner buffer state of S_004f21d0 to its default
// values, resets the embedded sub-object at +0x6C, runs both follow-up
// helpers, and returns the object pointer.
static int __fastcall S_004f21d0_ResetInnerBuffer(S_004f21d0* self, void* /*edx*/)
{
    self->field_0018 = 7;
    self->field_0014 = 0;
    self->field_0004 = 0;
    self->field_002c = static_cast<uint32_t>(-1);
    self->field_0028 = static_cast<uint32_t>(-1);
    self->field_0024 = static_cast<uint32_t>(-1);
    self->field_0020 = static_cast<uint32_t>(-1);
    self->field_001c = static_cast<uint32_t>(-1);
    self->field_0030 = 0;
    self->field_0048 = static_cast<uint32_t>(-1);
    self->field_0044 = static_cast<uint32_t>(-1);
    self->field_0040 = static_cast<uint32_t>(-1);
    self->field_003c = static_cast<uint32_t>(-1);
    self->field_0038 = static_cast<uint32_t>(-1);
    self->field_004c = 0;
    self->field_0064 = 0;

    // Embedded sub-object at this+0x6C (C_0082d410 in the callee).
    uint8_t* pad_006c = reinterpret_cast<uint8_t*>(self) + 0x6C;

    self->field_0068 = 15;
    self->field_0054 = 0;

    CSF_AT(uint32_t, pad_006c, 0x18) = 15;
    CSF_AT(uint32_t, pad_006c, 0x14) = 0;
    CSF_AT(uint8_t, pad_006c, 0x04) = 0;
    CSF_AT(uint32_t, pad_006c, 0x34) = 15;
    CSF_AT(uint32_t, pad_006c, 0x30) = 0;
    CSF_AT(uint8_t, pad_006c, 0x20) = 0;

    sub_500E90(pad_006c, nullptr);
    sub_500F10(self, nullptr);

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x00501170, S_004f21d0_ResetInnerBuffer, "auto");
