#include <cstdint>
#include "hook.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, sub_5168D0, 0x005168D0, (int a1, int a2, int a3, int a4));

static int __cdecl ProfileSaveGames_ForwardThreeArgsTo5168D0(int a1, int a2, int a3)
{
  // The original forwards an unwritten [ebp-4] slot (reserved by `push ecx`) as the 4th argument.
  // Its value is garbage, so pass 0.
  int unusedFourthArg = 0;
  return sub_5168D0(a1, a2, a3, unusedFourthArg);
}

HOOK(0x516C50, ProfileSaveGames_ForwardThreeArgsTo5168D0, "auto");
