#include "hook.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, ApplyScreenEffect, 0x006F8860,
         (int a1, int a2, int a3, int a4));

static int __cdecl GfxScreenEffects_ForwardEffect(int a1, int a2, int a3)
{
    // The original forwards an uninitialized local as its fourth argument.
    return ApplyScreenEffect(a1, a2, a3, 0);
}

HOOK(0x006F8940, GfxScreenEffects_ForwardEffect, "auto");
