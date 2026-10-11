#include <cstdint>
#include "hook.h"
#include "globals.h"

CSF_FUNC(uint32_t*, __cdecl, sub_75DF20, 0x0075DF20, (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4));

// Forwards the three caller arguments to 0x75DF20 with a fourth argument taken from the
// never-written [ebp-4] slot. That slot holds garbage in the original, so 0 is passed here.
// The caller cleans up the stack (__cdecl), so this function pops nothing.
static uint32_t* __cdecl WaypointGraph_ForwardTo75DF20(uint32_t* a1, uint32_t* a2, uint32_t* a3)
{
  return sub_75DF20(a1, a2, a3, 0);
}

HOOK(0x75E110, WaypointGraph_ForwardTo75DF20, "auto");
