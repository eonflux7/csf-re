#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x7768E0 takes 4 stack args (cdecl). This wrapper forwards the three
// caller-supplied arguments and passes the 4th slot, which the original reads
// from an unwritten [ebp-4] local, as 0.
CSF_FUNC(void*, __cdecl, sub_7768E0, 0x007768E0, (void* a1, void* a2, void* a3, int a4));

static void* __cdecl ForwardThreeArgsToHelper7768E0(void* a1, void* a2, void* a3)
{
  return sub_7768E0(a1, a2, a3, 0);
}

HOOK(0x00776D90, ForwardThreeArgsToHelper7768E0, "auto");
