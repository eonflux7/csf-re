#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Stack-arg forwarder: the original takes (a1, a2, a3) as stdcall, passes a1 in ecx,
// and pushes the original a3 then a2 as the helper's two stack arguments (ret 8).
// The helper's return value (eax) is returned unchanged.
CSF_FUNC(int, __fastcall, NetPostmanDdl_HandleMessage, 0x00750350, (void* self, void* edx, void* a2, int a3));

static int __stdcall NetPostmanDdl_ForwardToHandleMessage(void* a1, int a2, void* a3)
{
    return NetPostmanDdl_HandleMessage(a1, nullptr, a3, a2);
}

HOOK(0x00750A00, NetPostmanDdl_ForwardToHandleMessage, "auto");
