#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __cdecl SortCompare_AscendingKeyAt10(float* left, float* right)
{
    if (CSF_AT(float, left, 0x10) < CSF_AT(float, right, 0x10))
        return -1;

    return CSF_AT(float, left, 0x10) > CSF_AT(float, right, 0x10) ? 1 : 0;
}

HOOK(0x004010C0, SortCompare_AscendingKeyAt10, "auto");
