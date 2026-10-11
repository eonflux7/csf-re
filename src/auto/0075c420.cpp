#include <cstdint>
#include "hook.h"
#include "globals.h"

// The caller pushes its 3 arguments plus an untouched [ebp-4] local as the 4th argument.
// The callee is __cdecl, so the caller cleans up (ret 0).
CSF_FUNC(uint32_t*, __cdecl, sub_75C2C0, 0x0075C2C0, (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4));

static uint32_t* __cdecl WaypointGraph_ForwardTo75C2C0(uint32_t* a1, uint32_t* a2, uint32_t* a3)
{
  // The original forwards the never-written [ebp-4] slot as the 4th argument.
  // That value is garbage in the original, so it is passed as 0 here.
  int uninitArg = 0;
  return sub_75C2C0(a1, a2, a3, uninitArg);
}

HOOK(0x75C420, WaypointGraph_ForwardTo75C2C0, "auto");
