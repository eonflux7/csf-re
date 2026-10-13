#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0, (std::uint32_t component));
CSF_FUNC(void, __fastcall, CoreZlibLog_ReadVector2, 0x00692F60, (void* self, void* edx, float* a2));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));

CSF_GLOBAL(const char*, g_pszKey_QUALITY, 0x00899A5C);
CSF_GLOBAL(const char*, g_pszKey_MODE, 0x00899A60);
CSF_GLOBAL(const char*, g_pszKey_BRIGHTNESS, 0x00899A64);
CSF_GLOBAL(const char*, g_pszKey_NOISE, 0x00899A68);
CSF_GLOBAL(const char*, g_pszKey_VERTICAL_SYNC, 0x00899A6C);
CSF_GLOBAL(const char*, g_pszKey_ANTIALIAS, 0x00899A70);
CSF_GLOBAL(const char*, g_pszKey_SHADOWS, 0x00899A74);
CSF_GLOBAL(const char*, g_pszKey_TEX_FILTER, 0x00899A78);
CSF_GLOBAL(const char*, g_pszKey_TEX_QUALITY, 0x00899A7C);
CSF_GLOBAL(const char*, g_pszKey_MODEL_QUALITY, 0x00899A80);
CSF_GLOBAL(const char*, g_pszKey_EFFECTS_QUALITY, 0x00899A84);
CSF_GLOBAL(const char*, g_pszKey_WATER_QUALITY, 0x00899A88);
CSF_GLOBAL(const char*, g_pszKey_VIDEO, 0x00899A90);

static char __fastcall S_0050ab80_ReadVideoOptions(
    S_0050ab80* self, void* /*edx*/, S_00692b80* options)
{
    std::uint8_t* video = FindProperty(
        reinterpret_cast<std::uint8_t*>(options), nullptr,
        const_cast<char*>(g_pszKey_VIDEO));
    if (!video)
        return 0;

    std::uint8_t* property = FindProperty(
        video, nullptr, const_cast<char*>(g_pszKey_QUALITY));
    if (!property)
        return 0;
    self->field_000c = ParseIntegerComponent(
        reinterpret_cast<std::uint32_t>(property));

    property = FindProperty(video, nullptr, const_cast<char*>(g_pszKey_MODE));
    if (!property)
        return 0;

    alignas(8) std::uint8_t modeBytes[0x8];
    CSF_AT(float, modeBytes, 0) = 0.0f;
    CSF_AT(float, modeBytes, 4) = 0.0f;
    CoreZlibLog_ReadVector2(property, nullptr, reinterpret_cast<float*>(modeBytes));
    self->field_0004 = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(CSF_AT(float, modeBytes, 0)));
    self->field_0008 = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(CSF_AT(float, modeBytes, 4)));

    property = FindProperty(video, nullptr, const_cast<char*>(g_pszKey_BRIGHTNESS));
    if (!property)
        return 0;
    self->field_0010 = static_cast<float>(
        ReadComponentFloat(reinterpret_cast<std::uint32_t>(property)));

    property = FindProperty(video, nullptr, const_cast<char*>(g_pszKey_VERTICAL_SYNC));
    if (!property)
        return 0;
    self->field_0015 =
        ParseIntegerComponent(reinterpret_cast<std::uint32_t>(property)) == 1;

    property = FindProperty(video, nullptr, const_cast<char*>(g_pszKey_ANTIALIAS));
    if (!property)
        return 0;
    self->field_0018 = ParseIntegerComponent(
        reinterpret_cast<std::uint32_t>(property));

    property = FindProperty(video, nullptr, const_cast<char*>(g_pszKey_SHADOWS));
    if (!property)
        return 0;
    self->field_001c = ParseIntegerComponent(
        reinterpret_cast<std::uint32_t>(property));

    property = FindProperty(video, nullptr, const_cast<char*>(g_pszKey_TEX_FILTER));
    if (!property)
        return 0;
    self->field_0020 = ParseIntegerComponent(
        reinterpret_cast<std::uint32_t>(property));

    property = FindProperty(video, nullptr, const_cast<char*>(g_pszKey_TEX_QUALITY));
    if (!property)
        return 0;
    self->field_0024 = ParseIntegerComponent(
        reinterpret_cast<std::uint32_t>(property));

    property = FindProperty(video, nullptr, const_cast<char*>(g_pszKey_MODEL_QUALITY));
    if (!property)
        return 0;
    self->field_0028 = ParseIntegerComponent(
        reinterpret_cast<std::uint32_t>(property));

    property = FindProperty(video, nullptr, const_cast<char*>(g_pszKey_EFFECTS_QUALITY));
    if (!property)
        return 0;
    self->field_002c = ParseIntegerComponent(
        reinterpret_cast<std::uint32_t>(property));

    property = FindProperty(video, nullptr, const_cast<char*>(g_pszKey_WATER_QUALITY));
    if (!property)
        return 0;
    self->field_0030 = ParseIntegerComponent(
        reinterpret_cast<std::uint32_t>(property));

    property = FindProperty(video, nullptr, const_cast<char*>(g_pszKey_NOISE));
    if (!property)
        return 0;
    CSF_AT(std::uint8_t, self, 0x14) =
        ParseIntegerComponent(reinterpret_cast<std::uint32_t>(property)) == 1;

    return 1;
}

HOOK(0x0050AB80, S_0050ab80_ReadVideoOptions, "auto");
