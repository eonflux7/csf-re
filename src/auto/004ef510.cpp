#include "hook.h"

// Returns the input id unchanged, except the special id 24 which maps to 0.
//   lea eax, [ecx-0x18] ; neg eax ; sbb eax, eax ; and eax, ecx
static int __fastcall ReplaceId24WithZero(int value)
{
    return value != 24 ? value : 0;
}

HOOK(0x4EF510, ReplaceId24WithZero, "auto");
