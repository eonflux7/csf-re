#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Walks the objects attached to an RwFrame, then recurses into its children.
// Stops early (returns 0) as soon as the shared failure flag is set.

CSF_FUNC(int, __cdecl, RwFrameForAllObjects, 0x623910,
         (int a1, int (__cdecl* a2)(int*, int), int a3));

CSF_FUNC(int, __cdecl, RwFrameForAllChildren, 0x6237C0,
         (int a1, int (__cdecl* a2)(int, int), int a3));

static int __cdecl VisitFrameSubtree(int frame, int* failureFlag)
{
    // Per-object visitor at 0x6C0270 (signature: int(int* flag, int frame) as seen by RwFrameForAllObjects).
    RwFrameForAllObjects(frame,
                         reinterpret_cast<int (__cdecl*)(int*, int)>(CSF_ADDR_FN(0x6C0270)),
                         reinterpret_cast<int>(failureFlag));
    if (*failureFlag)
        return 0;

    // Recurse into children with this same function as the callback.
    RwFrameForAllChildren(frame,
                          reinterpret_cast<int (__cdecl*)(int, int)>(&VisitFrameSubtree),
                          reinterpret_cast<int>(failureFlag));
    if (*failureFlag)
        return 0;

    return frame;
}

HOOK(0x6C0290, VisitFrameSubtree, "auto");
