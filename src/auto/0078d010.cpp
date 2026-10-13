#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Resets a perf-counter record: zeroes the counter words, sets the word at +0x14 to 0x3DCCCCCD (0.1f), returns the same pointer.
static uint32_t* __fastcall DataSetPerfCounters_ResetSample(void* self, void* /*edx*/)
{
    uint32_t* record = static_cast<uint32_t*>(self);

    CSF_AT(uint32_t, record, 0x10) = 0;
    CSF_AT(uint32_t, record, 0x18) = 0;
    CSF_AT(uint32_t, record, 0x14) = 0x3dcccccd;
    CSF_AT(uint32_t, record, 0x00) = 0;
    CSF_AT(uint32_t, record, 0x04) = 0;
    CSF_AT(uint32_t, record, 0x08) = 0;
    CSF_AT(uint32_t, record, 0x0c) = 0;
    CSF_AT(uint32_t, record, 0x10) = 0;
    CSF_AT(uint32_t, record, 0x18) = 0;

    return record;
}

HOOK(0x78D010, DataSetPerfCounters_ResetSample, "auto");
