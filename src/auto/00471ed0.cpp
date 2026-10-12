#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall GetFrameIndexForTicks(void* self, void* /*edx*/, unsigned int ticks)
{
    const uint32_t begin = CSF_AT(uint32_t, self, 0x10);
    if (!begin)
        return 0;

    const uint32_t end = CSF_AT(uint32_t, self, 0x14);
    const int32_t byteCount = static_cast<int32_t>(end - begin);
    const uint32_t count = static_cast<uint32_t>(byteCount >> 2);
    if (ticks >= count)
        return 0;

    const uint32_t entryAddress = CSF_AT(uint32_t, self, 0x10) + 4 * ticks;
    CSF_AT(uint32_t, self, 0x08) = ticks;
    return CSF_AT(int32_t, reinterpret_cast<void*>(entryAddress), 0);
}

HOOK(0x00471ED0, GetFrameIndexForTicks, "auto");
