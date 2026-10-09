#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resets the playback/positional state of a sound instance to its defaults.
// Original: 0x005280A0 (C_008411a0, __thiscall, ret 0).
static int __fastcall C_008411a0_ResetPlaybackDefaults(C_008411a0* self, void* /*edx*/) {
    self->field_003c = 0;
    self->field_0060 = 0;
    self->field_0048 = 0.0f;
    self->field_004c = 0.0f;
    self->field_0050 = 0.0f;
    self->field_0040 = 0;
    self->field_0044 = 0.0f;
    self->field_0054 = 0;
    self->field_0058 = 1065353216;
    self->field_005c = 1065353216;
    return 1065353216;
}

HOOK(0x005280A0, C_008411a0_ResetPlaybackDefaults, "auto");
