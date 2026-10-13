#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_0047ea40_DestroyPointerStorage, 0x00558790, (S_0047ea40* self, void* /*edx*/));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor body for C_0086243c (base C_00862410): tears down two embedded
// pointer-storage sub-objects at +0x18 and +0x08, restores the base vtable,
// and frees the block when bit 0 of the argument is set.
static int* __fastcall C_0086243c_Destroy(C_0086243c* self, void* /*edx*/, char deleteFlag)
{
    self->vtbl = CSF_VTBL(0x0086243C);

    S_0047ea40_DestroyPointerStorage(
        reinterpret_cast<S_0047ea40*>(reinterpret_cast<uint8_t*>(self) + 0x18), nullptr);
    S_0047ea40_DestroyPointerStorage(
        reinterpret_cast<S_0047ea40*>(reinterpret_cast<uint8_t*>(self) + 0x08), nullptr);

    self->vtbl = CSF_VTBL(0x00862410);

    if ((deleteFlag & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<int*>(self);
}

HOOK(0x6F1670, C_0086243c_Destroy, "auto");
