#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, PairLink_PrepareForRebind, 0x004E55A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, DecrementByteCounter, 0x004E55B0, (void* self, void*));
CSF_FUNC(char, __fastcall, PairLink_TestEndpoints, 0x004E5B60, (void* self, void* edx, int a2));

// Searches one of the four link groups (selected by slot) for a link whose
// key field at +8 equals `key` and whose endpoints pass PairLink_TestEndpoints.
static char __fastcall C_0083e9d0_FindMatchingPairLink(C_0083e9d0* self, void* /*edx*/, int slot, int key)
{
    char found = 0;
    PairLink_PrepareForRebind(self, nullptr);

    uint32_t groupOffset = 0;
    bool validSlot = true;
    switch (slot)
    {
    case 0:
        groupOffset = 0x80;
        break;
    case 1:
        groupOffset = 0x70;
        break;
    case 2:
        groupOffset = 0x60;
        break;
    case 3:
        groupOffset = 0x50;
        break;
    default:
        validSlot = false;
        break;
    }

    if (validSlot)
    {
        // Group layout: +0 = pointer to link table, +8 = link count.
        const uint32_t linkCount = CSF_AT(uint32_t, self, groupOffset + 8);
        for (uint32_t i = 0; i < linkCount; ++i)
        {
            uint32_t* linkTable = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, groupOffset));
            uint8_t* link = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkTable[i]));
            if (static_cast<int>(CSF_AT(uint32_t, link, 8)) == key)
            {
                int endpointsArg = static_cast<int>(reinterpret_cast<uintptr_t>(link + 12));
                if (PairLink_TestEndpoints(self, nullptr, endpointsArg))
                {
                    found = 1;
                    break;
                }
            }
        }
    }

    DecrementByteCounter(self, nullptr);
    return found;
}

HOOK(0x004E7170, C_0083e9d0_FindMatchingPairLink, "auto");
