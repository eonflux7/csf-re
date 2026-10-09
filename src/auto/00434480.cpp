#include <cstdint>
#include "hook.h"
#include "globals.h"

// Hex-Rays lists 0x434050 with its real 4-slot stack layout: the caller pushes the
// 3 arguments plus an untouched [ebp-4] local as the 4th, and cleans up itself (__cdecl).
CSF_FUNC(void*, __cdecl, sub_434050, 0x00434050, (void* a1, void* a2, void* a3, int a4));

static void* __cdecl HudStringBase_ForwardTo434050(void* a1, void* a2, void* a3)
{
  // Deliberately uninitialized: reproduces the original's read of the never-written
  // [ebp-4] slot (reserved by the prologue's `push ecx`) that is forwarded as the 4th arg.
  volatile int uninitArg;
  return sub_434050(a1, a2, a3, uninitArg);
}

HOOK(0x434480, HudStringBase_ForwardTo434050, "auto");
