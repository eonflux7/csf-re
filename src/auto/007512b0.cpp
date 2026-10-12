#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(char*, __cdecl, FormatValueString, 0x0064A8F0, (void** a1, char* Format, ...));
CSF_FUNC(char, __fastcall, NameMatchesCString, 0x0064A960, (void* self, void* edx, char* a1));
CSF_FUNC(char, __fastcall, NameMatchesString, 0x0064A9B0, (void* self, void* edx, void** a2));
CSF_FUNC(char, __cdecl, NetSpawnMessages_FormatUnsignedValue, 0x0066C010, (uint32_t* a1, void** a2));
CSF_FUNC(char, __cdecl, NetSpawnMessages_FormatFloatValue, 0x0068E670, (float* a1, void** a2));

static char __fastcall NetSpawnMessages_FormatFieldValue(
    void* self, void* /*edx*/, void* nameOwner, void** output)
{
    void* name = static_cast<std::uint8_t*>(nameOwner) + 8;

    // The original reuses its first four-byte argument slot as a string.
    alignas(8) std::uint8_t temporaryBytes[4];
    void** temporary = reinterpret_cast<void**>(temporaryBytes);
    *temporary = nameOwner;

    InitializeTemporaryString(
        temporary, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(
            CSF_STR(0x00868A6C, "m_ArmaIndex"))));
    char matches = NameMatchesString(name, nullptr, temporary);
    DestroyTemporaryString(temporary, nullptr);
    if (matches)
    {
        FormatValueString(output, const_cast<char*>(CSF_STR(0x0083090C, "%d")),
                          CSF_AT(std::int32_t, self, 0));
        return 1;
    }

    InitializeTemporaryString(
        temporary, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(
            CSF_STR(0x00868A60, "m_TipoBicho"))));
    matches = NameMatchesString(name, nullptr, temporary);
    DestroyTemporaryString(temporary, nullptr);
    if (matches)
    {
        FormatValueString(output, const_cast<char*>(CSF_STR(0x0083090C, "%d")),
                          static_cast<int>(CSF_AT(std::uint16_t, self, 4)));
        return 1;
    }

    InitializeTemporaryString(
        temporary, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(
            CSF_STR(0x00868A50, "m_TipoProyectil"))));
    matches = NameMatchesString(name, nullptr, temporary);
    DestroyTemporaryString(temporary, nullptr);
    if (matches)
    {
        FormatValueString(output, const_cast<char*>(CSF_STR(0x0083090C, "%d")),
                          static_cast<int>(CSF_AT(std::uint16_t, self, 6)));
        return 1;
    }

    if (NameMatchesCString(name, nullptr,
                           const_cast<char*>(CSF_STR(0x00868A40, "m_fPosXInicial"))))
        return NetSpawnMessages_FormatFloatValue(&CSF_AT(float, self, 8), output);

    if (NameMatchesCString(name, nullptr,
                           const_cast<char*>(CSF_STR(0x00868A30, "m_fPosYInicial"))))
        return NetSpawnMessages_FormatFloatValue(&CSF_AT(float, self, 12), output);

    if (NameMatchesCString(name, nullptr,
                           const_cast<char*>(CSF_STR(0x00868A20, "m_fPosZInicial"))))
        return NetSpawnMessages_FormatFloatValue(&CSF_AT(float, self, 16), output);

    if (NameMatchesCString(name, nullptr,
                           const_cast<char*>(CSF_STR(0x00868A10, "m_fVelXInicial"))))
        return NetSpawnMessages_FormatFloatValue(&CSF_AT(float, self, 20), output);

    if (NameMatchesCString(name, nullptr,
                           const_cast<char*>(CSF_STR(0x00868A00, "m_fVelYInicial"))))
        return NetSpawnMessages_FormatFloatValue(&CSF_AT(float, self, 24), output);

    if (NameMatchesCString(name, nullptr,
                           const_cast<char*>(CSF_STR(0x008689F0, "m_fVelZInicial"))))
        return NetSpawnMessages_FormatFloatValue(&CSF_AT(float, self, 28), output);

    if (NameMatchesCString(name, nullptr,
                           const_cast<char*>(CSF_STR(0x008689E4, "m_uClassID"))))
        return NetSpawnMessages_FormatUnsignedValue(
            &CSF_AT(uint32_t, self, 32), output);

    if (NameMatchesCString(name, nullptr,
                           const_cast<char*>(CSF_STR(0x008689D4, "m_uHandleDueno"))))
        return NetSpawnMessages_FormatUnsignedValue(
            &CSF_AT(uint32_t, self, 36), output);

    return 0;
}

HOOK(0x007512B0, NetSpawnMessages_FormatFieldValue, "auto");
