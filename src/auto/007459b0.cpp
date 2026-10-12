#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(std::uint16_t, __cdecl, ResolveStringId, 0x0066BEB0, (std::uint32_t tempString));
CSF_FUNC(uint16_t*, __fastcall, Duplicated3DObject_Initialize, 0x007A3630, (void* self, void* edx, int a2));

static std::uint16_t* __fastcall NetPlayer_InitializeStringIds(
    NetPlayer* self, void* /*edx*/, int a2)
{
    Duplicated3DObject_Initialize(self, nullptr, a2);
    self->vtbl = CSF_VTBL(0x00867E70);

    alignas(8) std::uint8_t temporaryStringBytes[4] = {};

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x00867D24, "DejoDeInterrogarte"))));
    self->field_0020 = ResolveStringId(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x00867D38, "Dispara"))));
    self->field_0022 = ResolveStringId(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x00867D40, "DisparaDispersion"))));
    self->field_0024 = ResolveStringId(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x00867D54, "EstableceIndicePorRepeticion"))));
    self->field_0026 = ResolveStringId(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_ADDR_DATA(0x008679EC))));
    self->field_0028 = ResolveStringId(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x00867D74, "IncEstadisticas"))));
    self->field_002a = ResolveStringId(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_ADDR_DATA(0x00867D84))));
    self->field_002c = ResolveStringId(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x00867B74, "Muere"))));
    self->field_002e = ResolveStringId(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x00867D8C, "QuieroInterrogarte"))));
    self->field_0030 = ResolveStringId(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x00862A48, "Reset"))));
    self->field_0032 = ResolveStringId(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x00867DA0, "Respawn"))));
    self->field_0034 = ResolveStringId(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x00867DA8, "SetMuerte"))));
    self->field_0036 = ResolveStringId(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x00867DB4, "Tocado"))));
    self->field_0038 = ResolveStringId(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    return reinterpret_cast<std::uint16_t*>(self);
}

HOOK(0x007459B0, NetPlayer_InitializeStringIds, "auto");
