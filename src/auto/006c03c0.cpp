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

// RwFrameForAllChildren(frame, callback, data): iterates the child frames of a frame.
CSF_FUNC(int, __cdecl, RwFrameForAllChildren, 0x006237C0, (int a1, int (__cdecl* a2)(int, int), int a3));

// Per-child callback: increments the counter passed through `data` and returns the new value.
CSF_FUNC(int, __cdecl, IncrementCounterAndReturnValue, 0x006EE5A0, (int a1, int a2));

// Counts the child frames of an RwFrame by visiting each child with the increment callback.
static int __cdecl CountFrameChildren(int frame)
{
    int childCount = 0;
    RwFrameForAllChildren(frame, IncrementCounterAndReturnValue, reinterpret_cast<int>(&childCount));
    return childCount;
}

HOOK(0x006C03C0, CountFrameChildren, "auto");
