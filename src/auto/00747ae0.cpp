#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, RootDO_Construct, 0x0065E8F0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(std::uint16_t, __cdecl, ResolveStringId, 0x0066BEB0, (std::uint32_t tempString));

static NetProyectil* __fastcall NetProyectil_Construct(NetProyectil* self, void* /*edx*/, int a2)
{
    RootDO_Construct(self, nullptr, a2);

    // 747AEF reserves one word; each string constructor writes it before use.
    alignas(8) std::uint8_t tempStringBytes[4];
    CSF_AT(void**, self, 0) = CSF_VTBL(0x00867F60);

    InitializeTemporaryString(
        tempStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x00867F1C, "ColisionConBicho"))));
    self->field_0020 = ResolveStringId(CSF_AT(std::uint32_t, tempStringBytes, 0));

    InitializeTemporaryString(
        tempStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x008679E0, "Contacto"))));
    self->field_0022 = ResolveStringId(CSF_AT(std::uint32_t, tempStringBytes, 0));

    InitializeTemporaryString(
        tempStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x00867F30, "Detona"))));
    self->field_0024 = ResolveStringId(CSF_AT(std::uint32_t, tempStringBytes, 0));

    InitializeTemporaryString(
        tempStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_ADDR_DATA(0x008679EC))));
    self->field_0026 = ResolveStringId(CSF_AT(std::uint32_t, tempStringBytes, 0));

    return self;
}

HOOK(0x00747AE0, NetProyectil_Construct, "auto");
