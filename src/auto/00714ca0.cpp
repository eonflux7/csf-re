#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall GetStoredValueAtIndex(void* self, void* /*edx*/, int index)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x1AC);
    if (begin == 0)
        return -1;

    const std::int32_t byteDifference = static_cast<std::int32_t>(
        CSF_AT(std::uint32_t, self, 0x1B0) - begin);
    if ((byteDifference >> 2) == 0)
        return -1;

    const std::uint32_t elementAddress =
        CSF_AT(std::uint32_t, self, 0x1AC) +
        static_cast<std::uint32_t>(index) * 4u;
    return *reinterpret_cast<const int*>(elementAddress);
}

HOOK(0x00714CA0, GetStoredValueAtIndex, "auto");
