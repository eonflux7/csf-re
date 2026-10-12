#include <cstdint>
#include "hook.h"

static std::uint32_t* __cdecl TypeRegistry_InitializeZeroPair(
    int /*unused*/, std::uint32_t* output)
{
    output[0] = 0;
    output[1] = 0;
    return output;
}

HOOK(0x0054AC50, TypeRegistry_InitializeZeroPair, "auto");
