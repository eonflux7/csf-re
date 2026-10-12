#include <cstdint>
#include "hook.h"
#include "globals.h"

// 0x558EE0 takes three pointers plus a fourth int that the original reads from the
// never-written [ebp-4] slot. That slot is passed along as garbage, so it is passed as 0 here.
CSF_FUNC(uint32_t*, __cdecl, sub_558EE0, 0x00558EE0, (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4));

static uint32_t* __cdecl MpRules_ForwardToSabotageHelper(uint32_t* a1, uint32_t* a2, uint32_t* a3)
{
  // The original pushes an uninitialized [ebp-4] local as the 4th argument. Pass 0 for it.
  const int unusedFourthArg = 0;
  return sub_558EE0(a1, a2, a3, unusedFourthArg);
}

HOOK(0x559010, MpRules_ForwardToSabotageHelper, "auto");
