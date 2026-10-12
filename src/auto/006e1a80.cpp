#include <cstdint>
#include "hook.h"
#include "globals.h"

// Hex-Rays lists 0x6E19B0 with its real 4-slot stack layout: the caller pushes the
// 3 arguments plus an untouched [ebp-4] local as the 4th, and cleans up itself (__cdecl).
CSF_FUNC(void*, __cdecl, sub_6E19B0, 0x006E19B0, (void* a1, void* a2, void* a3, int a4));

static void* __cdecl ParticleEmitter_ForwardStringBaseTo6E19B0(void* a1, void* a2, void* a3)
{
  // The original forwards the never-written [ebp-4] slot (reserved by `push ecx`) as the
  // 4th argument. An uninitialized stack value is garbage, so pass 0 in its place.
  return sub_6E19B0(a1, a2, a3, 0);
}

HOOK(0x6E1A80, ParticleEmitter_ForwardStringBaseTo6E19B0, "auto");
