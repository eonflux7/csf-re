#include <cstdint>
#include "hook.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, sub_512F20, 0x00512F20, (int a1, int a2, int a3, int a4));

static int __cdecl SaveGames_ForwardTo512F20(int a1, int a2, int a3)
{
  // The original forwards an uninitialized [ebp-4] slot as the 4th argument; pass 0.
  const int unusedFourthArg = 0;
  return sub_512F20(a1, a2, a3, unusedFourthArg);
}

HOOK(0x5134F0, SaveGames_ForwardTo512F20, "auto");
