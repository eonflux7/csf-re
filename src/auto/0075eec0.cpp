#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static void* __fastcall WaypointGraph_InitRecordWithPayload(
    void* self, void* /*edx*/, int firstValue, int secondValue,
    int thirdValue, const void* payload, char flag)
{
    CSF_AT(int, self, 0x00) = firstValue;
    CSF_AT(int, self, 0x08) = thirdValue;
    CSF_AT(int, self, 0x04) = secondValue;

    // Preserve REP MOVSD's forward DWORD copy of 9 dwords (0x24 bytes), including overlap.
    for (std::uint32_t offset = 0; offset < 0x24; offset += 4)
    {
        const std::uint32_t word = CSF_AT(const std::uint32_t, payload, offset);
        CSF_AT(std::uint32_t, self, 0x0C + offset) = word;
    }

    CSF_AT(char, self, 0x30) = flag;
    CSF_AT(std::uint8_t, self, 0x31) = 0;
    return self;
}

HOOK(0x0075EEC0, WaypointGraph_InitRecordWithPayload, "auto");
