#include "hook.h"

// Returns 1 when the argument is exactly 32 (signed compare via sete after cmp).
static int __cdecl IsBitDepth32(int bitDepth) {
    return bitDepth == 32 ? 1 : 0;
}

HOOK(0x761120, IsBitDepth32, "auto");
