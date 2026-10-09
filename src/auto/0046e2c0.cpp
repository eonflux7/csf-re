#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Resets the parser state block of the mission script reader to its defaults.
// a2 is stored at +0x48; the two counters at +0x30 and +0x40 start at 10.
// Returns the struct pointer itself (eax = this).
static uint32_t* __fastcall MissionScript_ResetParseState(S_0046e2c0* self, void* /*edx*/, int a2) {
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 10;
    self->field_0040 = 10;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0044 = 0;
    self->field_0048 = static_cast<uint32_t>(a2);
    self->field_004c = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x46E2C0, MissionScript_ResetParseState, "auto");
