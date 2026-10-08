#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "globals.h"

// The original forwards (a1, a2, a3) to 0x7165B0 plus a 4th stack argument read
// from an uninitialized 4-byte local ([ebp-4], set up by the prologue's `push ecx`).
// The callee (ret 0) is __cdecl, so the caller cleans the 4 argument slots itself.
CSF_FUNC(int, __cdecl, Copy16ByteElementRange_7165B0, 0x7165B0, (int a1, int a2, int a3, int a4));

static int __cdecl ForwardTo7165B0(int a1, int a2, int a3)
{
  // Uninitialized on purpose: reproduces the read of the untouched [ebp-4] slot.
  volatile int stackLocal;
  return Copy16ByteElementRange_7165B0(a1, a2, a3, stackLocal);
}

HOOK(0x716B20, ForwardTo7165B0, "auto");
