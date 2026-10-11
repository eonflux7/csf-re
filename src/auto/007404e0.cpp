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

CSF_FUNC(uint32_t*, __fastcall, ClearListAndFreeNodes, 0x004E9980, (uint32_t* self, void* edx));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(void, __cdecl, MpSpawnPoints_FinishEventDescriptor, 0x0040ECF0, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void, __fastcall, sub_740840, 0x00740840, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_64EBC0, 0x0064EBC0, (void* self, void* edx));

// Resets the C_00867850 object (vtable 0x867850 -> base 0x867800), releases the
// node list held at +0x94 and the block it owns at +0x98, tears down the
// sub-objects at +0x78, +0x72, +0x6F, +0x6C and +0x44, then finishes through
// the base-class teardown at 0x64EBC0.
static int __fastcall C_00867850_ResetAndDestroy(C_00867850* self, void* /*edx*/)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00867850);
    CSF_AT(void*, self, 0x78) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x00867840));

    ClearListAndFreeNodes(reinterpret_cast<uint32_t*>(base + 0x94), nullptr);

    void* ownedBlock = CSF_AT(void*, self, 0x98);
    if (ownedBlock)
        Mem_FreeDebug(ownedBlock);
    CSF_AT(void*, self, 0x98) = nullptr;

    sub_740840(base + 0x78, nullptr);

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00867800);

    SceneLoader_NoOp();
    SceneLoader_NoOp();
    SceneLoader_NoOp();
    MpSpawnPoints_FinishEventDescriptor();

    return sub_64EBC0(self, nullptr);
}

HOOK(0x7404E0, C_00867850_ResetAndDestroy, "auto");
