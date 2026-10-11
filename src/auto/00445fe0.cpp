#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static uint32_t* __fastcall InitDrawCommandRecord(
    uint32_t* self, void* /*edx*/, int command,
    const void* source, const uint32_t* values, int argument, int finalArgument)
{
    CSF_AT(void*, self, 0x4) = CSF_VTBL(0x0082D448);
    CSF_AT(int32_t, self, 0x8) = -1;
    CSF_AT(int32_t, self, 0xC) = -1;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;

    CSF_AT(int, self, 0x0) = command;
    CSF_AT(uint32_t, self, 0x8) = CSF_AT(uint32_t, source, 0x4);
    CSF_AT(uint32_t, self, 0xC) = CSF_AT(uint32_t, source, 0x8);
    CSF_AT(uint32_t, self, 0x10) = values[0];
    CSF_AT(uint32_t, self, 0x14) = values[1];
    CSF_AT(uint32_t, self, 0x18) = values[2];
    CSF_AT(int, self, 0x20) = finalArgument;
    CSF_AT(int, self, 0x1C) = argument;
    return self;
}

HOOK(0x00445FE0, InitDrawCommandRecord, "auto");
