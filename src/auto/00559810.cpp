#include <cstdint>
#include "hook.h"
#include "globals.h"

// The original pushes its 3 arguments plus the never-written [ebp-4] local as a 4th
// argument and cleans up itself (__cdecl). The uninitialized slot is passed as 0.
CSF_FUNC(uint32_t*, __cdecl, sub_559790, 0x00559790, (int a1, int a2, uint32_t* a3, int a4));

static uint32_t* __cdecl MpRules_ForwardArgsWithZeroSlot(int a1, int a2, uint32_t* a3)
{
  // The original forwards the uninitialized [ebp-4] slot as the 4th argument.
  // Per convention, an uninitialized stack value that is passed along is 0.
  const int uninitializedSlot = 0;
  return sub_559790(a1, a2, a3, uninitializedSlot);
}

HOOK(0x559810, MpRules_ForwardArgsWithZeroSlot, "auto");
