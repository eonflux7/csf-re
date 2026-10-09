#include <cstdint>

#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, Entry_ComputeLookupValue, 0x00672AC0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, Entry_ApplyTimeOffset, 0x00473490, (void* self, void* edx, unsigned int a2, int a3));

static unsigned int __fastcall C_00464510_ResolveTimeIndexedEntry(void* self, void* /*edx*/, int /*a2*/, int /*a3*/, uint32_t* a4)
{
    uint8_t* owner = CSF_AT(uint8_t*, self, 0x20);

    uint32_t* requestHeader = reinterpret_cast<uint32_t*>(a4[0]);
    uint32_t flags = CSF_AT(uint32_t, requestHeader, 8);

    if ((flags & 0x800000u) != 0)
    {
        uint8_t* sceneData = CSF_AT(uint8_t*, g_pCurrentScene, 0xBC);
        owner = CSF_AT(uint8_t*, sceneData, 0x38);
        flags &= ~0x800000u;
    }

    uint8_t* table = CSF_AT(uint8_t*, owner, 8);
    uint8_t* entry = table + flags * 12u;

    uint32_t* timeRecord = reinterpret_cast<uint32_t*>(a4[1]);
    uint32_t* timeData = reinterpret_cast<uint32_t*>(timeRecord[1]);
    float timeValue = CSF_AT(float, timeData, 4);

    int64_t timeTicks = static_cast<int64_t>(timeValue);
    uint32_t timeLow = static_cast<uint32_t>(timeTicks);
    uint32_t timeHigh = static_cast<uint32_t>(static_cast<uint64_t>(timeTicks) >> 32);

    uint32_t lookupResult = static_cast<uint32_t>(
        Entry_ComputeLookupValue(CSF_AT(void*, entry, 4),
                                 reinterpret_cast<void*>(static_cast<uintptr_t>(timeHigh))));

    if (timeLow < lookupResult)
    {
        return Entry_ApplyTimeOffset(CSF_AT(void*, entry, 4), nullptr,
                                     timeLow, static_cast<int>(a4[2]));
    }
    return lookupResult;
}

HOOK(0x00464510, C_00464510_ResolveTimeIndexedEntry, "auto");
