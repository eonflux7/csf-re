#include <cstdint>
#include "globals.h"
#include "hook.h"

// RwFrameForAllChildren(frame, callback, userData): walks the child frames of `frame`
// and invokes `callback` for each one.
CSF_FUNC(int, __cdecl, RwFrameForAllChildren, 0x6237C0, (int a1, int (__cdecl* a2)(int, int), int a3));

static int __cdecl GetHAnimChildFrameCount(int a1)
{
    // The frame lives at +4 of the object passed in.
    int frameHandle = CSF_AT(int, reinterpret_cast<uint8_t*>(a1), 0x4);

    int childFrameCount = 0;
    RwFrameForAllChildren(frameHandle,
                          reinterpret_cast<int (__cdecl*)(int, int)>(CSF_ADDR_FN(0x6E8010)),
                          reinterpret_cast<int>(&childFrameCount));
    return childFrameCount;
}

HOOK(0x6E8090, GetHAnimChildFrameCount, "auto");
