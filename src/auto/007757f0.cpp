#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x775440 takes 4 stack args (cdecl). This wrapper forwards the three
// caller-supplied arguments and passes the 4th slot, which the original reads
// from an unwritten [ebp-4] local, as 0.
CSF_FUNC(int, __cdecl, sub_775440, 0x00775440, (int a1, int a2, int a3, int a4));

static int __cdecl ForwardThreeArgsTo775440(int a1, int a2, int a3)
{
  return sub_775440(a1, a2, a3, 0);
}

HOOK(0x007757F0, ForwardThreeArgsTo775440, "auto");
