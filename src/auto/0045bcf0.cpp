#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

// Returns the number of 4-byte elements in the [+0x78, +0x7c) pointer range,
// or 0 when the begin pointer is null.
static int __fastcall GetPointerRangeElementCount(void* self, void* /*edx*/)
{
    uint32_t pBegin = CSF_AT(uint32_t, self, 0x78);
    if (!pBegin)
        return 0;

    uint32_t pEnd = CSF_AT(uint32_t, self, 0x7C);
    return static_cast<int32_t>(pEnd - pBegin) >> 2;
}

HOOK(0x45BCF0, GetPointerRangeElementCount, "auto");
