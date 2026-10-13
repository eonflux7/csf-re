#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(void, __fastcall, CoreZlibLog_ReadVector2, 0x00692F60, (void* self, void* edx, float* a2));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));

CSF_GLOBAL(const char*, g_pszKey_AUTO_AIM, 0x00899A24);
CSF_GLOBAL(const char*, g_pszKey_SUBTITLES, 0x00899A28);
CSF_GLOBAL(const char*, g_pszKey_INVERT_VIEW, 0x00899A2C);
CSF_GLOBAL(const char*, g_pszKey_SENSIBILIDAD, 0x00899A30);
CSF_GLOBAL(const char*, g_pszKey_GAME_VIBRATION, 0x00899A34);
CSF_GLOBAL(const char*, g_pszKey_JUEGO, 0x00899A98);

static char __fastcall S_0050a360_ReadGameOptions(
    S_0050a360* self, void* /*edx*/, S_00692b80* root)
{
    auto* gameOptions = FindProperty(
        reinterpret_cast<std::uint8_t*>(root), nullptr,
        const_cast<char*>(g_pszKey_JUEGO));
    if (!gameOptions)
        return 0;

    auto* property = FindProperty(
        gameOptions, nullptr, const_cast<char*>(g_pszKey_AUTO_AIM));
    if (!property)
        return 0;
    self->field_0004 =
        ParseIntegerComponent(reinterpret_cast<std::uint32_t>(property)) != 0;

    property = FindProperty(
        gameOptions, nullptr, const_cast<char*>(g_pszKey_SUBTITLES));
    if (!property)
        return 0;
    self->field_0005 =
        ParseIntegerComponent(reinterpret_cast<std::uint32_t>(property)) != 0;

    property = FindProperty(
        gameOptions, nullptr, const_cast<char*>(g_pszKey_INVERT_VIEW));
    if (!property)
        return 0;
    self->field_0006 =
        ParseIntegerComponent(reinterpret_cast<std::uint32_t>(property)) != 0;

    property = FindProperty(
        gameOptions, nullptr, const_cast<char*>(g_pszKey_SENSIBILIDAD));
    if (!property)
        return 0;
    CoreZlibLog_ReadVector2(
        property, nullptr,
        reinterpret_cast<float*>(reinterpret_cast<std::uint8_t*>(self) + 8));

    property = FindProperty(
        gameOptions, nullptr, const_cast<char*>(g_pszKey_GAME_VIBRATION));
    if (!property)
        return 0;
    CSF_AT(std::uint8_t, self, 0x07) =
        ParseIntegerComponent(reinterpret_cast<std::uint32_t>(property)) != 0;

    return 1;
}

HOOK(0x0050A360, S_0050a360_ReadGameOptions, "auto");
