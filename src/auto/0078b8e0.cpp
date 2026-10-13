#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0, (std::uint32_t component));
CSF_FUNC(int, __fastcall, DataNode_ReadVector3, 0x00693020, (void* self, void* edx, float* a2));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));

CSF_GLOBAL(const char*, g_pszKey_COLOR_INI, 0x008A9DEC);
CSF_GLOBAL(const char*, g_pszKey_WIDTH_net_perf_counters, 0x008A9E10);
CSF_GLOBAL(const char*, g_pszKey_HEIGHT_net_perf_counters, 0x008A9E14);
CSF_GLOBAL(const char*, g_pszKey_FALLOUT_INI, 0x008A9E2C);
CSF_GLOBAL(const char*, g_pszKey_FALLOUT_END, 0x008A9E30);
CSF_GLOBAL(const char*, g_pszKey_FADE_DIST, 0x008A9E34);
CSF_GLOBAL(const char*, g_pszKey_FADE_DIST_MIN, 0x008A9E38);
CSF_GLOBAL(const char*, g_pszKey_FADE_DIST_ALPHA, 0x008A9E3C);
CSF_GLOBAL(const char*, g_pszKey_RADIUS_net_perf_counters, 0x008A9E44);
CSF_GLOBAL(const char*, g_pszKey_FADEIN_TIME, 0x008A9E48);

static void __stdcall NetPerfCounters_ReadVisualSettings(
    S_00692b80* source, void* settings)
{
    auto* node = reinterpret_cast<std::uint8_t*>(source);

    CSF_AT(float, settings, 0x50) = static_cast<float>(
        ReadFloatAttribute(node, nullptr, const_cast<char*>(g_pszKey_WIDTH_net_perf_counters)));
    CSF_AT(float, settings, 0x54) = static_cast<float>(
        ReadFloatAttribute(node, nullptr, const_cast<char*>(g_pszKey_HEIGHT_net_perf_counters)));

    alignas(8) std::uint8_t colorBytes[0xC] = {};
    auto* property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_COLOR_INI));
    if (property)
        DataNode_ReadVector3(property, nullptr, reinterpret_cast<float*>(colorBytes));

    const auto red = static_cast<std::uint8_t>(
        static_cast<std::int64_t>(CSF_AT(float, colorBytes, 0x0)));
    const auto green = static_cast<std::uint8_t>(
        static_cast<std::int64_t>(CSF_AT(float, colorBytes, 0x4)));
    const auto blue = static_cast<std::uint8_t>(
        static_cast<std::int64_t>(CSF_AT(float, colorBytes, 0x8)));
    CSF_AT(std::uint32_t, settings, 0x5C) =
        static_cast<std::uint32_t>(red) |
        (static_cast<std::uint32_t>(green) << 8) |
        (static_cast<std::uint32_t>(blue) << 16) |
        (static_cast<std::uint32_t>(0xFF) << 24);

    CSF_AT(float, settings, 0x58) = static_cast<float>(
        ReadFloatAttribute(node, nullptr, const_cast<char*>(g_pszKey_RADIUS_net_perf_counters)));
    CSF_AT(float, settings, 0x60) = static_cast<float>(
        ReadFloatProperty(node, nullptr, const_cast<char*>(g_pszKey_FALLOUT_INI)));
    CSF_AT(float, settings, 0x64) = static_cast<float>(
        ReadFloatProperty(node, nullptr, const_cast<char*>(g_pszKey_FALLOUT_END)));

    property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_FADE_DIST));
    if (property)
        CSF_AT(float, settings, 0x6C) = static_cast<float>(
            ReadComponentFloat(reinterpret_cast<std::uint32_t>(property)));

    property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_FADE_DIST_MIN));
    if (property)
        CSF_AT(float, settings, 0x70) = static_cast<float>(
            ReadComponentFloat(reinterpret_cast<std::uint32_t>(property)));

    property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_FADE_DIST_ALPHA));
    if (property)
        CSF_AT(float, settings, 0x74) = static_cast<float>(
            ReadComponentFloat(reinterpret_cast<std::uint32_t>(property)));

    CSF_AT(float, settings, 0x68) = static_cast<float>(
        ReadFloatAttribute(node, nullptr, const_cast<char*>(g_pszKey_FADEIN_TIME)));
}

HOOK(0x0078B8E0, NetPerfCounters_ReadVisualSettings, "auto");
