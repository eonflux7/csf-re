#include <cstdint>
#include <cstring>

#include "hook.h"
#include "types/auto_classes.h"

// Clears the 256-byte input buffer at pad_0230[2304] (0x0B30 from the object start),
// marks the input as active, and zeroes four per-device state flags. Returns 0.
static char __fastcall CInputDirectInput_ResetInputBuffers(CInputDirectInput* self, void* /*edx*/) {
    self->field_0004 = 1;
    std::memset(&self->pad_0230[2304], 0, 0x100);
    self->field_0c4c = 0;
    self->field_0c4e = 0;
    self->field_0c4d = 0;
    self->field_0c4f = 0;
    return 0;
}

HOOK(0x713100, CInputDirectInput_ResetInputBuffers, "auto");
