#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// int __thiscall C_0083dd58::sub_4DB0D0(void)  (0x4DB0D0)
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));

static int __fastcall C_0083dd58_CheckStateAndReset(C_0083dd58* self, void* /*edx*/) {
    int result = static_cast<int>(self->field_0070 - 1);
    if (self->field_0070 == 1) {
        self->field_0070 = 5;
        result = ResetState(&self->field_001c, nullptr);
        self->field_0018 &= ~1u;
    }
    return result;
}

HOOK(0x004DB0D0, C_0083dd58_CheckStateAndReset, "auto");
