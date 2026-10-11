#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(void, __fastcall, MpSpawnPoints_FinishEventDescriptor, 0x0040ECF0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, BaseDestructor_7A2DE0, 0x007A2DE0, (void* self, void* edx));

static int __fastcall C_008678a8_Destroy(C_008678a8* self, void* /*edx*/)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    self->vtbl = CSF_VTBL(0x008678A8);

    SceneLoader_NoOp();  // this + 1704
    SceneLoader_NoOp();  // this + 1700
    SceneLoader_NoOp();  // this + 1694
    SceneLoader_NoOp();  // this + 1688
    SceneLoader_NoOp();  // this + 1682
    SceneLoader_NoOp();  // this + 1676
    SceneLoader_NoOp();  // this + 1670
    SceneLoader_NoOp();  // this + 1664
    SceneLoader_NoOp();  // this + 1652
    SceneLoader_NoOp();  // this + 1640
    MpSpawnPoints_FinishEventDescriptor(base + 1112, nullptr);
    SceneLoader_NoOp();  // this + 1106
    SceneLoader_NoOp();  // this + 1103
    SceneLoader_NoOp();  // this + 1100
    SceneLoader_NoOp();  // this + 1094
    SceneLoader_NoOp();  // this + 1091
    SceneLoader_NoOp();  // this + 1088

    return BaseDestructor_7A2DE0(self, nullptr);
}

HOOK(0x00741360, C_008678a8_Destroy, "auto");
