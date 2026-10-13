#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NameMatchesCString, 0x0064A960, (void* self, void* edx, char* a1));
CSF_FUNC(char, __fastcall, NameMatchesString, 0x0064A9B0, (void* self, void* edx, void** a2));
CSF_FUNC(char, __stdcall, NetCogible_BaseDispatchField, 0x0065E6F0, (int a1, int a2, int a3, int a4));
CSF_FUNC(char, __fastcall, NetGame_FormatEndTimeMember, 0x0074B160, (void* self, void* edx, int a2, void** a3));
CSF_FUNC(char, __fastcall, NetPlayer_FormatReferenceMember, 0x0074B2B0, (void* self, void* edx, int a2, void** a3));
CSF_FUNC(char, __fastcall, NetSpawnMessages_FormatInitialSpawnField, 0x007538D0, (void* self, void* , void* nameObject, void** output));

static char __stdcall NetPostman_FormatField(
    void* object, void* fieldRequest, int nameObject, void** output)
{
    auto* members = static_cast<std::uint8_t*>(object);
    void* requestedName = static_cast<std::uint8_t*>(fieldRequest) + 8;
    alignas(8) std::uint8_t temporaryStringBytes[0x4];
    auto* temporaryString = reinterpret_cast<void**>(temporaryStringBytes);

    InitializeTemporaryString(
        temporaryString, nullptr,
        reinterpret_cast<std::int32_t>(CSF_STR(0x00868850, "m_dsGrupoAlQueVoy")));
    char matches = NameMatchesString(requestedName, nullptr, temporaryString);
    DestroyTemporaryString(temporaryString, nullptr);
    if (matches)
        return NetPlayer_FormatReferenceMember(members + 0x44, nullptr, nameObject, output);

    InitializeTemporaryString(
        temporaryString, nullptr,
        reinterpret_cast<std::int32_t>(CSF_STR(0x0086883C, "m_dsGrupoSiguiente")));
    matches = NameMatchesString(requestedName, nullptr, temporaryString);
    DestroyTemporaryString(temporaryString, nullptr);
    if (matches)
        return NetPlayer_FormatReferenceMember(members + 0x50, nullptr, nameObject, output);

    InitializeTemporaryString(
        temporaryString, nullptr,
        reinterpret_cast<std::int32_t>(CSF_STR(0x00868824, "m_dsPostmanDescription")));
    matches = NameMatchesString(requestedName, nullptr, temporaryString);
    DestroyTemporaryString(temporaryString, nullptr);
    if (matches)
        return NetSpawnMessages_FormatInitialSpawnField(
            members + 0x5C, nullptr, reinterpret_cast<void*>(nameObject), output);

    if (NameMatchesCString(
            requestedName, nullptr,
            const_cast<char*>(CSF_STR(0x00868810, "m_dsPuntoAlQueVoy"))))
        return NetPlayer_FormatReferenceMember(members + 0x6C, nullptr, nameObject, output);

    if (NameMatchesCString(
            requestedName, nullptr,
            const_cast<char*>(CSF_STR(0x008687FC, "m_dsPuntoSiguiente"))))
        return NetPlayer_FormatReferenceMember(members + 0x78, nullptr, nameObject, output);

    if (NameMatchesCString(
            requestedName, nullptr,
            const_cast<char*>(CSF_STR(0x008687E8, "m_dsTiempoLlegar"))))
        return NetGame_FormatEndTimeMember(members + 0x88, nullptr, nameObject, output);

    if (NameMatchesCString(
            requestedName, nullptr,
            const_cast<char*>(CSF_STR(0x008687D8, "m_dsTiempoSalir"))))
        return NetGame_FormatEndTimeMember(members + 0xA0, nullptr, nameObject, output);

    return NetCogible_BaseDispatchField(
        reinterpret_cast<int>(object), reinterpret_cast<int>(fieldRequest),
        nameObject, reinterpret_cast<int>(output));
}

HOOK(0x0074F8D0, NetPostman_FormatField, "auto");
