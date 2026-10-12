#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resets every field of a particle emitter state block to its default value
// (flags and counters zeroed, field_0010 set to 1). Returns the block pointer.
static int __fastcall ParticleEmitter_InitDefaults(S_006e2b30* self, void* /*edx*/) {
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0010 = 1;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_001d = 0;
    self->field_001e = 0;
    self->field_001f = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    return reinterpret_cast<int>(self);
}

HOOK(0x006E2B30, ParticleEmitter_InitDefaults, "auto");
