#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __fastcall, C_0082d410_EraseRangeAndGetStart, 0x004F4770, (void* self, void* edx, uint32_t* a2, int a3, unsigned int a4));

static uint32_t* __fastcall HudStringShow_ReleaseHelperAndScanRange(
    void* self, void* /*edx*/)
{
    void* helper = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x20));
    if (helper)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, helper, 0)[0])(helper, nullptr, 1);
        CSF_AT(uint32_t, self, 0x20) = 0;
    }

    const uint32_t capacity = CSF_AT(uint32_t, self, 0x1C);
    auto* stringObject = reinterpret_cast<uint8_t*>(self) + 4;
    auto* bufferStorage = stringObject + 4;

    uint32_t rangeBase;
    if (capacity < 0x10)
        rangeBase = reinterpret_cast<uint32_t>(bufferStorage);
    else
        rangeBase = CSF_AT(uint32_t, bufferStorage, 0);

    const uint32_t rangeEnd =
        rangeBase + CSF_AT(uint32_t, stringObject, 0x14);

    uint32_t rangeBegin;
    if (capacity < 0x10)
        rangeBegin = reinterpret_cast<uint32_t>(bufferStorage);
    else
        rangeBegin = CSF_AT(uint32_t, bufferStorage, 0);

    alignas(8) uint8_t resultBytes[0x4];
    return C_0082d410_EraseRangeAndGetStart(
        stringObject, nullptr, reinterpret_cast<uint32_t*>(resultBytes),
        static_cast<int>(rangeBegin), rangeEnd);
}

HOOK(0x004F4800, HudStringShow_ReleaseHelperAndScanRange, "auto");
