#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NameMatchesCString, 0x0064A960, (void* self, void* edx, char* a1));
CSF_FUNC(char, __fastcall, NameMatchesString, 0x0064A9B0, (void* self, void* edx, void** a2));
CSF_FUNC(char, __stdcall, NetCogible_BaseDispatchField, 0x0065E6F0, (int a1, int a2, int a3, int a4));
CSF_FUNC(char, __fastcall, NetGame_FormatEndTimeMember, 0x0074B160, (void* self, void* edx, int a2, void** a3));
CSF_FUNC(char, __fastcall, ReadRespawnMember, 0x0074B220, (void* self, void* edx, int a2, void** a3));
CSF_FUNC(char, __fastcall, NetPlayer_FormatReferenceMember, 0x0074B2B0, (void* self, void* edx, int a2, void** a3));
CSF_FUNC(char, __fastcall, NetSpawnMessages_FormatUnsignedValue, 0x007519A0, (std::uint16_t* self, void* , void* name, void** output));
CSF_FUNC(char, __fastcall, NetSpawnMessages_FormatSessionField, 0x00752DC0, (char* self, void* edx, void* nameObject, void** destination));

static char __stdcall NetGame_FormatField(
    int objectAddress, int nameAddress, int memberNameAddress, int outputAddress)
{
    auto* object = reinterpret_cast<std::uint8_t*>(objectAddress);
    void* name = reinterpret_cast<std::uint8_t*>(nameAddress) + 8;
    void* memberName = reinterpret_cast<void*>(memberNameAddress);
    auto* output = reinterpret_cast<void**>(outputAddress);

    alignas(8) std::uint8_t temporaryStringBytes[0x4];
    auto* temporaryString = reinterpret_cast<void**>(temporaryStringBytes);

    InitializeTemporaryString(
        temporaryString, nullptr,
        reinterpret_cast<std::int32_t>(CSF_STR(0x00868420, "m_dsClaveBando1")));
    // The initializer returns its self pointer.
    char matches = NameMatchesString(name, nullptr, temporaryString);
    DestroyTemporaryString(temporaryString, nullptr);
    if (matches)
        return NetPlayer_FormatReferenceMember(
            object + 68, nullptr, memberNameAddress, output);

    InitializeTemporaryString(
        temporaryString, nullptr,
        reinterpret_cast<std::int32_t>(CSF_STR(0x00868410, "m_dsClaveBando2")));
    matches = NameMatchesString(name, nullptr, temporaryString);
    DestroyTemporaryString(temporaryString, nullptr);
    if (matches)
        return NetPlayer_FormatReferenceMember(
            object + 80, nullptr, memberNameAddress, output);

    InitializeTemporaryString(
        temporaryString, nullptr,
        reinterpret_cast<std::int32_t>(CSF_STR(0x008683FC, "m_dsEstadoPartida")));
    matches = NameMatchesString(name, nullptr, temporaryString);
    DestroyTemporaryString(temporaryString, nullptr);
    if (matches)
        return ReadRespawnMember(object + 92, nullptr, memberNameAddress, output);

    if (NameMatchesCString(
            name, nullptr,
            const_cast<char*>(CSF_STR(0x008683D0, "m_dsMasterHaEntradoEnSesionRecienConectados"))))
        return ReadRespawnMember(object + 95, nullptr, memberNameAddress, output);

    if (NameMatchesCString(
            name, nullptr,
            const_cast<char*>(CSF_STR(0x008683BC, "m_dsOpcionesJuego"))))
        return NetSpawnMessages_FormatSessionField(
            reinterpret_cast<char*>(object + 100), nullptr, memberName, output);

    if (NameMatchesCString(
            name, nullptr,
            const_cast<char*>(CSF_STR(0x008683AC, "m_dsPartidaID"))))
        return NetPlayer_FormatReferenceMember(
            object + 696, nullptr, memberNameAddress, output);

    if (NameMatchesCString(
            name, nullptr,
            const_cast<char*>(CSF_STR(0x00868394, "m_dsPostmenQueHanSalido"))))
        return NetSpawnMessages_FormatUnsignedValue(
            reinterpret_cast<std::uint16_t*>(object + 708),
            nullptr, memberName, output);

    if (NameMatchesCString(
            name, nullptr,
            const_cast<char*>(CSF_STR(0x00868378, "m_dsPuntoRespawnAlemanes"))))
        return NetSpawnMessages_FormatUnsignedValue(
            reinterpret_cast<std::uint16_t*>(object + 714),
            nullptr, memberName, output);

    if (NameMatchesCString(
            name, nullptr,
            const_cast<char*>(CSF_STR(0x00868360, "m_dsPuntoRespawnAliados"))))
        return NetSpawnMessages_FormatUnsignedValue(
            reinterpret_cast<std::uint16_t*>(object + 720),
            nullptr, memberName, output);

    if (NameMatchesCString(
            name, nullptr,
            const_cast<char*>(CSF_STR(0x0086834C, "m_dsPuntosBando1"))))
        return NetSpawnMessages_FormatUnsignedValue(
            reinterpret_cast<std::uint16_t*>(object + 726),
            nullptr, memberName, output);

    if (NameMatchesCString(
            name, nullptr,
            const_cast<char*>(CSF_STR(0x00868338, "m_dsPuntosBando2"))))
        return NetSpawnMessages_FormatUnsignedValue(
            reinterpret_cast<std::uint16_t*>(object + 732),
            nullptr, memberName, output);

    if (NameMatchesCString(
            name, nullptr,
            const_cast<char*>(CSF_STR(0x00868318, "m_dsTiempoFinalPartidaActual"))))
        return NetGame_FormatEndTimeMember(
            object + 744, nullptr, memberNameAddress, output);

    if (NameMatchesCString(
            name, nullptr,
            const_cast<char*>(CSF_STR(0x00868308, "m_dsVictoria"))))
        return ReadRespawnMember(object + 768, nullptr, memberNameAddress, output);

    return NetCogible_BaseDispatchField(
        objectAddress, nameAddress, memberNameAddress, outputAddress);
}

HOOK(0x0074C760, NetGame_FormatField, "auto");
