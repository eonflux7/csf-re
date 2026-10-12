#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

using AtomicVisitorFn = int (__cdecl*)(int atomic, int data);

CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));
CSF_FUNC(int, __cdecl, RwFrameGetRoot, 0x00623740, (int a1));
CSF_FUNC(int, __cdecl, RwFrameForAllChildren, 0x006237C0, (int a1, int (__cdecl* a2)(int, int), int a3));
CSF_FUNC(int, __cdecl, RwFrameForAllObjects, 0x00623910, (int frame, int (__cdecl* visitor)(uint32_t*, int), int data));

static int (__cdecl* const FrameObjectVisitor)(uint32_t*, int) =
    reinterpret_cast<int (__cdecl*)(uint32_t*, int)>(CSF_ADDR_FN(0x006C0270));
static int (__cdecl* const FrameChildVisitor)(int, int) =
    reinterpret_cast<int (__cdecl*)(int, int)>(CSF_ADDR_FN(0x006C0290));
static AtomicVisitorFn const StoreIntToOutPointerVisitor =
    reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006C02E0));

static int __cdecl GfxFrames_GetProviderFrameObject(int clump)
{
    int rootFrame = RwFrameGetRoot(CSF_AT(int, clump, 0x4));

    int found = 0;
    RwFrameForAllObjects(rootFrame, FrameObjectVisitor, reinterpret_cast<int>(&found));
    if (found != 0)
        return found;

    RwFrameForAllChildren(rootFrame, FrameChildVisitor, reinterpret_cast<int>(&found));
    if (found != 0)
        return found;

    int atomicResult = 0;
    RpClumpForAllAtomics(clump, StoreIntToOutPointerVisitor,
                        reinterpret_cast<int>(&atomicResult));
    return atomicResult;
}

HOOK(0x006C0500, GfxFrames_GetProviderFrameObject, "auto");
