#include <cstdint>
#include "globals.h"
#include "hook.h"

static int __fastcall GetFirstElement(void* self, void* /*edx*/)
{
    if (CSF_AT(std::uint32_t, self, 0x1C) != 0)
        return *reinterpret_cast<int*>(CSF_AT(std::uint32_t, self, 0x14));

    return 0;
}

HOOK(0x4FC1D0, GetFirstElement, "auto");
