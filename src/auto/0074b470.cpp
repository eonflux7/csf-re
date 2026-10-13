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
CSF_FUNC(char, __fastcall, NetSpawnMessages_FormatFactionOrClassId, 0x007531E0, (void* self, void* , void* property, void** output));

static char __stdcall NetGhost_DispatchField(
    int objectAddress, int fieldAddress, int propertyAddress, int outputAddress)
{
    auto* object = reinterpret_cast<std::uint8_t*>(objectAddress);
    auto* fieldName = reinterpret_cast<std::uint8_t*>(fieldAddress) + 8;
    auto* output = reinterpret_cast<void**>(outputAddress);
    alignas(8) std::uint8_t temporaryStringBytes[0x4];

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(
            CSF_STR(0x0086819C, "m_dsBombaActivada"))));
    char matches = NameMatchesString(
        fieldName, nullptr, reinterpret_cast<void**>(temporaryStringBytes));
    DestroyTemporaryString(temporaryStringBytes, nullptr);
    if (matches)
        return ReadRespawnMember(object + 68, nullptr, propertyAddress, output);

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(
            CSF_STR(0x00868188, "m_dsBombaYaExploto"))));
    matches = NameMatchesString(
        fieldName, nullptr, reinterpret_cast<void**>(temporaryStringBytes));
    DestroyTemporaryString(temporaryStringBytes, nullptr);
    if (matches)
        return ReadRespawnMember(object + 71, nullptr, propertyAddress, output);

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(
            CSF_STR(0x0086817C, "m_dsEstado"))));
    matches = NameMatchesString(
        fieldName, nullptr, reinterpret_cast<void**>(temporaryStringBytes));
    DestroyTemporaryString(temporaryStringBytes, nullptr);
    if (matches)
        return ReadRespawnMember(object + 74, nullptr, propertyAddress, output);

    if (NameMatchesCString(
            fieldName, nullptr,
            const_cast<char*>(CSF_STR(0x00868164, "m_dsGhostDescription"))))
        return NetSpawnMessages_FormatFactionOrClassId(
            object + 80, nullptr, reinterpret_cast<void*>(propertyAddress), output);

    if (NameMatchesCString(
            fieldName, nullptr,
            const_cast<char*>(CSF_STR(0x00868144, "m_dsHandleBichoQueMeManipula"))))
        return NetPlayer_FormatReferenceMember(
            object + 92, nullptr, propertyAddress, output);

    if (NameMatchesCString(
            fieldName, nullptr,
            const_cast<char*>(CSF_STR(0x00868124, "m_dsTiempoParaExplosionBomba"))))
        return NetGame_FormatEndTimeMember(
            object + 104, nullptr, propertyAddress, output);

    return NetCogible_BaseDispatchField(
        objectAddress, fieldAddress, propertyAddress, outputAddress);
}

HOOK(0x0074B470, NetGhost_DispatchField, "auto");
