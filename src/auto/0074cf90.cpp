#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, RootDO_Construct, 0x0065E8F0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(std::uint16_t, __cdecl, ResolveStringId, 0x0066BEB0, (std::uint32_t tempString));

static std::int32_t StringSource(const char* text)
{
    return static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(text));
}

static NetGame* __fastcall NetGame_Construct(NetGame* self, void* /*edx*/, int a2)
{
    RootDO_Construct(self, nullptr, a2);

    // The original reserves its temporary with push ecx, retaining the receiver.
    std::uint32_t temp = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(self));

    self->vtbl = CSF_VTBL(0x008684C8);

    InitializeTemporaryString(&temp, nullptr,
        StringSource(CSF_STR(0x00868430, "AddPuntoRespawnToMemory")));
    self->field_0020 = ResolveStringId(temp);

    InitializeTemporaryString(&temp, nullptr,
        StringSource(CSF_STR(0x00868448, "CiclaMemoriaPuntosRespawn")));
    self->field_0022 = ResolveStringId(temp);

    InitializeTemporaryString(&temp, nullptr,
        StringSource(CSF_STR(0x00868464, "NotificaCogemosUnaClave")));
    self->field_0024 = ResolveStringId(temp);

    InitializeTemporaryString(&temp, nullptr,
        StringSource(CSF_STR(0x0086847C, "NotificaHayQueMigrar")));
    self->field_0026 = ResolveStringId(temp);

    InitializeTemporaryString(&temp, nullptr,
        StringSource(CSF_STR(0x00868494, "NotificaPunto")));
    self->field_0028 = ResolveStringId(temp);

    InitializeTemporaryString(&temp, nullptr,
        StringSource(CSF_STR(0x008682BC, "NotificaYaConectadosMasterHaEntradoEnSesion")));
    self->field_002a = ResolveStringId(temp);

    InitializeTemporaryString(&temp, nullptr,
        StringSource(CSF_STR(0x008682E8, "ReseteaMemoriasPuntosRespawn")));
    self->field_002c = ResolveStringId(temp);

    return self;
}

HOOK(0x0074CF90, NetGame_Construct, "auto");
