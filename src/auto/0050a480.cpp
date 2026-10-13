#include <cstdint>
#include "globals.h"
#include "hook.h"

// COptionsFile default-state initializer: installs the vtable and resets the
// option flags and the -1 sentinel. Returns `this` in eax (__thiscall, ret 0).
static void* __fastcall OptionsFile_InitDefaults(void* self, void* /*edx*/) {
    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x83FCB0);
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint8_t, self, 0x04) = 1;
    CSF_AT(uint8_t, self, 0x05) = 1;
    CSF_AT(uint8_t, self, 0x06) = 0;
    CSF_AT(uint8_t, self, 0x07) = 1;
    CSF_AT(uint8_t, self, 0x10) = 0;
    CSF_AT(int32_t, self, 0x14) = -1;
    CSF_AT(uint8_t, self, 0x18) = 1;
    return self;
}

HOOK(0x0050A480, OptionsFile_InitDefaults, "auto");
