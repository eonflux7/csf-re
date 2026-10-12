#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(char*, __cdecl, FormatValueString, 0x0064A8F0, (void** a1, char* Format, ...));
CSF_FUNC(char, __fastcall, NameMatchesCString, 0x0064A960, (void* self, void* edx, char* a1));
CSF_FUNC(char, __fastcall, NameMatchesString, 0x0064A9B0, (void* self, void* edx, void** a2));
CSF_FUNC(char, __cdecl, NetSpawnMessages_FormatUnsignedParameter, 0x0066C010, (uint32_t* a1, void** a2));

static char __fastcall NetSpawnMessages_FormatNetworkParameter(
    void* self, void* /*edx*/, void* nameOwner, void** output)
{
    void* name = static_cast<std::uint8_t*>(nameOwner) + 8;

    // The original reuses its first stack argument as temporary string storage.
    InitializeTemporaryString(
        &nameOwner, nullptr,
        reinterpret_cast<std::int32_t>(CSF_STR(0x00868D20, "bHuboCambios")));
    char matches = NameMatchesString(name, nullptr, reinterpret_cast<void**>(&nameOwner));
    DestroyTemporaryString(&nameOwner, nullptr);
    if (matches)
    {
        FormatValueString(output, const_cast<char*>(CSF_STR(0x0083090C, "%d")),
                          static_cast<int>(CSF_AT(std::uint8_t, self, 0x0)));
        return 1;
    }

    InitializeTemporaryString(
        &nameOwner, nullptr,
        reinterpret_cast<std::int32_t>(
            CSF_STR(0x00868E24, "uParRed_BandWidthEntrada")));
    matches = NameMatchesString(name, nullptr, reinterpret_cast<void**>(&nameOwner));
    DestroyTemporaryString(&nameOwner, nullptr);
    if (matches)
    {
        FormatValueString(output, const_cast<char*>(CSF_STR(0x0083090C, "%d")),
                          CSF_AT(std::int32_t, self, 0x4));
        return 1;
    }

    InitializeTemporaryString(
        &nameOwner, nullptr,
        reinterpret_cast<std::int32_t>(
            CSF_STR(0x00868DFC, "uParRed_BandWidthEntradaSeleccionado")));
    matches = NameMatchesString(name, nullptr, reinterpret_cast<void**>(&nameOwner));
    DestroyTemporaryString(&nameOwner, nullptr);
    if (matches)
    {
        FormatValueString(output, const_cast<char*>(CSF_STR(0x0083090C, "%d")),
                          CSF_AT(std::int32_t, self, 0x8));
        return 1;
    }

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868DE4, "uParRed_BandWidthSalida"))))
    {
        return NetSpawnMessages_FormatUnsignedParameter(
            reinterpret_cast<uint32_t*>(static_cast<std::uint8_t*>(self) + 0xC),
            output);
    }

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868DC0, "uParRed_BandWidthSalidaSeleccionado"))))
    {
        return NetSpawnMessages_FormatUnsignedParameter(
            reinterpret_cast<uint32_t*>(static_cast<std::uint8_t*>(self) + 0x10),
            output);
    }

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868DA8, "uParRed_EntradaJitter"))))
    {
        return NetSpawnMessages_FormatUnsignedParameter(
            reinterpret_cast<uint32_t*>(static_cast<std::uint8_t*>(self) + 0x14),
            output);
    }

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868D90, "uParRed_EntradaLatencia"))))
    {
        return NetSpawnMessages_FormatUnsignedParameter(
            reinterpret_cast<uint32_t*>(static_cast<std::uint8_t*>(self) + 0x18),
            output);
    }

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868D7C, "uParRed_OverHead"))))
    {
        return NetSpawnMessages_FormatUnsignedParameter(
            reinterpret_cast<uint32_t*>(static_cast<std::uint8_t*>(self) + 0x1C),
            output);
    }

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868D64, "uParRed_SalidaJitter"))))
    {
        return NetSpawnMessages_FormatUnsignedParameter(
            reinterpret_cast<uint32_t*>(static_cast<std::uint8_t*>(self) + 0x20),
            output);
    }

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868D4C, "uParRed_SalidaLatencia"))))
    {
        return NetSpawnMessages_FormatUnsignedParameter(
            reinterpret_cast<uint32_t*>(static_cast<std::uint8_t*>(self) + 0x24),
            output);
    }

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868D30, "uParRed_SimulacionActiva"))))
    {
        return NetSpawnMessages_FormatUnsignedParameter(
            reinterpret_cast<uint32_t*>(static_cast<std::uint8_t*>(self) + 0x28),
            output);
    }

    return 0;
}

HOOK(0x00752990, NetSpawnMessages_FormatNetworkParameter, "auto");
