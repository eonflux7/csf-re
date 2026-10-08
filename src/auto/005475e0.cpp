#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "globals.h"

// Target 0x5475E0 forwards (a1, a2, a3) to 0x547050 plus a fourth stack argument
// read from an uninitialized 4-byte local ([ebp-4], reserved by the prologue's
// `push ecx`).  The callee is __cdecl (ret 0), so the caller cleans the four
// argument slots itself (`add esp, 0x10`).
CSF_FUNC(void*, __cdecl, MenuExtras_CopyEntriesBackward_547050, 0x547050, (int a1, int a2, void* a3, int a4));

static void* __cdecl MenuExtras_ForwardTo547050(int a1, int a2, void* a3)
{
  // Deliberately uninitialized: reproduces the original's read of the untouched
  // [ebp-4] slot left over from the `push ecx` prologue.
  volatile int stackLocal;
  return MenuExtras_CopyEntriesBackward_547050(a1, a2, a3, stackLocal);
}

HOOK(0x5475E0, MenuExtras_ForwardTo547050, "auto");
