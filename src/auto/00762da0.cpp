#include <cstdint>
#include "hook.h"
#include "globals.h"

// The caller pushes 3 arguments plus an untouched [ebp-4] local as the 4th, and cleans up itself (__cdecl).
CSF_FUNC(uint32_t*, __cdecl, sub_762C70, 0x00762C70, (int a1, int a2, uint32_t* a3, int a4));

static uint32_t* __cdecl CharacterDefs_ForwardTo762C70(int a1, int a2, uint32_t* a3)
{
  // The original forwards its never-written [ebp-4] slot as the 4th argument; that value is garbage, so pass 0.
  return sub_762C70(a1, a2, a3, 0);
}

HOOK(0x762DA0, CharacterDefs_ForwardTo762C70, "auto");
