#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the NetPlayerDdl base-field dispatcher at 0x7A3BC0. The original
// takes (a1, a2, a3) as stdcall, passes a1 in ecx, and pushes a3 then a2 so the
// callee sees a2 as its first stack argument and a3 as its second.
CSF_FUNC(char, __fastcall, NetPlayerDdl_DispatchBaseField, 0x007A3BC0, (void* self, void* edx, int a5, int a3));

static char __stdcall NetPlayerDdl_ForwardToDispatchBaseField(void* a1, int a2, int a3)
{
    return NetPlayerDdl_DispatchBaseField(a1, nullptr, a2, a3);
}

HOOK(0x00750FB0, NetPlayerDdl_ForwardToDispatchBaseField, "auto");
