#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, DefaultDestination_Get, 0x004FE4A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, DestinationEntries_Count, 0x0044BB80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, DestinationEntries_At, 0x0044BBA0, (void* self, void* edx, int a2));

// Walks the entries of the default destination and looks for one whose +0x38 dword
// equals the truncated float key taken from a3->[0]->[4]->[4]. When found, writes 1
// to the flag byte of the result holder (a1->[4]) and returns a1. Otherwise writes
// the last comparison result (0) and returns a1 as well.
static int __stdcall FindEntryMatchingFloatKeyAndSetFlag(int a1, int /*a2*/, int a3)
{
    void* globalValue = reinterpret_cast<void*>(GetGlobalValue_8B0568());
    bool matched = false;
    void* destination = DefaultDestination_Get(globalValue, nullptr);
    uint32_t entryIndex = 0;

    while (entryIndex < static_cast<uint32_t>(DestinationEntries_Count(destination, nullptr)))
    {
        void* entry = reinterpret_cast<void*>(
            DestinationEntries_At(destination, nullptr, static_cast<int>(entryIndex)));
        ++entryIndex;

        void* requestPtr = *reinterpret_cast<void**>(a3);
        void* innerPtr = *reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(requestPtr) + 4);
        float keyValue = *reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(innerPtr) + 4);

        int64_t truncatedKey = static_cast<int64_t>(keyValue);
        uint32_t keyLow = static_cast<uint32_t>(truncatedKey);

        matched = (CSF_AT(uint32_t, entry, 0x38) == keyLow);
        if (matched)
        {
            uint8_t* flagHolder = *reinterpret_cast<uint8_t**>(reinterpret_cast<uint8_t*>(a1) + 4);
            flagHolder[1] = static_cast<uint8_t>(matched);
            return a1;
        }
    }

    uint8_t* flagHolder = *reinterpret_cast<uint8_t**>(reinterpret_cast<uint8_t*>(a1) + 4);
    flagHolder[1] = static_cast<uint8_t>(matched);
    return a1;
}

HOOK(0x0045B290, FindEntryMatchingFloatKeyAndSetFlag, "auto");
