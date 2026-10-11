#include <cstdint>
#include "hook.h"

static std::uint32_t* __stdcall ClearOutputPair(std::uint32_t* output)
{
    output[0] = 0;
    output[1] = 0;
    return output;
}

HOOK(0x0048EE60, ClearOutputPair, "auto");
