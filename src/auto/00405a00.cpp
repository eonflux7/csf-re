#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resets a freshly allocated S_00526d40 (global request record) to its default state.
// Installs the C_0082d448 vtable at +0x24 and returns the address of the first dword.
static uint32_t* __fastcall S_00526d40_InitGlobalRequest(S_00526d40* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(void*, self, 0x24) = CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, self, 0x28) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x2C) = 0xFFFFFFFFu;
    self->field_0000 = 1;
    CSF_AT(uint32_t, self, 0x04) = 1;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint8_t, self, 0x34) = 0;
    CSF_AT(uint8_t, self, 0x35) = 1;
    CSF_AT(uint8_t, self, 0x36) = 1;
    CSF_AT(uint32_t, self, 0x08) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x0C) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x30) = 0;
    return &self->field_0000;
}

HOOK(0x405A00, S_00526d40_InitGlobalRequest, "auto");
