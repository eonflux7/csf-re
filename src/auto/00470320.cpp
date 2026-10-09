#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "globals.h"

// 0x46FCE0 takes 4 stack args (__cdecl): a1, a2, a3 and the never-written [ebp-4] slot.
CSF_FUNC(void*, __cdecl, sub_46FCE0, 0x0046FCE0, (void* a1, void* a2, void* a3, int a4));

static void* __cdecl MissionScript_ForwardStringBaseArgs(void* a1, void* a2, void* a3)
{
  // The original forwards the uninitialized [ebp-4] slot (reserved by `push ecx`) as the
  // 4th argument. Per the rules, an uninitialized value passed along is garbage: pass 0.
  return sub_46FCE0(a1, a2, a3, 0);
}

HOOK(0x470320, MissionScript_ForwardStringBaseArgs, "auto");
