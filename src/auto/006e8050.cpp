#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "hook.h"

// HAnim frame id query (lib): int __cdecl RpHAnimFrameGetID(RwFrame*)
CSF_FUNC(int, __cdecl, RpHAnimFrameGetID, 0x0056CFA0, (int a1));

// RwFrame child iterator (lib): calls callback(child, data) for each child
CSF_FUNC(int, __cdecl, RwFrameForAllChildren, 0x006237C0,
         (int a1, int (__cdecl* a2)(int, int), int a3));

// Recursive search over a frame hierarchy for the frame whose HAnim id equals
// searchData[0]. On match stores the frame in searchData[1] and returns 0 to
// stop the walk; otherwise descends into the children and returns the frame.
static int __cdecl HAnim_FindFrameByIdRecursive(int frame, int* searchData)
{
    if (RpHAnimFrameGetID(frame) == searchData[0])
    {
        searchData[1] = frame;
        return 0;
    }

    RwFrameForAllChildren(frame,
                          reinterpret_cast<int (__cdecl*)(int, int)>(&HAnim_FindFrameByIdRecursive),
                          reinterpret_cast<int>(searchData));
    return frame;
}

HOOK(0x6E8050, HAnim_FindFrameByIdRecursive, "auto");
