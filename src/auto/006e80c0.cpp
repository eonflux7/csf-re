#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RpSkinGeometryGetSkin, 0x00587050, (int a1));
CSF_FUNC(int, __cdecl, RpSkinAtomicSetHAnimHierarchy, 0x00587020, (int a1, int a2));

// Attaches the HAnim hierarchy to an atomic only when its geometry has a skin.
// Always returns the atomic handle unchanged.
static int __cdecl GfxHanim_SetAtomicHierarchyIfSkinned(int atomicHandle, int hierarchy)
{
    void* atomic = reinterpret_cast<void*>(atomicHandle);
    int geometry = CSF_AT(int, atomic, 0x18);
    if (RpSkinGeometryGetSkin(geometry))
        RpSkinAtomicSetHAnimHierarchy(atomicHandle, hierarchy);
    return atomicHandle;
}

HOOK(0x006E80C0, GfxHanim_SetAtomicHierarchyIfSkinned, "auto");
