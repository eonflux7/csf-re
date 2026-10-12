#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x441A50 takes 4 stack args (cdecl). The original forwards its three
// caller-supplied arguments and passes the 4th slot, which it reads from an
// unwritten [ebp-4] local, as 0.
CSF_FUNC(int, __cdecl, sub_441A50, 0x00441A50, (int a1, int a2, int a3, int a4));

static int __cdecl DynamicObj_Forward3ArgsWithZeroTail(int a1, int a2, int a3)
{
  return sub_441A50(a1, a2, a3, 0);
}

HOOK(0x00441AE0, DynamicObj_Forward3ArgsWithZeroTail, "auto");
