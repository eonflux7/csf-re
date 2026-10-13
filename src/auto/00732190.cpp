#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_0047ea40_DestroyPointerStorage, 0x00558790, (S_0047ea40* self, void* /*edx*/));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Destructor body for C_0086709c (base C_0086704c): tears down two embedded
// pointer-storage sub-objects at +0x18 and +0x08, restores the base vtable,
// and frees the block when bit 0 of the argument is set.
static int* __fastcall C_0086709c_Destroy(C_0086709c* self, void* /*edx*/, char deleteFlag)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0086709C);

    S_0047ea40_DestroyPointerStorage(
        reinterpret_cast<S_0047ea40*>(reinterpret_cast<uint8_t*>(self) + 0x18), nullptr);
    S_0047ea40_DestroyPointerStorage(
        reinterpret_cast<S_0047ea40*>(reinterpret_cast<uint8_t*>(self) + 0x08), nullptr);

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0086704C);

    if ((deleteFlag & 1) != 0)
        CoreMemory_Free(self);

    return reinterpret_cast<int*>(self);
}

HOOK(0x732190, C_0086709c_Destroy, "auto");
