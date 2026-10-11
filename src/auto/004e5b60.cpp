#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, PairLink_FindEndpointSlot, 0x004E58E0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, PairLink_ProcessEndpointRecord, 0x004E5890, (void* self, void* edx, uint32_t* a2));

// Returns a 32-bit BOOL-style value: 0 when the lookup fails in mode 1; otherwise the result of
// processing the matched endpoint in mode 1, or 1/0 for a key match in the default mode.
// In the default mode, scans the 12-byte entries at field_000c for a key equal to *(a2 + 12).
static int __fastcall PairLink_TestEndpoints(C_0083e9d0* self, void* /*edx*/, int a2)
{
    if (self->field_0030 == 1)
    {
        int slotIndex = PairLink_FindEndpointSlot(self, nullptr, a2 + 8);
        if (slotIndex == -1)
            return 0;

        uint32_t* slotRecord = reinterpret_cast<uint32_t*>(
            self->field_0020 + 20u * static_cast<uint32_t>(slotIndex));
        return PairLink_ProcessEndpointRecord(self, nullptr, slotRecord);
    }

    const uint32_t endpointCount = self->field_0014;
    uint32_t matchIndex = 0xFFFFFFFFu;

    if (endpointCount != 0)
    {
        const uint32_t wantedKey = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(a2)), 0xC);
        const void* table = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_000c));

        matchIndex = 0;
        for (;;)
        {
            if (CSF_AT(uint32_t, table, 4 + 12 * matchIndex) == wantedKey)
                break;

            ++matchIndex;
            if (matchIndex >= endpointCount)
            {
                matchIndex = 0xFFFFFFFFu;
                break;
            }
        }
    }

    return matchIndex != 0xFFFFFFFFu ? 1 : 0;
}

HOOK(0x004E5B60, PairLink_TestEndpoints, "auto");
