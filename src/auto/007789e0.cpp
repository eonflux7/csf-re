#include <cstdint>
#include "hook.h"
#include "globals.h"

// The decompiler lists 0x778800 as taking only 3 parameters, but the original caller
// pushes 4 stack slots and cleans 16 bytes itself (the callee ends in `ret` / ret 0,
// i.e. __cdecl). The 4th argument is an untouched 4-byte stack local, so the binding
// is given 4 parameters to reproduce the exact argument layout.
CSF_FUNC(int, __cdecl, sub_778800, 0x00778800, (int a1, int a2, int a3, int a4));

static int __cdecl GameDataDefs_ForwardTo778800(int a1, int a2, int a3)
{
  // Deliberately uninitialized: this reproduces the original's read of the never-written
  // [ebp-4] slot (reserved by the prologue's `push ecx`) that is forwarded as the 4th arg.
  volatile int uninitArg;
  return sub_778800(a1, a2, a3, uninitArg);
}

HOOK(0x7789E0, GameDataDefs_ForwardTo778800, "auto");
