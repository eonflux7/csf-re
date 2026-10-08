#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "globals.h"

// The original forwards (a1, a2, a3) to 0x723C10 together with a 4th stack
// argument read from the untouched 4-byte local at [ebp-4] (reserved by the
// prologue's `push ecx`, never written). The callee is __cdecl (ret 0), so this
// caller cleans the 4 argument slots itself (add esp, 0x10).
CSF_FUNC(int, __cdecl, Copy16ByteElementRange_723C10, 0x723C10, (int a1, int a2, int a3, int a4));

static int __cdecl ForwardTo723C10(int a1, int a2, int a3)
{
  // Uninitialized on purpose: reproduces the read of the untouched [ebp-4] slot.
  volatile int stackLocal;
  return Copy16ByteElementRange_723C10(a1, a2, a3, stackLocal);
}

HOOK(0x723FB0, ForwardTo723C10, "auto");
