#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall IsDistinctNonSentinelValue(
    void* self, void* /*edx*/, std::int32_t candidate)
{
    const std::int32_t current = CSF_AT(std::int32_t, self, 0x78);
    return current != 2 &&
           current != -1 &&
           candidate != current &&
           candidate != 2 &&
           candidate != -1;
}

HOOK(0x0048E0D0, IsDistinctNonSentinelValue, "auto");
