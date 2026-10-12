#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x40AE60 takes 4 stack args (cdecl). This wrapper forwards the three
// caller-supplied arguments and passes the 4th slot, which the original reads
// from an unwritten [ebp-4] local, as 0.
CSF_FUNC(uint32_t*, __cdecl, sub_40AE60, 0x0040AE60, (int a1, int a2, uint32_t* a3, int a4));

static uint32_t* __cdecl Forward3ArgsToAE60(int a1, int a2, uint32_t* a3)
{
  return sub_40AE60(a1, a2, a3, 0);
}

HOOK(0x0040B460, Forward3ArgsToAE60, "auto");
