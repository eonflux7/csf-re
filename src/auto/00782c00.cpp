#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x782AC0 takes 4 stack args (cdecl). This wrapper forwards the three
// caller-supplied arguments and passes the 4th slot, which the original pushes
// from an unwritten [ebp-4] local, as 0.
CSF_FUNC(void*, __cdecl, sub_782AC0, 0x00782AC0, (int a1, int a2, void* a3, int a4));

static void* __cdecl Forward3ArgsToAC0(int a1, int a2, void* a3)
{
  return sub_782AC0(a1, a2, a3, 0);
}

HOOK(0x00782C00, Forward3ArgsToAC0, "auto");
