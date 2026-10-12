#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x006CE270: __cdecl pair processor. Bound exactly as given in the packet.
CSF_FUNC(int, __cdecl, CollisionShapes_ProcessPair, 0x006CE270, (int a1, int a2));

// 0x006CE5E0: __stdcall thunk (ret 8). Forwards both stack arguments to the
// __cdecl processor and returns its result unchanged.
static int __stdcall CollisionShapes_ForwardPairArguments(int a1, int a2)
{
    return CollisionShapes_ProcessPair(a1, a2);
}

HOOK(0x006CE5E0, CollisionShapes_ForwardPairArguments, "auto");
