#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Forwards to the NetCogible DDL helper at 0x742F10. The original is a stdcall
// with three stack arguments (a1, a2, a3) and returns with `ret 12`. It loads a1
// into ecx and pushes a2 then a3, so the helper receives a1 as `this` and sees
// (a3, a2) as its two stack arguments, in that order.
CSF_FUNC(int, __fastcall, sub_742F10, 0x00742F10, (void* self, void* edx, void* a2, int a3));

static int __stdcall NetCogibleDdl_ForwardToCoreHelper(char* a1, int a2, C_008622e8* a3)
{
    return sub_742F10(a1, nullptr, a3, a2);
}

HOOK(0x007433F0, NetCogibleDdl_ForwardToCoreHelper, "auto");
