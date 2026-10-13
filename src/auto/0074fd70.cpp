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

CSF_FUNC(void*, __fastcall, RootDO_Construct, 0x0065E8F0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(std::uint16_t, __cdecl, ResolveStringId, 0x0066BEB0, (std::uint32_t tempString));

static NetPostman* __fastcall NetPostman_Construct(NetPostman* self, void* /*edx*/, int a2)
{
    RootDO_Construct(self, nullptr, a2);

    alignas(8) std::uint8_t temporaryStringBytes[4];
    CSF_AT(std::uint32_t, temporaryStringBytes, 0) =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(self));

    self->vtbl = CSF_VTBL(0x00868890);

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x008687D4, "Die"))));
    self->field_0020 =
        ResolveStringId(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x00868864, "Heal"))));
    self->field_0022 =
        ResolveStringId(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_ADDR_DATA(0x008679EC))));
    self->field_0024 =
        ResolveStringId(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    return self;
}

HOOK(0x0074FD70, NetPostman_Construct, "auto");
