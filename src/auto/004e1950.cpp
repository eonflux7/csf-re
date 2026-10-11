#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0083e5e8_ResetBaseState, 0x004E1300, (void* self, void* edx));

static int __fastcall C_0083e5e8_ResetState(C_0083e5e8* self, void* /*edx*/) {
    C_0083e5e8_ResetBaseState(self, nullptr);
    CSF_AT(int, self, 0x18) = 0;
    CSF_AT(uint8_t, self, 0x1c) = 0;
    CSF_AT(float, self, 0x2c) = 1.0f;
    return 0;
}

HOOK(0x004E1950, C_0083e5e8_ResetState, "auto");
