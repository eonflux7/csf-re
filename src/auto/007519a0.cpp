#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(char*, __cdecl, FormatValueString, 0x0064A8F0, (void** a1, char* Format, ...));
CSF_FUNC(char, __fastcall, NameMatchesString, 0x0064A9B0, (void* self, void* edx, void** a2));

static char __fastcall NetSpawnMessages_FormatUnsignedValue(
    std::uint16_t* self, void* /*edx*/, void* name, void** output)
{
    alignas(8) std::uint8_t temporaryString[0x4];
    InitializeTemporaryString(
        temporaryString, nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(
            CSF_STR(0x008680E8, "m_uValor"))));

    const char matches = NameMatchesString(
        static_cast<std::uint8_t*>(name) + 8, nullptr,
        reinterpret_cast<void**>(temporaryString));
    DestroyTemporaryString(temporaryString, nullptr);

    if (!matches)
        return 0;

    FormatValueString(output, const_cast<char*>(CSF_STR(0x0083090C, "%d")),
                      static_cast<int>(CSF_AT(std::uint16_t, self, 0)));
    return 1;
}

HOOK(0x007519A0, NetSpawnMessages_FormatUnsignedValue, "auto");
