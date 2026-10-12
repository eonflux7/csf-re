#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the NetGameDDL helper at 0x74DA00. The original takes
// (a1, a2, a3) as stdcall, passes a1 in ecx, and pushes a2 then a3, so the
// callee receives a3 as its first stack argument and a2 as its second.
// The pseudocode calls sub_74DA00(a1, a3, a2), so that order is kept here.
CSF_FUNC(void, __fastcall, NetGameDdl_HandleEntry, 0x0074DA00, (void* self, void* edx, void* a2, int a3));

static void __stdcall NetGameDdl_ForwardToHandleEntry(void* a1, int a2, void* a3)
{
    NetGameDdl_HandleEntry(a1, nullptr, a3, a2);
}

HOOK(0x0074E5D0, NetGameDdl_ForwardToHandleEntry, "auto");
