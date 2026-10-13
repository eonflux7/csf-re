#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(const char*, g_pszKey_JUEGO, 0x00899A98);
CSF_GLOBAL(const char*, g_pszKey_VOICE_MUTED, 0x00899A38);

CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));

// Reads the VOICE_MUTED property under the JUEGO section of an options file.
// Stores (value != 0) into this+0x10 and returns 1 when found, 0 otherwise.
static char __fastcall ReadVoiceMutedFlagFromOptions(S_0050a360* self, void* /*edx*/, S_00692b80* optionsRoot)
{
    std::uint8_t* gameSection = FindProperty(
        reinterpret_cast<std::uint8_t*>(optionsRoot), nullptr,
        const_cast<char*>(g_pszKey_JUEGO));

    std::uint8_t* voiceMutedProperty = FindProperty(
        gameSection, nullptr,
        const_cast<char*>(g_pszKey_VOICE_MUTED));

    if (!voiceMutedProperty)
        return 0;

    int parsedValue = ParseIntegerComponent(
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(voiceMutedProperty)));
    CSF_AT(std::uint8_t, self, 0x10) = (parsedValue != 0) ? 1 : 0;
    return 1;
}

HOOK(0x0050B0D0, ReadVoiceMutedFlagFromOptions, "auto");
