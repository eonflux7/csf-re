#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall NetDdlObjectManager_Reset(char* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x00) = 1;

    CSF_AT(int32_t, self, 0x44) = -1;
    CSF_AT(int32_t, self, 0x40) = -1;
    CSF_AT(int32_t, self, 0x3C) = -1;
    CSF_AT(int32_t, self, 0x38) = -1;
    CSF_AT(int32_t, self, 0x34) = -1;
    CSF_AT(int32_t, self, 0x30) = -1;
    CSF_AT(int32_t, self, 0x2C) = -1;
    CSF_AT(int32_t, self, 0x28) = -1;
    CSF_AT(int32_t, self, 0x24) = -1;
    CSF_AT(int32_t, self, 0x20) = -1;
    CSF_AT(int32_t, self, 0x1C) = -1;
    CSF_AT(int32_t, self, 0x18) = -1;
    CSF_AT(int32_t, self, 0x14) = -1;
    CSF_AT(int32_t, self, 0x10) = -1;
    CSF_AT(int32_t, self, 0x0C) = -1;
    CSF_AT(int32_t, self, 0x08) = -1;
    CSF_AT(int32_t, self, 0x04) = -1;

    CSF_AT(int32_t, self, 0x4C) = -1;
    CSF_AT(int32_t, self, 0x50) = -1;
    CSF_AT(int32_t, self, 0x54) = -1;
    CSF_AT(int32_t, self, 0x58) = -1;
    CSF_AT(int32_t, self, 0x5C) = -1;
    CSF_AT(int32_t, self, 0x60) = -1;
    CSF_AT(int32_t, self, 0x64) = -1;
    CSF_AT(int32_t, self, 0x68) = -1;
    CSF_AT(int32_t, self, 0x6C) = -1;
    CSF_AT(int32_t, self, 0x70) = -1;
    CSF_AT(int32_t, self, 0x74) = -1;
    CSF_AT(int32_t, self, 0x78) = -1;
    CSF_AT(int32_t, self, 0x7C) = -1;
    CSF_AT(int32_t, self, 0x80) = -1;
    CSF_AT(int32_t, self, 0x84) = -1;
    CSF_AT(int32_t, self, 0x88) = -1;

    char* entryFlags = self + 0x90;
    for (int remaining = 16; remaining != 0; --remaining)
    {
        CSF_AT(int32_t, entryFlags, -4) = -1;
        CSF_AT(uint8_t, entryFlags, 0) = 0;
        CSF_AT(uint8_t, entryFlags, 1) = 0;
        entryFlags += 8;
    }

    CSF_AT(int32_t, self, 0x10C) = -1;
    CSF_AT(uint8_t, self, 0x00) = 1;
    return -1;
}

HOOK(0x00749E90, NetDdlObjectManager_Reset, "auto");
