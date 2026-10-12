#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"

// __thiscall: ecx = this, edx unused. Returns this (eax). Callee pops 0 bytes (ret).
static void* __fastcall SpatialQuery_InitializeQueryInfo(void* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(float, self, 0x2C) = 1.0f;
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint8_t, self, 0x48) = 1;
    CSF_AT(uint32_t, self, 0x58) = 0;
    CSF_AT(uint32_t, self, 0x5C) = 0;
    CSF_AT(uint32_t, self, 0x60) = 0;
    CSF_AT(uint32_t, self, 0x00) = 0;
    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x4C) = 0;
    CSF_AT(uint32_t, self, 0x50) = 0;
    CSF_AT(uint8_t, self, 0x54) = 0;
    CSF_AT(uint8_t, self, 0x55) = 0;
    return self;
}

HOOK(0x0040E070, SpatialQuery_InitializeQueryInfo, "auto");
