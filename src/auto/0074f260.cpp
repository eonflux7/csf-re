#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the NetPlayerList DDL helper at 0x74F050. The original takes
// (a1, a2, a3) as stdcall, passes a1 in ecx, and pushes a2 and a3 as the
// helper's two stack arguments, so no frame is needed beyond the arguments.
CSF_FUNC(char, __fastcall, NetPlayerListDdl_ProcessEntry, 0x0074F050, (void* self, void* edx, void* arg0, int a3));

static char __stdcall NetPlayerListDdl_ForwardToProcessEntry(void* a1, void* a2, int a3)
{
    return NetPlayerListDdl_ProcessEntry(a1, nullptr, a2, a3);
}

HOOK(0x0074F260, NetPlayerListDdl_ForwardToProcessEntry, "auto");
