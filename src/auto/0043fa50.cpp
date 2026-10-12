#include <cstdint>
#include "hook.h"
#include "globals.h"

CSF_FUNC(uint32_t*, __cdecl, sub_43F610, 0x0043F610, (int a1, int a2, uint32_t* a3, int a4));

// Forwards the three caller arguments to 0x43F610 with a zero fourth argument.
// The original passes an unwritten [ebp-4] slot as that fourth argument; it is
// garbage in the original, so 0 is used here.
static uint32_t* __cdecl Campanya_ForwardToBriefingSetup(int a1, int a2, uint32_t* a3)
{
  return sub_43F610(a1, a2, a3, 0);
}

HOOK(0x0043FA50, Campanya_ForwardToBriefingSetup, "auto");
