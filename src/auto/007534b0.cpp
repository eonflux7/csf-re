#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(char*, __cdecl, FormatValueString, 0x0064A8F0, (void** a1, char* Format, ...));
CSF_FUNC(char, __fastcall, NameMatchesCString, 0x0064A960, (void* self, void* edx, char* a1));
CSF_FUNC(char, __fastcall, NameMatchesString, 0x0064A9B0, (void* self, void* edx, void** a2));
CSF_FUNC(char, __cdecl, NetSpawnMessages_FormatPlayerValue, 0x0066C010, (uint32_t* a1, void** a2));

static char __fastcall NetSpawnMessages_FormatPlayerField(
    void* self, void* /*edx*/, void* nameObject, void** output)
{
    void* name = static_cast<std::uint8_t*>(nameObject) + 8;

    InitializeTemporaryString(
        &nameObject, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x008690F4, "m_bHaCambiado"))));
    char matches = NameMatchesString(name, nullptr, &nameObject);
    DestroyTemporaryString(&nameObject, nullptr);
    if (matches)
    {
        FormatValueString(output, const_cast<char*>(CSF_STR(0x0083090C, "%d")),
                          static_cast<int>(CSF_AT(std::uint8_t, self, 0)));
        return 1;
    }

    InitializeTemporaryString(
        &nameObject, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x008690E8, "m_uPlayer00"))));
    matches = NameMatchesString(name, nullptr, &nameObject);
    DestroyTemporaryString(&nameObject, nullptr);
    if (matches)
    {
        FormatValueString(output, const_cast<char*>(CSF_STR(0x0083090C, "%d")),
                          CSF_AT(std::int32_t, self, 4));
        return 1;
    }

    InitializeTemporaryString(
        &nameObject, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x008690DC, "m_uPlayer01"))));
    matches = NameMatchesString(name, nullptr, &nameObject);
    DestroyTemporaryString(&nameObject, nullptr);
    if (matches)
    {
        FormatValueString(output, const_cast<char*>(CSF_STR(0x0083090C, "%d")),
                          CSF_AT(std::int32_t, self, 8));
        return 1;
    }

    if (NameMatchesCString(name, nullptr, const_cast<char*>(CSF_STR(0x008690D0, "m_uPlayer02"))))
        return NetSpawnMessages_FormatPlayerValue(&CSF_AT(uint32_t, self, 12), output);
    if (NameMatchesCString(name, nullptr, const_cast<char*>(CSF_STR(0x008690C4, "m_uPlayer03"))))
        return NetSpawnMessages_FormatPlayerValue(&CSF_AT(uint32_t, self, 16), output);
    if (NameMatchesCString(name, nullptr, const_cast<char*>(CSF_STR(0x008690B8, "m_uPlayer04"))))
        return NetSpawnMessages_FormatPlayerValue(&CSF_AT(uint32_t, self, 20), output);
    if (NameMatchesCString(name, nullptr, const_cast<char*>(CSF_STR(0x008690AC, "m_uPlayer05"))))
        return NetSpawnMessages_FormatPlayerValue(&CSF_AT(uint32_t, self, 24), output);
    if (NameMatchesCString(name, nullptr, const_cast<char*>(CSF_STR(0x008690A0, "m_uPlayer06"))))
        return NetSpawnMessages_FormatPlayerValue(&CSF_AT(uint32_t, self, 28), output);
    if (NameMatchesCString(name, nullptr, const_cast<char*>(CSF_STR(0x00869094, "m_uPlayer07"))))
        return NetSpawnMessages_FormatPlayerValue(&CSF_AT(uint32_t, self, 32), output);
    if (NameMatchesCString(name, nullptr, const_cast<char*>(CSF_STR(0x00869088, "m_uPlayer08"))))
        return NetSpawnMessages_FormatPlayerValue(&CSF_AT(uint32_t, self, 36), output);
    if (NameMatchesCString(name, nullptr, const_cast<char*>(CSF_STR(0x0086907C, "m_uPlayer09"))))
        return NetSpawnMessages_FormatPlayerValue(&CSF_AT(uint32_t, self, 40), output);
    if (NameMatchesCString(name, nullptr, const_cast<char*>(CSF_STR(0x00869070, "m_uPlayer10"))))
        return NetSpawnMessages_FormatPlayerValue(&CSF_AT(uint32_t, self, 44), output);
    if (NameMatchesCString(name, nullptr, const_cast<char*>(CSF_STR(0x00869064, "m_uPlayer11"))))
        return NetSpawnMessages_FormatPlayerValue(&CSF_AT(uint32_t, self, 48), output);
    if (NameMatchesCString(name, nullptr, const_cast<char*>(CSF_STR(0x00869058, "m_uPlayer12"))))
        return NetSpawnMessages_FormatPlayerValue(&CSF_AT(uint32_t, self, 52), output);
    if (NameMatchesCString(name, nullptr, const_cast<char*>(CSF_STR(0x0086904C, "m_uPlayer13"))))
        return NetSpawnMessages_FormatPlayerValue(&CSF_AT(uint32_t, self, 56), output);
    if (NameMatchesCString(name, nullptr, const_cast<char*>(CSF_STR(0x00869040, "m_uPlayer14"))))
        return NetSpawnMessages_FormatPlayerValue(&CSF_AT(uint32_t, self, 60), output);
    if (NameMatchesCString(name, nullptr, const_cast<char*>(CSF_STR(0x00869034, "m_uPlayer15"))))
        return NetSpawnMessages_FormatPlayerValue(&CSF_AT(uint32_t, self, 64), output);
    if (NameMatchesCString(name, nullptr, const_cast<char*>(CSF_STR(0x00869028, "m_uPlayer16"))))
        return NetSpawnMessages_FormatPlayerValue(&CSF_AT(uint32_t, self, 68), output);

    return 0;
}

HOOK(0x007534B0, NetSpawnMessages_FormatPlayerField, "auto");
