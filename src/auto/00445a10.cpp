#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static uint32_t* __fastcall CopyRecordPreservingThirdWord(
    uint32_t* self, void* /*edx*/, uint32_t* source)
{
    CSF_AT(uint32_t, self, 0x00) = CSF_AT(uint32_t, source, 0x00);
    CSF_AT(uint32_t, self, 0x04) = CSF_AT(uint32_t, source, 0x04);
    CSF_AT(uint32_t, self, 0x0C) = CSF_AT(uint32_t, source, 0x0C);
    CSF_AT(uint32_t, self, 0x10) = CSF_AT(uint32_t, source, 0x10);
    CSF_AT(uint32_t, self, 0x14) = CSF_AT(uint32_t, source, 0x14);
    CSF_AT(uint32_t, self, 0x18) = CSF_AT(uint32_t, source, 0x18);
    CSF_AT(uint32_t, self, 0x1C) = CSF_AT(uint32_t, source, 0x1C);
    CSF_AT(uint32_t, self, 0x20) = CSF_AT(uint32_t, source, 0x20);
    CSF_AT(uint32_t, self, 0x24) = CSF_AT(uint32_t, source, 0x24);
    return self;
}

HOOK(0x00445A10, CopyRecordPreservingThirdWord, "auto");
