#include <cstdint>
#include "hook.h"

// Returns bit 2 (value 1 or 0) of the 32-bit argument.
static int __stdcall MenuMpAccounts_TestFlagBit2(unsigned int a1)
{
    return (a1 >> 2) & 1;
}

HOOK(0x55BE10, MenuMpAccounts_TestFlagBit2, "auto");
