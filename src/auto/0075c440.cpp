#include <cstdint>
#include "hook.h"
#include "globals.h"

// The caller pushes its 3 arguments plus the never-written [ebp-4] local as the 4th,
// and cleans up itself (__cdecl). The 4th argument is passed as 0 here since the
// original's uninitialized stack value is not meaningful behavior.
CSF_FUNC(uint32_t*, __cdecl, sub_75C350, 0x0075C350, (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4));

static uint32_t* __cdecl WaypointGraph_ForwardTo75C350(uint32_t* a1, uint32_t* a2, uint32_t* a3)
{
  return sub_75C350(a1, a2, a3, 0);
}

HOOK(0x75C440, WaypointGraph_ForwardTo75C350, "auto");
