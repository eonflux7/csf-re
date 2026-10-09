#include <cstdint>
#include "hook.h"
#include "globals.h"

// Hex-Rays lists 0x46C7E0 with a 4-slot cdecl signature: the caller pushes the
// 3 arguments plus the [ebp-4] local as the 4th, and cleans up itself.
CSF_FUNC(void*, __cdecl, sub_46C7E0, 0x0046C7E0, (void* a1, void* a2, void* a3, int a4));

static void* __cdecl MissionScript_ForwardToInternalParser(void* a1, void* a2, void* a3)
{
  // The original forwards an unwritten [ebp-4] slot as the 4th argument. Uninitialized
  // stack values are garbage, so pass 0 here.
  return sub_46C7E0(a1, a2, a3, 0);
}

HOOK(0x46D690, MissionScript_ForwardToInternalParser, "auto");
