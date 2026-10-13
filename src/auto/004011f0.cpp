#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Resets a 0x34-byte scene query filter to its default state and returns it.
// Sentinel -1 fields mean "no filter", 0x1C holds the default value 10.
static uint32_t* __fastcall SceneQueries_InitializeFilter(void* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 10;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(uint32_t, self, 0x00) = 0;
    CSF_AT(uint32_t, self, 0x04) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x08) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x0C) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x2C) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x30) = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004011F0, SceneQueries_InitializeFilter, "auto");
