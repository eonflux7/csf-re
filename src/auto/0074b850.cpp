#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, RootDO_Construct, 0x0065E8F0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(std::uint16_t, __cdecl, ResolveStringId, 0x0066BEB0, (std::uint32_t tempString));

static NetGhost* __fastcall NetGhost_Construct(NetGhost* self, void* /*edx*/, int a2)
{
    RootDO_Construct(self, nullptr, a2);

    alignas(8) std::uint8_t temporaryStringBytes[4];
    CSF_AT(std::uint32_t, temporaryStringBytes, 0) =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(self));

    self->vtbl = CSF_VTBL(0x00868238);

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x008681B0, "DejoDeManipularte"))));
    CSF_AT(std::uint16_t, self, 0x20) =
        ResolveStringId(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x008681C4, "QuieroManipularte"))));
    CSF_AT(std::uint16_t, self, 0x22) =
        ResolveStringId(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x008681D8, "SetBombaActivada"))));
    CSF_AT(std::uint16_t, self, 0x24) =
        ResolveStringId(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    return self;
}

HOOK(0x0074B850, NetGhost_Construct, "auto");
