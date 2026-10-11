#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Base-class constructor (thiscall, one stack argument)
CSF_FUNC(void*, __fastcall, RootDO_Construct, 0x0065E8F0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
// Takes the 4-byte temporary string object by value (cdecl), returns a 16-bit id
CSF_FUNC(std::uint16_t, __cdecl, ResolveStringId, 0x0066BEB0, (std::uint32_t tempString));

static inline std::int32_t StrArg(const char* s)
{
    return static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(s));
}

static NetCogible* __fastcall NetCogible_Construct(NetCogible* self, void* /*edx*/, int a2)
{
    std::uint32_t temp = 0;

    RootDO_Construct(self, nullptr, a2);
    self->vtbl = CSF_VTBL(0x00867A58);

    InitializeTemporaryString(&temp, nullptr, StrArg(CSF_STR(0x0086796C, "BeginRespawn")));
    self->field_0020 = ResolveStringId(temp);

    InitializeTemporaryString(&temp, nullptr, StrArg(CSF_STR(0x008679D8, "Clavate")));
    self->field_0022 = ResolveStringId(temp);

    InitializeTemporaryString(&temp, nullptr, StrArg(CSF_STR(0x008679E0, "Contacto")));
    self->field_0024 = ResolveStringId(temp);

    InitializeTemporaryString(&temp, nullptr,
                              static_cast<std::int32_t>((std::uintptr_t)CSF_ADDR_DATA(0x008679EC)));
    self->field_0026 = ResolveStringId(temp);

    InitializeTemporaryString(&temp, nullptr, StrArg(CSF_STR(0x008679F0, "RecogerArma")));
    self->field_0028 = ResolveStringId(temp);

    InitializeTemporaryString(&temp, nullptr, StrArg(CSF_STR(0x008679FC, "RecogerMunicion")));
    self->field_002a = ResolveStringId(temp);

    InitializeTemporaryString(&temp, nullptr, StrArg(CSF_STR(0x0086797C, "TeHeCogido")));
    self->field_002c = ResolveStringId(temp);

    return self;
}

HOOK(0x00742A90, NetCogible_Construct, "auto");
