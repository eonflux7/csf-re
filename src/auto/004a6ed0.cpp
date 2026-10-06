#include <cstdint>
#include "hook.h"

// 0x004A6ED0 (6 bytes, no stack args, __cdecl)
// Original body: mov eax, 7; ret
// Returns the constant 7. No side effects, no arguments, no `this`.
static int __cdecl ReturnSeven()
{
    return 7;
}

HOOK(0x004A6ED0, ReturnSeven, "auto");
