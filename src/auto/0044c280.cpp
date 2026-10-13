#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x0044BFC0 is __cdecl. Bound with the signature given in the packet.
// The original also pushes ECX and a second copy of a2 for this call; that
// extra context is not part of the bound signature.
CSF_FUNC(int, __cdecl, Process_Pair44BFC0, 0x0044BFC0, (int a1, int a2));

// 0x0044C280: __stdcall thunk, callee-cleaned (ret 8). It forwards its two
// stack arguments unchanged to the __cdecl callee.
static int __stdcall ForwardPairTo44BFC0(int a1, int a2)
{
    return Process_Pair44BFC0(a1, a2);
}

HOOK(0x0044C280, ForwardPairTo44BFC0, "auto");
