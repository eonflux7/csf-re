#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RpSkinAtomicGetType, 0x005870F0, (int a1));
CSF_FUNC(int, __cdecl, RpSkinGeometryGetSkin, 0x00587050, (int a1));
CSF_FUNC(int, __cdecl, RpSkinIsSplit, 0x005870E0, (int a1));
CSF_FUNC(int, __cdecl, RtSkinSplitAtomicSplit, 0x007C73A0, (int a1, unsigned int a2));

// Splits a skinned atomic when it has a skin type and its geometry skin is not already split.
static void __cdecl GfxSkin_SplitAtomicIfUnsplit(int atomicAddr, int /*unusedArg*/)
{
    if (atomicAddr == 0)
        return;

    auto* atomic = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(atomicAddr));
    if (CSF_AT(int, atomic, 0x70) == 0)
        return;

    if (RpSkinAtomicGetType(atomicAddr) == 0)
        return;

    int geometryAddr = CSF_AT(int, atomic, 0x18);
    if (geometryAddr != 0)
    {
        int skin = RpSkinGeometryGetSkin(geometryAddr);
        if (skin != 0 && RpSkinIsSplit(skin) != 0)
            return;
    }

    RtSkinSplitAtomicSplit(atomicAddr, 20);
}

HOOK(0x6C4DF0, GfxSkin_SplitAtomicIfUnsplit, "auto");
