#include <cstdint>

#include "hook.h"

// Normalizes an identifier: returns the input unchanged, except the sentinel
// value 8124 (0x1FBC) which is mapped to 0.
//
// The original computes this branchlessly (lea/neg/sbb/and over the full
// 32-bit register), which is equivalent to the comparison below.
static int __fastcall RemapSentinelIdToZero(int id)
{
    return id != 0x1FBC ? id : 0;
}

HOOK(0x4EA000, RemapSentinelIdToZero, "auto");
