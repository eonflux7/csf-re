#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(char*, __cdecl, FormatValueString, 0x0064A8F0, (void** a1, char* Format, ...));
CSF_FUNC(char, __fastcall, NameMatchesCString, 0x0064A960, (void* self, void* edx, char* a1));
CSF_FUNC(char, __fastcall, NameMatchesString, 0x0064A9B0, (void* self, void* edx, void** a2));
CSF_FUNC(char, __cdecl, NetSpawnMessages_FormatPacketTime, 0x0066C010, (uint32_t* a1, void** a2));

static char __fastcall NetSpawnMessages_FormatNetworkParameter(
    void* self, void* /*edx*/, void* nameObject, void** output)
{
    void* name = static_cast<std::uint8_t*>(nameObject) + 8;
    alignas(8) std::uint8_t temporaryBytes[4];
    void** temporary = reinterpret_cast<void**>(temporaryBytes);

    InitializeTemporaryString(
        temporary, nullptr,
        reinterpret_cast<std::int32_t>(CSF_STR(0x00868D20, "bHuboCambios")));
    char matches = NameMatchesString(name, nullptr, temporary);
    DestroyTemporaryString(temporary, nullptr);
    if (matches)
    {
        FormatValueString(
            output, const_cast<char*>(CSF_STR(0x0083090C, "%d")),
            static_cast<int>(CSF_AT(std::uint8_t, self, 0)));
        return 1;
    }

    InitializeTemporaryString(
        temporary, nullptr,
        reinterpret_cast<std::int32_t>(
            CSF_STR(0x00868E70, "uParRed_EmpaquetadoActivo")));
    matches = NameMatchesString(name, nullptr, temporary);
    DestroyTemporaryString(temporary, nullptr);
    if (matches)
    {
        FormatValueString(
            output, const_cast<char*>(CSF_STR(0x0083090C, "%d")),
            CSF_AT(std::int32_t, self, 4));
        return 1;
    }

    InitializeTemporaryString(
        temporary, nullptr,
        reinterpret_cast<std::int32_t>(
            CSF_STR(0x00868E58, "uParRed_TamanyoPaquete")));
    matches = NameMatchesString(name, nullptr, temporary);
    DestroyTemporaryString(temporary, nullptr);
    if (matches)
    {
        FormatValueString(
            output, const_cast<char*>(CSF_STR(0x0083090C, "%d")),
            CSF_AT(std::int32_t, self, 8));
        return 1;
    }

    if (NameMatchesCString(
            name, nullptr,
            const_cast<char*>(CSF_STR(0x00868E40, "uParRed_TiempoPaquete"))))
    {
        return NetSpawnMessages_FormatPacketTime(
            reinterpret_cast<uint32_t*>(static_cast<std::uint8_t*>(self) + 12),
            output);
    }

    return 0;
}

HOOK(0x00752C50, NetSpawnMessages_FormatNetworkParameter, "auto");
