#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x69EC50 takes 4 stack args (cdecl). This wrapper forwards the three
// caller-supplied arguments and passes the 4th slot, which the original reads
// from an unwritten [ebp-4] local, as 0.
CSF_FUNC(void*, __cdecl, sub_69EC50, 0x0069EC50, (void* a1, void* a2, void* a3, int a4));

static void* __cdecl RtFS_ForwardThreeArgsTo69EC50(void* a1, void* a2, void* a3)
{
  return sub_69EC50(a1, a2, a3, 0);
}

HOOK(0x0069ED10, RtFS_ForwardThreeArgsTo69EC50, "auto");
