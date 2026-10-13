#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x44C190 takes 4 stack args (cdecl). This wrapper forwards the three
// caller-supplied arguments and passes the 4th slot, which the original reads
// from an unwritten [ebp-4] local, as 0.
CSF_FUNC(uint32_t*, __cdecl, sub_44C190, 0x0044C190, (int a1, int a2, uint32_t* a3, int a4));

static uint32_t* __cdecl ForwardThreeArgsToC190(int a1, int a2, uint32_t* a3)
{
  return sub_44C190(a1, a2, a3, 0);
}

HOOK(0x0044C2D0, ForwardThreeArgsToC190, "auto");
