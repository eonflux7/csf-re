#include <cstdint>
#include "hook.h"
#include "globals.h"

// The caller pushes a1, a2, a3 and a fourth slot that the original never writes
// (the [ebp-4] local reserved by `push ecx`). The fourth argument is forwarded as
// garbage in the original, so it is passed as 0 here.
CSF_FUNC(uint32_t*, __cdecl, sub_558DE0, 0x00558DE0, (int a1, int a2, uint32_t* a3, int a4));

static uint32_t* __cdecl MpRules_ForwardSabotageArgsTo558DE0(int a1, int a2, uint32_t* a3)
{
  int uninitializedArg = 0;
  return sub_558DE0(a1, a2, a3, uninitializedArg);
}

HOOK(0x558F60, MpRules_ForwardSabotageArgsTo558DE0, "auto");
