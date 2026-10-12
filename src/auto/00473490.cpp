#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, ProcessEntryPair, 0x00472B60, (void* self, void* edx, int a2));

static unsigned int __fastcall Entry_ApplyTimeOffset(
    void* self, void* /*edx*/, unsigned int index, int timeOffset)
{
    const uint32_t begin = CSF_AT(uint32_t, self, 0x10);
    if (!begin)
        return 0; // The original leaves EAX unspecified on this path.

    const uint32_t difference = CSF_AT(uint32_t, self, 0x14) - begin;
    const unsigned int count =
        static_cast<unsigned int>(static_cast<int32_t>(difference) >> 2);

    if (index < count)
    {
        const uint32_t entries = CSF_AT(uint32_t, self, 0x10);
        CSF_AT(uint32_t, self, 0x08) = index;
        void* entry = CSF_AT(void*, reinterpret_cast<void*>(entries), index * 4);
        return reinterpret_cast<uint32_t>(
            ProcessEntryPair(entry, nullptr, timeOffset));
    }
    return count;
}

HOOK(0x00473490, Entry_ApplyTimeOffset, "auto");
