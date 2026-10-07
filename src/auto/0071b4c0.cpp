#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// __thiscall (ecx = this, one stack arg), ret 4
CSF_FUNC(int, __fastcall, Sub71B460, 0x71B460, (int self, void* edx, uint32_t* a2));

// Forwards to sub_71B460 only when the first argument is non-null; otherwise
// returns whatever the original left in eax (nothing is written).
static int __stdcall CallHandlerIfNonNull(int a1, uint32_t* a2)
{
    if (a1)
        return Sub71B460(a1, nullptr, a2);
    return 0;
}

HOOK(0x0071B4C0, CallHandlerIfNonNull, "auto");
