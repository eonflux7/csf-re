#include <cstdint>
#include "globals.h"
#include "hook.h"

// Resets a freshly allocated object: zeroes six dwords, installs the default
// vtable at +0x1c, and sets the -1 sentinels at +0x18, +0x20, +0x24.
static void* __fastcall InitDefaultFields(void* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x00) = 0;
    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x1C) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x82D448)));
    CSF_AT(uint32_t, self, 0x20) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x24) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x18) = 0xFFFFFFFFu;
    return self;
}

HOOK(0x461940, InitDefaultFields, "auto");
