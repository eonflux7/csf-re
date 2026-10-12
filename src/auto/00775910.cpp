#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x775600 takes 4 stack args (cdecl). This wrapper forwards the three
// caller-supplied arguments and passes the 4th slot, which the original reads
// from an unwritten [ebp-4] local, as 0.
CSF_FUNC(uint32_t*, __cdecl, sub_775600, 0x00775600, (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4));

static uint32_t* __cdecl Forward3ArgsWithZeroFourth(uint32_t* a1, uint32_t* a2, uint32_t* a3)
{
  return sub_775600(a1, a2, a3, 0);
}

HOOK(0x00775910, Forward3ArgsWithZeroFourth, "auto");
