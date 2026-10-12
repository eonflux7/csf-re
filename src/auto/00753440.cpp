#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));
CSF_FUNC(int, __fastcall, DestroyTransportBase, 0x0064EBC0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_00867890_ResetVtableAndClearField4, 0x00740840, (C_00867890* self, void*));

static std::uint32_t* __fastcall C_00868fd8_DestroySpawnMessage(
    C_00868fd8* self, void* /*edx*/, char flags)
{
    auto* embedded = reinterpret_cast<C_00867890*>(
        reinterpret_cast<std::uint8_t*>(self) + 0x154);

    self->vtbl = CSF_VTBL(0x00868FD8);
    embedded->vtbl = CSF_VTBL(0x00869018);
    C_00867890_ResetVtableAndClearField4(embedded, nullptr);

    self->vtbl = CSF_VTBL(0x00868768);
    SceneLoader_NoOp();
    DestroyTransportBase(self, nullptr);

    if (flags & 1)
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(self));

    return reinterpret_cast<std::uint32_t*>(self);
}

HOOK(0x00753440, C_00868fd8_DestroySpawnMessage, "auto");
