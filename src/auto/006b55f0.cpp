#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x6B54D0 takes 4 stack args (cdecl). This wrapper forwards the three
// caller-supplied arguments and passes the 4th slot, which the original reads
// from an unwritten [ebp-4] local, as 0.
CSF_FUNC(int, __cdecl, sub_6B54D0, 0x006B54D0, (int a1, int a2, int a3, int a4));

static int __cdecl Forward3ArgsToB54D0(int a1, int a2, int a3)
{
  return sub_6B54D0(a1, a2, a3, 0);
}

HOOK(0x006B55F0, Forward3ArgsToB54D0, "auto");
