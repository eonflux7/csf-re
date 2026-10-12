#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

CSF_FUNC(uint8_t*, __fastcall, RangeAllocateTail, 0x0051B0C0, (void* self, void* edx, char* Src, unsigned int a3, char* a4));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// __thiscall(this in ecx), stack args: requestedCount (+8), payloadArg (+0xC); ret 8.
static uint32_t __fastcall AdvanceRangeEnd(void* self, void* /*edx*/, uint32_t requestedCount, uint32_t payloadArg)
{
    uint32_t rangeStart = CSF_AT(uint32_t, self, 0x4);

    uint32_t result;
    if (rangeStart)
        result = CSF_AT(uint32_t, self, 0x8) - rangeStart;
    else
        result = 0;

    if (result >= requestedCount)
    {
        if (rangeStart)
        {
            result = CSF_AT(uint32_t, self, 0x8);
            if (requestedCount < result - rangeStart)
            {
                uint32_t newEnd = requestedCount + rangeStart;
                if (newEnd != result)
                {
                    uint32_t copyCount = result - result;
                    _memcpy(reinterpret_cast<void*>(static_cast<uintptr_t>(newEnd)),
                            reinterpret_cast<const void*>(static_cast<uintptr_t>(result)),
                            copyCount);
                    CSF_AT(uint32_t, self, 0x8) = newEnd;
                    return newEnd;
                }
            }
        }
        return result;
    }

    uint32_t usedBytes = 0;
    if (rangeStart)
        usedBytes = CSF_AT(uint32_t, self, 0x8) - rangeStart;

    char* source = reinterpret_cast<char*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x8)));
    char* payloadPtr = reinterpret_cast<char*>(&payloadArg);
    uint8_t* allocated = RangeAllocateTail(self, nullptr, source, requestedCount - usedBytes, payloadPtr);
    return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(allocated));
}

HOOK(0x0051B330, AdvanceRangeEnd, "auto");
