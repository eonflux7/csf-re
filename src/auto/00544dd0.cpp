#include <cstdint>
#include "hook.h"
#include "globals.h"

// The decompiler lists 0x544C00 as taking only 3 parameters, but the original caller
// pushes 4 stack slots and cleans 16 bytes itself (the callee ends in `ret` / ret 0,
// i.e. __cdecl). The 4th argument is an untouched 4-byte stack local, so the binding
// is given 4 parameters to reproduce the exact argument layout.
CSF_FUNC(void*, __cdecl, Sub_544C00, 0x544C00, (void* a1, void* a2, void* a3, int a4));

static void* __cdecl CMenuSaveGame_ForwardTo544C00(void* a1, void* a2, void* a3)
{
  // Deliberately uninitialized: this reproduces the original's read of the never-written
  // [ebp-4] slot (reserved by the prologue's `push ecx`) that is forwarded as the 4th arg.
  volatile int uninitArg;
  return Sub_544C00(a1, a2, a3, uninitArg);
}

HOOK(0x544DD0, CMenuSaveGame_ForwardTo544C00, "auto");
