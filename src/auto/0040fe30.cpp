#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Initializes an audio sound request record (__thiscall, 5 stack args, ret 0x14).
// Returns a pointer to the record's first dword (field_0000).
static uint32_t* __fastcall AudioSoundManager_InitializeRequest(S_00526d40* self, void* /*edx*/,
                                                                int a2, int a3, int a4, int a5, int a6) {
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(int, self, 0x28) = -1;
    CSF_AT(int, self, 0x2C) = -1;
    CSF_AT(void*, self, 0x24) = CSF_VTBL(0x0082D448);

    self->field_0000 = static_cast<uint32_t>(a2);
    CSF_AT(int, self, 0x04) = a4;
    CSF_AT(int, self, 0x10) = a5;
    CSF_AT(uint8_t, self, 0x35) = 1;
    CSF_AT(uint8_t, self, 0x36) = 1;
    CSF_AT(int, self, 0x08) = a3;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint8_t, self, 0x34) = 0;
    CSF_AT(int, self, 0x0C) = a6;
    CSF_AT(uint32_t, self, 0x30) = 0;

    return &self->field_0000;
}

HOOK(0x0040FE30, AudioSoundManager_InitializeRequest, "auto");
