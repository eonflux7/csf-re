#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint16_t*, __fastcall, Duplicated3DObject_Initialize, 0x007A3630, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(std::uint16_t, __cdecl, ResolveStringId, 0x0066BEB0, (std::uint32_t tempString));

static uint16_t* __fastcall NetChat_Construct(NetChat* self, void* /*edx*/, int a2)
{
    Duplicated3DObject_Initialize(self, nullptr, a2);

    alignas(8) std::uint8_t temporaryBytes[4];
    auto& temporaryString = *reinterpret_cast<std::uint32_t*>(temporaryBytes);
    temporaryString = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(self));

    self->vtbl = CSF_VTBL(0x00868968);
    InitializeTemporaryString(temporaryBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x008689C4, "MensajeChatEx"))));
    self->field_0020 = ResolveStringId(temporaryString);

    InitializeTemporaryString(temporaryBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x00868944, "MensajeFliEx"))));
    self->field_0022 = ResolveStringId(temporaryString);

    return reinterpret_cast<uint16_t*>(self);
}

HOOK(0x750FD0, NetChat_Construct, "auto");
