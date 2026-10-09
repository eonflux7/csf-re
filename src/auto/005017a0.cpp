#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee bound as in the packet. The original pushes its three arguments plus an
// unwritten [ebp-4] local as a fourth argument, and the caller (__cdecl) cleans up.
CSF_FUNC(void*, __cdecl, sub_5010E0, 0x005010E0, (void* a1, void* a2, void* a3, int a4));

static void* __cdecl SceneLoader_ForwardStringBaseTo5010E0(void* a1, void* a2, void* a3)
{
  // The original forwards an uninitialized local as the fourth argument; pass 0.
  int uninitializedLocal = 0;
  return sub_5010E0(a1, a2, a3, uninitializedLocal);
}

HOOK(0x5017A0, SceneLoader_ForwardStringBaseTo5010E0, "auto");
