#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(char*, __cdecl, FormatValueString, 0x0064A8F0, (void** a1, char* Format, ...));
CSF_FUNC(char, __fastcall, NameMatchesCString, 0x0064A960, (void* self, void* edx, char* a1));
CSF_FUNC(char, __fastcall, NameMatchesString, 0x0064A9B0, (void* self, void* edx, void** a2));
CSF_FUNC(char, __cdecl, NetSpawnMessages_FormatInitialPoint, 0x0066BF70, (uint16_t* a1, void** a2));
CSF_FUNC(char, __cdecl, NetSpawnMessages_FormatUnsigned32Value, 0x0066C010, (uint32_t* a1, void** a2));
CSF_FUNC(char, __cdecl, NetSpawnMessages_FormatCharacterValue, 0x0074B1E0, (char* a1, void** a2));
CSF_FUNC(char, __cdecl, NetSpawnMessages_FormatByteValue, 0x00752500, (char* value, int destination));

static char __fastcall NetSpawnMessages_FormatSessionField(
    char* self, void* /*edx*/, void* nameObject, void** destination)
{
    void* name = static_cast<uint8_t*>(nameObject) + 8;

    // The original reuses the first argument's slot for its temporary string.
    InitializeTemporaryString(
        &nameObject, nullptr,
        reinterpret_cast<std::int32_t>(CSF_STR(0x00868F6C, "m_bComando")));
    char matches = NameMatchesString(name, nullptr, &nameObject);
    DestroyTemporaryString(&nameObject, nullptr);
    if (matches)
    {
        FormatValueString(destination, const_cast<char*>(CSF_STR(0x0083090C, "%d")),
                          static_cast<int>(CSF_AT(uint8_t, self, 0)));
        return 1;
    }

    InitializeTemporaryString(
        &nameObject, nullptr,
        reinterpret_cast<std::int32_t>(CSF_STR(0x00868F60, "m_bEspia")));
    matches = NameMatchesString(name, nullptr, &nameObject);
    DestroyTemporaryString(&nameObject, nullptr);
    if (matches)
    {
        FormatValueString(destination, const_cast<char*>(CSF_STR(0x0083090C, "%d")),
                          static_cast<int>(CSF_AT(uint8_t, self, 1)));
        return 1;
    }

    InitializeTemporaryString(
        &nameObject, nullptr,
        reinterpret_cast<std::int32_t>(CSF_STR(0x00868F54, "m_bFranco")));
    matches = NameMatchesString(name, nullptr, &nameObject);
    DestroyTemporaryString(&nameObject, nullptr);
    if (matches)
    {
        FormatValueString(destination, const_cast<char*>(CSF_STR(0x0083090C, "%d")),
                          static_cast<int>(CSF_AT(uint8_t, self, 2)));
        return 1;
    }

    if (NameMatchesCString(name, nullptr,
                           const_cast<char*>(CSF_STR(0x00868F44, "m_bFuegoAmigo"))))
        return NetSpawnMessages_FormatByteValue(
            self + 3, reinterpret_cast<int>(destination));

    if (NameMatchesCString(name, nullptr,
                           const_cast<char*>(CSF_STR(0x00868F3C, "m_bLan"))))
        return NetSpawnMessages_FormatByteValue(
            self + 4, reinterpret_cast<int>(destination));

    if (NameMatchesCString(name, nullptr,
                           const_cast<char*>(CSF_STR(0x00868F2C, "m_cMaxPlayers"))))
        return NetSpawnMessages_FormatCharacterValue(self + 5, destination);

    if (NameMatchesCString(name, nullptr,
                           const_cast<char*>(CSF_STR(0x00868F1C, "m_cModoJuego"))))
        return NetSpawnMessages_FormatCharacterValue(self + 6, destination);

    if (NameMatchesCString(name, nullptr,
                           const_cast<char*>(CSF_STR(0x00868F0C, "m_uTiempoDMatch"))))
        return NetSpawnMessages_FormatUnsigned32Value(
            reinterpret_cast<uint32_t*>(self + 392), destination);

    if (NameMatchesCString(name, nullptr,
                           const_cast<char*>(CSF_STR(0x00868EF8, "m_uTiempoSabotage"))))
        return NetSpawnMessages_FormatUnsigned32Value(
            reinterpret_cast<uint32_t*>(self + 396), destination);

    if (NameMatchesCString(name, nullptr,
                           const_cast<char*>(CSF_STR(0x00868EE4, "m_uTiempoTDMatch"))))
        return NetSpawnMessages_FormatUnsigned32Value(
            reinterpret_cast<uint32_t*>(self + 400), destination);

    if (NameMatchesCString(name, nullptr,
                           const_cast<char*>(CSF_STR(0x00868ED4, "m_usCantPostmen"))))
        return NetSpawnMessages_FormatInitialPoint(
            reinterpret_cast<uint16_t*>(self + 404), destination);

    if (NameMatchesCString(name, nullptr,
                           const_cast<char*>(CSF_STR(0x00868EC4, "m_usFragsDMatch"))))
        return NetSpawnMessages_FormatInitialPoint(
            reinterpret_cast<uint16_t*>(self + 406), destination);

    if (NameMatchesCString(name, nullptr,
                           const_cast<char*>(CSF_STR(0x00868EB0, "m_usFragsTDMatch"))))
        return NetSpawnMessages_FormatInitialPoint(
            reinterpret_cast<uint16_t*>(self + 408), destination);

    if (NameMatchesCString(name, nullptr,
                           const_cast<char*>(CSF_STR(0x00868EA0, "m_usFreqPostmen"))))
        return NetSpawnMessages_FormatInitialPoint(
            reinterpret_cast<uint16_t*>(self + 410), destination);

    if (NameMatchesCString(name, nullptr,
                           const_cast<char*>(CSF_STR(0x00868E8C, "m_usPuntosSabotage"))))
        return NetSpawnMessages_FormatInitialPoint(
            reinterpret_cast<uint16_t*>(self + 412), destination);

    return 0;
}

HOOK(0x00752DC0, NetSpawnMessages_FormatSessionField, "auto");
