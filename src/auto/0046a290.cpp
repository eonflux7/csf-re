#include <cstdint>
#include "hook.h"
#include "globals.h"

// Hex-Rays lists 0x46A050 with its real 4-slot stack layout: the caller pushes the
// 3 arguments plus the untouched [ebp-4] local as the 4th, and cleans up itself (__cdecl).
CSF_FUNC(int, __cdecl, MissionScript_Dispatch46A050, 0x0046A050, (int a1, int a2, int a3, int a4));

static int __cdecl MissionScript_ForwardTo46A050(int a1, int a2, int a3)
{
  // The original forwards [ebp-4], a slot the prologue reserves (`push ecx`) but never writes.
  // That value is garbage, so it is passed as 0.
  const int uninitializedLocal = 0;
  return MissionScript_Dispatch46A050(a1, a2, a3, uninitializedLocal);
}

HOOK(0x46A290, MissionScript_ForwardTo46A050, "auto");
