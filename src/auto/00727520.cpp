#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "globals.h"

// The original is a pure forwarder: it reads its three declared arguments plus an
// uninitialized 4th stack slot (the [ebp-4] local created by the prologue's `push ecx`)
// and calls 0x7273C0 with (a1, a2, a3, <uninitialized local>) in that order.
// The callee is __cdecl (ret 0), so the caller itself discards the four argument slots.
CSF_FUNC(void*, __cdecl, Sub_7273C0, 0x7273C0, (int a1, int a2, void* a3, int a4));

static void* __cdecl ForwardTo7273C0(int a1, int a2, void* a3)
{
  // Uninitialized on purpose: reproduces the read of the untouched [ebp-4] slot
  // that the original pushes as the 4th argument.
  volatile int stackLocal;
  return Sub_7273C0(a1, a2, a3, stackLocal);
}

HOOK(0x727520, ForwardTo7273C0, "auto");
