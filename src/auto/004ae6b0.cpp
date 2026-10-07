#include <cstdint>
#include "globals.h"
#include "hook.h"

// Target: 0x004AE6B0 (6 bytes)
//   mov eax, 0x1A   ; 26
//   ret
// Trivial constant-return stub: called with no stack arguments and no ecx use.
// Presumably a virtual getter (C_00839ea8 vtable) that always yields 26.

static int __cdecl GetValue26()
{
    return 26;
}

HOOK(0x004AE6B0, GetValue26, "auto");
