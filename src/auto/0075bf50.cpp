#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall SetMarkedFlagAndIncrementCounter(void* self, void* /*edx*/)
{
    const std::uint32_t counter = CSF_AT(std::uint32_t, self, 0x1C) + 1u;
    CSF_AT(std::uint8_t, self, 0x18) = 1;
    CSF_AT(std::uint32_t, self, 0x1C) = counter;
    return static_cast<int>(counter);
}

HOOK(0x0075BF50, SetMarkedFlagAndIncrementCounter, "auto");
