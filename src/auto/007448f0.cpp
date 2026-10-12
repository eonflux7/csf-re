#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NameMatchesCString, 0x0064A960, (void* self, void* edx, char* a1));
CSF_FUNC(char, __fastcall, NameMatchesString, 0x0064A9B0, (void* self, void* edx, void** a2));
CSF_FUNC(char, __fastcall, ReadRespawnMember, 0x0074B220, (void* self, void* edx, int a2, void** a3));
CSF_FUNC(char, __fastcall, NetPlayer_FormatReferenceMember, 0x0074B2B0, (void* self, void* edx, int a2, void** a3));
CSF_FUNC(char, __fastcall, NetPlayer_FormatDescriptionMember, 0x00751830, (void* self, void* edx, int name, void** output));
CSF_FUNC(char, __fastcall, NetSpawnMessages_FormatUnsignedValue, 0x007519A0, (std::uint16_t* self, void* , void* name, void** output));
CSF_FUNC(char, __fastcall, NetSpawnMessages_FormatLifeCount, 0x00751A00, (void* self, void* , void* name, void** output));
CSF_FUNC(char, __fastcall, NetPlayer_FormatActionsMember, 0x00751A70, (void* self, void* edx, int a2, void** a3));
CSF_FUNC(int, __fastcall, Lib_7A3090, 0x007A3090, (void* self, void* edx, int a1, int a2, int a3, int a4));

