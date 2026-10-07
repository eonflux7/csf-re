#include "hook.h"

// Returns the constant 0x64 (100), matching the original's `mov eax, 0x64; ret`.
// No stack arguments are popped (ret 0), i.e. __cdecl with no parameters.
static int __cdecl GetClassSize()
{
    return 100;
}

HOOK(0x6DF7B0, GetClassSize, "auto");
