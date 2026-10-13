#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(char*, __cdecl, FormatValueString, 0x0064A8F0, (void** a1, char* Format, ...));
CSF_FUNC(char, __fastcall, NameMatchesString, 0x0064A9B0, (void* self, void* edx, void** a2));

// Compares the name stored at a2+8 against "m_uValor"; on match, formats the
// signed byte at this into a3 using "N/A" as the format string.
static char __fastcall ReadRespawnMember(char* self, void* /*edx*/, void* a2, void* a3)
{
    // Stack-owned temporary string object (4 bytes of frame room).
    alignas(8) std::uint8_t tempStringBytes[4];

    void* tempString = InitializeTemporaryString(
        tempStringBytes, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(CSF_STR(0x008680E8, "m_uValor"))));

    char nameMatches = NameMatchesString(
        static_cast<std::uint8_t*>(a2) + 8, nullptr,
        reinterpret_cast<void**>(tempString));

    DestroyTemporaryString(tempStringBytes, nullptr);

    if (nameMatches == 0)
        return 0;

    FormatValueString(
        reinterpret_cast<void**>(a3),
        const_cast<char*>(CSF_STR(0x008680F4, "N/A")),
        static_cast<int>(*self));
    return 1;
}

HOOK(0x0074B220, ReadRespawnMember, "auto");
