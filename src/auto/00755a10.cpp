#include <cstdint>
#include "hook.h"

// __cdecl, one int argument, returns a 32-bit BOOL (eax = 0 or 1).
// Returns false only for the two reserved codes 20 and 21.
static int __cdecl IsNotReservedConfigCode(int code) {
    return code != 21 && code != 20;
}

HOOK(0x755A10, IsNotReservedConfigCode, "auto");
