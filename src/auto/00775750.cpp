#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x7753B0 takes 4 stack args (cdecl). This wrapper forwards the three
// caller-supplied arguments and passes the 4th slot, which the original reads
// from an unwritten [ebp-4] local, as 0.
CSF_FUNC(int, __cdecl, sub_7753B0, 0x007753B0, (int a1, int a2, int a3, int a4));

static int __cdecl ForwardThreeArgsTo7753B0(int a1, int a2, int a3)
{
  return sub_7753B0(a1, a2, a3, 0);
}

HOOK(0x00775750, ForwardThreeArgsTo7753B0, "auto");
