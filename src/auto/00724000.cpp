#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "globals.h"

// The original forwards (a1, a2, a3) to 0x723C50 plus a 4th stack argument read
// from an uninitialized 4-byte local ([ebp-4], set up by the prologue's `push ecx`).
// The callee (ret 0) is __cdecl, so this caller cleans the 4 argument slots itself.
CSF_FUNC(int, __cdecl, Sub_723C50, 0x723C50, (int a1, int a2, int a3, int a4));

static int __cdecl ForwardTo723C50(int a1, int a2, int a3)
{
  // Uninitialized on purpose: reproduces the read of the untouched [ebp-4] slot.
  volatile int stackLocal;
  return Sub_723C50(a1, a2, a3, stackLocal);
}

HOOK(0x724000, ForwardTo723C50, "auto");
