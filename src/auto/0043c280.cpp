#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __stdcall IsFlaggedInFirstThreeStates(void* object)
{
    if ((CSF_AT(std::uint32_t, object, 0x170) & 0x2000u) == 0)
        return 0;

    const std::uint32_t state = CSF_AT(std::uint32_t, object, 0x78);
    return state <= 2u ? 1 : 0;
}

HOOK(0x0043C280, IsFlaggedInFirstThreeStates, "auto");
