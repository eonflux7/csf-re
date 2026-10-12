#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RpMatFXAtomicQueryEffects, 0x005712A0, (int a1));
CSF_FUNC(int, __cdecl, RpSkinGeometryGetSkin, 0x00587050, (int a1));
CSF_FUNC(int, __cdecl, RpGeometryAddRef, 0x00590B90, (int a1));
CSF_FUNC(int, __cdecl, GfxFrames_ConfigureMaterialEffects, 0x006C07D0, (int object, int attachmentOwner));

static void __fastcall GfxObjectInstances_FinalizeAtomicVisitState(
    void* self, void* /*edx*/, int atomic)
{
    if (!atomic)
        return;

    RpSkinGeometryGetSkin(CSF_AT(
        int, reinterpret_cast<void*>(CSF_AT(int, self, 0x0C)), 0x18));
    RpSkinGeometryGetSkin(
        CSF_AT(int, reinterpret_cast<void*>(atomic), 0x18));

    if (!CSF_AT(int, self, 0x1C))
    {
        const int geometry = CSF_AT(
            int, reinterpret_cast<void*>(CSF_AT(int, self, 0x0C)), 0x18);
        CSF_AT(int, self, 0x1C) = geometry;
        RpGeometryAddRef(geometry);
    }

    bool added = false;
    int slotIndex = 1;
    auto* geometrySlot = reinterpret_cast<std::uint8_t*>(self) + 0x20;
    do
    {
        if (added)
            break;

        if (!CSF_AT(int, geometrySlot, 0))
        {
            const int geometry =
                CSF_AT(int, reinterpret_cast<void*>(atomic), 0x18);
            CSF_AT(int, geometrySlot, 0) = geometry;
            RpGeometryAddRef(geometry);
            added = true;
        }

        ++slotIndex;
        geometrySlot += 4;
    }
    while (slotIndex < 3);

    if (RpMatFXAtomicQueryEffects(atomic))
        GfxFrames_ConfigureMaterialEffects(atomic, CSF_AT(int, self, 0x40));
}

HOOK(0x006E5940, GfxObjectInstances_FinalizeAtomicVisitState, "auto");
