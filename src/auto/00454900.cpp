#include <cstdint>
#include "globals.h"
#include "hook.h"

// Hex-Rays shows 0x453B80 with 4 stack args. The caller pushes its 3 arguments plus an
// unwritten [ebp-4] local as the 4th, and the caller cleans up (__cdecl).
CSF_FUNC(int, __cdecl, sub_453B80, 0x00453B80, (int a1, int a2, int a3, int a4));

static int __cdecl ForwardArgsTo453B80(int a1, int a2, int a3)
{
  // The original forwards the never-written [ebp-4] slot as the 4th argument.
  // That value is garbage in the original, so pass 0.
  int uninitializedLocal = 0;
  return sub_453B80(a1, a2, a3, uninitializedLocal);
}

HOOK(0x454900, ForwardArgsTo453B80, "auto");