static char __fastcall NetPlayer_FormatMember(
    void* self, void* /*edx*/, Duplicated3DObject* object,
    void* nameContext, int memberName, void** output)
{
    auto* playerBytes = reinterpret_cast<std::uint8_t*>(object);
    void* comparisonName = static_cast<std::uint8_t*>(nameContext) + 8;
    void* valueName = reinterpret_cast<void*>(memberName);
    alignas(8) std::uint8_t temporaryStringBytes[0x4];

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(
            CSF_STR(0x00867D14, "m_dsArmaActiva"))));
    char matches = NameMatchesString(
        comparisonName, nullptr, reinterpret_cast<void**>(temporaryStringBytes));
    DestroyTemporaryString(temporaryStringBytes, nullptr);
    if (matches)
        return ReadRespawnMember(playerBytes + 0x440, nullptr, memberName, output);

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(
            CSF_STR(0x00867D08, "m_dsBando"))));
    matches = NameMatchesString(
        comparisonName, nullptr, reinterpret_cast<void**>(temporaryStringBytes));
    DestroyTemporaryString(temporaryStringBytes, nullptr);
    if (matches)
        return ReadRespawnMember(playerBytes + 0x443, nullptr, memberName, output);

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(
            CSF_STR(0x00867CF4, "m_dsBandoRespawn"))));
    matches = NameMatchesString(
        comparisonName, nullptr, reinterpret_cast<void**>(temporaryStringBytes));
    DestroyTemporaryString(temporaryStringBytes, nullptr);
    if (matches)
        return NetSpawnMessages_FormatUnsignedValue(
            reinterpret_cast<std::uint16_t*>(playerBytes + 0x446),
            nullptr, valueName, output);

    if (NameMatchesCString(comparisonName, nullptr,
            const_cast<char*>(CSF_STR(0x00867CDC, "m_dsIndicePorRepeticion"))))
        return ReadRespawnMember(playerBytes + 0x44C, nullptr, memberName, output);

    if (NameMatchesCString(comparisonName, nullptr,
            const_cast<char*>(CSF_STR(0x00867CCC, "m_dsItemActivo"))))
        return ReadRespawnMember(playerBytes + 0x44F, nullptr, memberName, output);

    if (NameMatchesCString(comparisonName, nullptr,
            const_cast<char*>(CSF_STR(0x00867CB8, "m_dsPlayerActions"))))
        return NetPlayer_FormatActionsMember(
            playerBytes + 0x452, nullptr, memberName, output);

    if (NameMatchesCString(comparisonName, nullptr,
            const_cast<char*>(CSF_STR(0x00867CA0, "m_dsPlayerDescription"))))
        return NetPlayer_FormatDescriptionMember(
            playerBytes + 0x458, nullptr, memberName, output);

    if (NameMatchesCString(comparisonName, nullptr,
            const_cast<char*>(CSF_STR(0x00867C84, "m_dsPlayerQueMeInterroga"))))
        return NetPlayer_FormatReferenceMember(
            playerBytes + 0x668, nullptr, memberName, output);

    if (NameMatchesCString(comparisonName, nullptr,
            const_cast<char*>(CSF_STR(0x00867C74, "m_dsUserFlags"))))
        return NetPlayer_FormatReferenceMember(
            playerBytes + 0x674, nullptr, memberName, output);

    if (NameMatchesCString(comparisonName, nullptr,
            const_cast<char*>(CSF_STR(0x00867C50, "m_dsValor_ESTJUG_AMIGOS_ASESINADOS"))))
        return NetSpawnMessages_FormatUnsignedValue(
            reinterpret_cast<std::uint16_t*>(playerBytes + 0x680),
            nullptr, valueName, output);

    if (NameMatchesCString(comparisonName, nullptr,
            const_cast<char*>(CSF_STR(0x00867C28, "m_dsValor_ESTJUG_BANDERAS_CAPTURADAS"))))
        return NetSpawnMessages_FormatUnsignedValue(
            reinterpret_cast<std::uint16_t*>(playerBytes + 0x686),
            nullptr, valueName, output);

    if (NameMatchesCString(comparisonName, nullptr,
            const_cast<char*>(CSF_STR(0x00867C00, "m_dsValor_ESTJUG_ENEMIGOS_ASESINADOS"))))
        return NetSpawnMessages_FormatUnsignedValue(
            reinterpret_cast<std::uint16_t*>(playerBytes + 0x68C),
            nullptr, valueName, output);

    if (NameMatchesCString(comparisonName, nullptr,
            const_cast<char*>(CSF_STR(0x00867BE0, "m_dsValor_ESTJUG_MUERTES_AMIGO"))))
        return NetSpawnMessages_FormatUnsignedValue(
            reinterpret_cast<std::uint16_t*>(playerBytes + 0x692),
            nullptr, valueName, output);

    if (NameMatchesCString(comparisonName, nullptr,
            const_cast<char*>(CSF_STR(0x00867BBC, "m_dsValor_ESTJUG_MUERTES_ENEMIGO"))))
        return NetSpawnMessages_FormatUnsignedValue(
            reinterpret_cast<std::uint16_t*>(playerBytes + 0x698),
            nullptr, valueName, output);

    if (NameMatchesCString(comparisonName, nullptr,
            const_cast<char*>(CSF_STR(0x00867BA0, "m_dsValor_ESTJUG_SUICIDIOS"))))
        return NetSpawnMessages_FormatUnsignedValue(
            reinterpret_cast<std::uint16_t*>(playerBytes + 0x69E),
            nullptr, valueName, output);

    if (NameMatchesCString(comparisonName, nullptr,
            const_cast<char*>(CSF_STR(0x00867B88, "m_dsValor_EndRespawn"))))
        return ReadRespawnMember(playerBytes + 0x6A4, nullptr, memberName, output);

    if (NameMatchesCString(comparisonName, nullptr,
            const_cast<char*>(CSF_STR(0x00867B7C, "m_dsVida"))))
        return NetSpawnMessages_FormatLifeCount(
            playerBytes + 0x6A8, nullptr, valueName, output);

    return static_cast<char>(Lib_7A3090(
        self, nullptr,
        static_cast<int>(reinterpret_cast<std::intptr_t>(object)),
        static_cast<int>(reinterpret_cast<std::intptr_t>(nameContext)),
        memberName,
        static_cast<int>(reinterpret_cast<std::intptr_t>(output))));
}

HOOK(0x007448F0, NetPlayer_FormatMember, "auto");
