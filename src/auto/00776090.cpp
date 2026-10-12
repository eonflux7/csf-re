#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x7758C0 takes 4 stack args (cdecl). The original forwards its three
// caller-supplied arguments in order and passes the 4th slot, which it reads
// from an unwritten [ebp-4] local, as 0.
CSF_FUNC(void*, __cdecl, StringBaseInitFromArgs, 0x007758C0, (void* a1, void* a2, void* a3, int a4));

static void* __cdecl ForwardThreeStringBaseArgs(void* a1, void* a2, void* a3)
{
  return StringBaseInitFromArgs(a1, a2, a3, 0);
}

HOOK(0x00776090, ForwardThreeStringBaseArgs, "auto");
