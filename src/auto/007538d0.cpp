#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(char*, __cdecl, LegacyString_Format, 0x0064A8F0, (void** a1, char* Format, ...));
CSF_FUNC(char, __fastcall, NameMatchesCString, 0x0064A960, (void* self, void* edx, char* a1));
CSF_FUNC(char, __fastcall, NameMatchesString, 0x0064A9B0, (void* self, void* edx, void** a2));
CSF_FUNC(char, __cdecl, NetSpawnMessages_FormatInitialPoint, 0x0066BF70, (uint16_t* a1, void** a2));

static char __fastcall NetSpawnMessages_FormatInitialSpawnField(
    void* self, void* /*edx*/, void* nameObject, void** output)
{
    void* fieldName = static_cast<std::uint8_t*>(nameObject) + 8;

    InitializeTemporaryString(
        &nameObject, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(
            CSF_STR(0x00868F78, "m_cBando"))));
    char matches = NameMatchesString(fieldName, nullptr, &nameObject);
    DestroyTemporaryString(&nameObject, nullptr);
    if (matches)
    {
        LegacyString_Format(
            output, const_cast<char*>(CSF_STR(0x008680F4, "N/A")),
            static_cast<int>(CSF_AT(std::int8_t, self, 0x0)));
        return 1;
    }

    InitializeTemporaryString(
        &nameObject, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(
            CSF_STR(0x008689E4, "m_uClassID"))));
    matches = NameMatchesString(fieldName, nullptr, &nameObject);
    DestroyTemporaryString(&nameObject, nullptr);
    if (matches)
    {
        LegacyString_Format(
            output, const_cast<char*>(CSF_STR(0x0083090C, "%d")),
            CSF_AT(std::int32_t, self, 0x4));
        return 1;
    }

    InitializeTemporaryString(
        &nameObject, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(
            CSF_STR(0x00869114, "m_wGrupoInicial"))));
    matches = NameMatchesString(fieldName, nullptr, &nameObject);
    DestroyTemporaryString(&nameObject, nullptr);
    if (matches)
    {
        LegacyString_Format(
            output, const_cast<char*>(CSF_STR(0x0083090C, "%d")),
            static_cast<int>(CSF_AT(std::uint16_t, self, 0x8)));
        return 1;
    }

    if (NameMatchesCString(
            fieldName, nullptr,
            const_cast<char*>(CSF_STR(0x00869104, "m_wPuntoInicial"))))
    {
        return NetSpawnMessages_FormatInitialPoint(
            reinterpret_cast<uint16_t*>(static_cast<std::uint8_t*>(self) + 0xA),
            output);
    }

    return 0;
}

HOOK(0x007538D0, NetSpawnMessages_FormatInitialSpawnField, "auto");
