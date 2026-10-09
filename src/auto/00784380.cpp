#include <cstdint>
#include "hook.h"
#include "globals.h"

// Bound as in the packet: the caller pushes a1, a2, a3 and a 4th value taken from the
// uninitialized local [ebp-4]; the callee is __cdecl, so the wrapper pops nothing (ret 0).
CSF_FUNC(uint32_t*, __cdecl, sub_784250, 0x00784250, (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4));

static uint32_t* __cdecl WeaponDefs_ForwardTo784250(uint32_t* a1, uint32_t* a2, uint32_t* a3)
{
  // The original forwards the never-written [ebp-4] slot as the 4th argument.
  // An uninitialized value is garbage, so pass 0.
  int unusedFourthArg = 0;
  return sub_784250(a1, a2, a3, unusedFourthArg);
}

HOOK(0x784380, WeaponDefs_ForwardTo784250, "auto");
