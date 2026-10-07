#include <cstdint>
#include "globals.h"
#include "hook.h"

static int __fastcall C_DecrementCounterAt30(void* self, void* /*edx*/, int /*unused*/)
{
    std::uint32_t result = CSF_AT(std::uint32_t, self, 0x30);
    if (result != 0)
    {
        --result;
        CSF_AT(std::uint32_t, self, 0x30) = result;
    }
    return static_cast<int>(result);
}

HOOK(0x48CAC0, C_DecrementCounterAt30, "auto");
