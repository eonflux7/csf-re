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

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(void, __cdecl, MpSpawnPoints_FinishEventDescriptor, 0x0040ECF0, ());
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));
CSF_FUNC(int, __fastcall, sub_64EBC0, 0x0064EBC0, (void* self, void* edx));

static uint32_t* __fastcall C_00867800_Destruct(C_00867800* self, void* /*edx*/, char a2)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00867800);

    SceneLoader_NoOp();
    SceneLoader_NoOp();
    SceneLoader_NoOp();
    MpSpawnPoints_FinishEventDescriptor();

    sub_64EBC0(self, nullptr);

    if ((a2 & 1) != 0)
        NetSession_ReleaseTransportStorage(static_cast<int>(reinterpret_cast<uintptr_t>(self)));

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00740490, C_00867800_Destruct, "auto");
