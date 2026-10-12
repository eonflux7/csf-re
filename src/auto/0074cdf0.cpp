#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the NetGame record-processing helper at 0x74C560. The original takes
// (a1, a2, a3) as stdcall, passes a1 in ecx, and pushes a2 and a3 as the helper's
// two stack arguments, so no frame is needed beyond the arguments. The helper returns
// its result in al and the original does not touch eax afterwards, so the return
// stays a char.
CSF_FUNC(char, __fastcall, NetGameDdl_ProcessRecord, 0x0074C560, (void* self, void* edx, int a2, void* a3));

static char __stdcall NetGameDdl_ForwardToProcessRecord(void* a1, int a2, void* a3)
{
    return NetGameDdl_ProcessRecord(a1, nullptr, a2, a3);
}

HOOK(0x0074CDF0, NetGameDdl_ForwardToProcessRecord, "auto");
