#include <cstdint>
#include "globals.h"
#include "hook.h"

static int __fastcall HudStringShow_SetValues(void* self, void* /*edx*/,
                                             int firstValue, int secondValue) {
    CSF_AT(int, self, 0x4C) = firstValue;
    CSF_AT(int, self, 0x50) = secondValue;
    return firstValue;
}

HOOK(0x004F4180, HudStringShow_SetValues, "auto");
