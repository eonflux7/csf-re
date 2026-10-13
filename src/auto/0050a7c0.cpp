#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0, (std::uint32_t component));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));

CSF_GLOBAL(const char*, g_pszKey_MUSIC, 0x00899A48);
CSF_GLOBAL(const char*, g_pszKey_FX, 0x00899A4C);
CSF_GLOBAL(const char*, g_pszKey_OUTPUT, 0x00899A50);
CSF_GLOBAL(const char*, g_pszKey_REVERB, 0x00899A54);
CSF_GLOBAL(const char*, g_pszKey_EAX, 0x00899A58);
CSF_GLOBAL(const char*, g_pszKey_SOUND, 0x00899A8C);

static char __fastcall S_0050a7c0_LoadSoundOptions(
    S_0050a7c0* self, void* /*edx*/, S_00692b80* options)
{
    // Packet-listed binding; the actual instructions call this entry directly.
    (void)CSF_VTBL(0x00692B80);

    auto* sound = FindProperty(
        reinterpret_cast<std::uint8_t*>(options), nullptr,
        const_cast<char*>(g_pszKey_SOUND));
    if (!sound)
        return 0;

    if (MenuExtras_GetChildCount(sound, nullptr) != 5)
    {
        MenuExtras_GetChildCount(sound, nullptr);
        return 0;
    }

    auto* music = FindProperty(sound, nullptr, const_cast<char*>(g_pszKey_MUSIC));
    if (!music)
        return 0;
    CSF_AT(float, self, 0x04) =
        static_cast<float>(ReadComponentFloat(reinterpret_cast<std::uint32_t>(music)));

    auto* effects = FindProperty(sound, nullptr, const_cast<char*>(g_pszKey_FX));
    if (!effects)
        return 0;
    CSF_AT(float, self, 0x08) =
        static_cast<float>(ReadComponentFloat(reinterpret_cast<std::uint32_t>(effects)));

    auto* output = FindProperty(sound, nullptr, const_cast<char*>(g_pszKey_OUTPUT));
    if (!output)
        return 0;
    const int outputMode = ParseIntegerComponent(reinterpret_cast<std::uint32_t>(output));
    CSF_AT(int, self, 0x0C) = outputMode;
    if (!outputMode)
        CSF_AT(int, self, 0x0C) = 1;

    auto* reverb = FindProperty(sound, nullptr, const_cast<char*>(g_pszKey_REVERB));
    if (!reverb)
        return 0;
    CSF_AT(std::uint8_t, self, 0x10) =
        ParseIntegerComponent(reinterpret_cast<std::uint32_t>(reverb)) == 1;

    auto* eax = FindProperty(sound, nullptr, const_cast<char*>(g_pszKey_EAX));
    if (eax)
        CSF_AT(std::uint8_t, self, 0x11) =
            ParseIntegerComponent(reinterpret_cast<std::uint32_t>(eax)) == 1;

    return 1;
}

HOOK(0x0050A7C0, S_0050a7c0_LoadSoundOptions, "auto");
