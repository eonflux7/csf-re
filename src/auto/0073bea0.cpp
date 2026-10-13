#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callees, bound by address exactly as the packet specifies.
CSF_FUNC(void, __cdecl, MpSpawnPoints_FinishEventDescriptor, 0x0040ECF0, ());
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));
CSF_FUNC(int, __fastcall, NetSpawnMessages_DestroyBase, 0x0064EBC0, (void* self, void* edx));

// Original vtable object (C_00867578) referenced by the store below.

// __thiscall destructor body of C_00867578: resets the vtable, tears down the
// embedded members in the original order, and, when the low bit of a2 is set,
// releases the transport storage. Returns this.
static uint32_t* __fastcall C_00867578_Destruct(C_00867578* self, void* /*edx*/, char a2)
{
    CSF_AT(void*, self, 0) = CSF_VTBL(0x00867578);

    SceneLoader_NoOp();                                   // this + 0x68
    SceneLoader_NoOp();                                   // this + 0x5C
    MpSpawnPoints_FinishEventDescriptor();                // this + 0x50
    SceneLoader_NoOp();                                   // this + 0x4A
    SceneLoader_NoOp();                                   // this + 0x47
    SceneLoader_NoOp();                                   // this + 0x44

    NetSpawnMessages_DestroyBase(self, nullptr);

    if ((a2 & 1) != 0)
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(self));

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0073BEA0, C_00867578_Destruct, "auto");
