#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
static CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));
static CSF_FUNC(int, __fastcall, NetSpawnMessages_DestroyBase, 0x0064EBC0, (void* self, void* edx));
static CSF_FUNC(void, __fastcall, C_00867890_ResetVtableAndClearField4, 0x00740840, (C_00867890* self, void*));

static void* __fastcall C_00868f88_Destroy(C_00868f88* self, void* /*edx*/, char flags)
{
    auto* bytes = reinterpret_cast<std::uint8_t*>(self);
    CSF_AT(void**, self, 0x0) = CSF_VTBL(0x00868F88);
    CSF_AT(void**, bytes + 0xD8, 0x0) = CSF_VTBL(0x00868FC8);
    C_00867890_ResetVtableAndClearField4(
        reinterpret_cast<C_00867890*>(bytes + 0xD8), nullptr);

    CSF_AT(void**, self, 0x0) = CSF_VTBL(0x00868658);
    SceneLoader_NoOp();
    SceneLoader_NoOp();
    SceneLoader_NoOp();
    NetSpawnMessages_DestroyBase(self, nullptr);

    if (flags & 1)
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(self));

    return self;
}

HOOK(0x00753330, C_00868f88_Destroy, "auto");
