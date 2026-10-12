#include "hook.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, HudStringShow_Process, 0x004F2BC0,
         (int a1, int a2, int a3, int a4));

static int __cdecl HudStringShow_Forward(int a1, int a2, int a3)
{
    // The original forwards an uninitialized stack slot as the fourth argument.
    return HudStringShow_Process(a1, a2, a3, 0);
}

HOOK(0x004F2EF0, HudStringShow_Forward, "auto");
