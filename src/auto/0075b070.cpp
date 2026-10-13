#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x75AE50 takes 4 stack args (cdecl). This wrapper forwards the three
// caller-supplied arguments and passes the 4th slot, which the original reads
// from an unwritten [ebp-4] local, as 0.
CSF_FUNC(int, __cdecl, sub_75AE50, 0x0075AE50, (int a1, int a2, int a3, int a4));

static int __cdecl SoundDefs_ForwardThreeArgsToAE50(int a1, int a2, int a3)
{
  return sub_75AE50(a1, a2, a3, 0);
}

HOOK(0x0075B070, SoundDefs_ForwardThreeArgsToAE50, "auto");
