#include <cstdint>
#include "hook.h"

// Returns the 32-bit value stored at offset +4 of the object/struct passed by address.
static int __stdcall GetDwordField4(int a1)
{
    return *reinterpret_cast<const int*>(a1 + 4);
}

HOOK(0x46ED90, GetDwordField4, "auto");
