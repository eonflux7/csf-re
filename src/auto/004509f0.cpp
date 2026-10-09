#include <cstdint>
#include "hook.h"
#include "globals.h"

// 0x44FA50 takes 4 stack args (__cdecl). The caller pushes its 3 args plus the
// [ebp-4] slot (reserved by the prologue's `push ecx`, never written) as the 4th.
CSF_FUNC(int, __cdecl, sub_44FA50, 0x0044FA50, (int a1, int a2, int a3, int a4));

static int __cdecl Forward3ArgsTo44FA50(int a1, int a2, int a3)
{
  // The original forwards the never-written [ebp-4] slot as the 4th argument.
  // An uninitialized stack value passed along is garbage, so pass 0.
  int uninitArg = 0;
  return sub_44FA50(a1, a2, a3, uninitArg);
}

HOOK(0x4509F0, Forward3ArgsTo44FA50, "auto");
