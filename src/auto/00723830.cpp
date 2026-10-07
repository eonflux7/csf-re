#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "globals.h"

// The original forwards (a1, a2, a3) to 0x723610 plus a 4th stack argument read
// from an uninitialized 4-byte local ([ebp-4], reserved by the prologue's `push ecx`).
// The callee is __cdecl (ret 0), so this wrapper cleans the 16 bytes of arguments itself.
CSF_FUNC(int, __cdecl, TripleArgHandler, 0x723610, (int a1, int a2, int a3, int a4));

static int __cdecl ForwardToTripleArgHandler(int a1, int a2, int a3)
{
  // Uninitialized on purpose: reproduces the read of the untouched [ebp-4] slot.
  volatile int stackLocal;
  return TripleArgHandler(a1, a2, a3, stackLocal);
}

HOOK(0x723830, ForwardToTripleArgHandler, "auto");
