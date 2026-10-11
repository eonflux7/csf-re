#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Fills a 0x20-byte-plus record with the default parameter table.
// __thiscall with no stack arguments: ecx = this, ret 0.
static uint32_t* __fastcall InitDefaultsRecord(void* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(uint32_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0x3912CCF7;
    CSF_AT(uint32_t, self, 0x30) = 0x41700000;
    CSF_AT(uint32_t, self, 0x34) = 0x41700000;
    CSF_AT(uint32_t, self, 0x00) = 200;
    CSF_AT(uint32_t, self, 0x04) = 10;
    CSF_AT(uint32_t, self, 0x08) = 5;
    CSF_AT(uint32_t, self, 0x0C) = 5;
    CSF_AT(uint32_t, self, 0x10) = 0x3E4CCCCD;
    CSF_AT(uint32_t, self, 0x14) = 0x3ECCCCCD;
    CSF_AT(uint8_t, self, 0x18) = 0;
    CSF_AT(int32_t, self, 0x1C) = -1;
    return static_cast<uint32_t*>(self);
}

HOOK(0x506B00, InitDefaultsRecord, "auto");
