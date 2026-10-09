#include <cstdint>
#include "hook.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, sub_46EE10, 0x0046EE10, (int a1, int a2, int a3, int a4));

// The original never writes its [ebp-4] local (reserved by `push ecx` in the prologue),
// and forwards it as the 4th argument. Per the rules, an uninitialized stack value is passed as 0.
static int __cdecl MissionScript_ForwardTo46EE10(int a1, int a2, int a3)
{
  int unusedLocal = 0;
  return sub_46EE10(a1, a2, a3, unusedLocal);
}

HOOK(0x46EEC0, MissionScript_ForwardTo46EE10, "auto");
