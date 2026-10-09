#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Resets every field of an S_00470e10 node to its default state.
// a2 is stored at +0x6C. Returns the node's address (its field_0000).
static uint8_t* __fastcall S_00470e10_InitDefaults(S_00470e10* self, void* /*edx*/, int a2) {
    self->field_0000 = 1;
    self->field_0001 = 1;
    self->field_0004 = 0xFFFFFFFFu;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_0050 = 0;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_0060 = 0;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_006c = static_cast<uint32_t>(a2);
    self->field_0074 = 0;
    self->field_0078 = 0;
    self->field_007c = 0;
    return &self->field_0000;
}

HOOK(0x470E10, S_00470e10_InitDefaults, "auto");
