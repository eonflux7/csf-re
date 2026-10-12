#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(char*, __cdecl, FormatValueString, 0x0064A8F0, (void** a1, char* Format, ...));
CSF_FUNC(char, __fastcall, NameMatchesString, 0x0064A9B0, (void* self, void* edx, void** a2));

static char __fastcall NetSpawnMessages_FormatPlayerDescriptionMember(
    void* self, void* /*edx*/, void* member, void** output)
{
    void* memberName = static_cast<std::uint8_t*>(member) + 8;

    // The original constructs its temporary over the two stack arguments.
    alignas(8) std::uint8_t temporaryBytes[8];
    CSF_AT(void*, temporaryBytes, 0) = member;
    CSF_AT(void**, temporaryBytes, 4) = output;

    InitializeTemporaryString(
        temporaryBytes, nullptr,
        reinterpret_cast<std::int32_t>(CSF_STR(0x00868A88, "m_TipoPlayerMP")));
    char matches = NameMatchesString(
        memberName, nullptr, reinterpret_cast<void**>(temporaryBytes));
    DestroyTemporaryString(temporaryBytes, nullptr);
    if (matches)
    {
        FormatValueString(
            CSF_AT(void**, temporaryBytes, 4),
            const_cast<char*>(CSF_STR(0x008680F4, "N/A")),
            static_cast<int>(CSF_AT(std::int8_t, self, 0)));
        return 1;
    }

    InitializeTemporaryString(
        temporaryBytes, nullptr,
        reinterpret_cast<std::int32_t>(CSF_STR(0x008689E4, "m_uClassID")));
    matches = NameMatchesString(
        memberName, nullptr, reinterpret_cast<void**>(temporaryBytes));
    DestroyTemporaryString(temporaryBytes, nullptr);
    if (matches)
    {
        FormatValueString(
            CSF_AT(void**, temporaryBytes, 4),
            const_cast<char*>(CSF_STR(0x0083090C, "%d")),
            CSF_AT(std::int32_t, self, 0x104));
        return 1;
    }

    InitializeTemporaryString(
        temporaryBytes, nullptr,
        reinterpret_cast<std::int32_t>(CSF_STR(0x00868A78, "m_uStationID")));
    matches = NameMatchesString(
        memberName, nullptr, reinterpret_cast<void**>(temporaryBytes));
    DestroyTemporaryString(temporaryBytes, nullptr);
    if (matches)
    {
        FormatValueString(
            CSF_AT(void**, temporaryBytes, 4),
            const_cast<char*>(CSF_STR(0x0083090C, "%d")),
            CSF_AT(std::int32_t, self, 0x108));
        return 1;
    }

    return 0;
}

HOOK(0x00751830, NetSpawnMessages_FormatPlayerDescriptionMember, "auto");
