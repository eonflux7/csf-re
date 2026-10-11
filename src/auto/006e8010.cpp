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

CSF_FUNC(int, __cdecl, RpHAnimFrameGetHierarchy, 0x0056CF90, (int a1));
CSF_FUNC(int, __cdecl, RwFrameForAllChildren, 0x006237C0, (int a1, int (__cdecl* a2)(int, int), int a3));

// Recursively walks a frame tree looking for the first frame that owns an HAnim hierarchy.
// If the frame has a hierarchy, stores it in *outHierarchy and returns 0 (stop).
// Otherwise visits all child frames with this same function and returns the frame.
static int __cdecl HAnimFrame_FindHierarchyRecursive(int frame, uint32_t* outHierarchy)
{
    int hierarchy = RpHAnimFrameGetHierarchy(frame);
    if (hierarchy != 0)
    {
        *outHierarchy = static_cast<uint32_t>(hierarchy);
        return 0;
    }

    RwFrameForAllChildren(frame,
        reinterpret_cast<int (__cdecl*)(int, int)>(CSF_ADDR_FN(0x6E8010)),
        static_cast<int>(reinterpret_cast<uintptr_t>(outHierarchy)));
    return frame;
}

HOOK(0x6E8010, HAnimFrame_FindHierarchyRecursive, "auto");
