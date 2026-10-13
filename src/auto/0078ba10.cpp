#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(int, __fastcall, DataNode_ReadVector3Property, 0x00693A60, (void* self, void* edx, char* String1, int a3));

CSF_GLOBAL(const char*, g_pszKey_COLOR_INI, 0x008A9DEC);
CSF_GLOBAL(const char*, g_pszKey_COLOR_END, 0x008A9DF0);
CSF_GLOBAL(const char*, g_pszKey_DURATION, 0x008A9E40);
CSF_GLOBAL(const char*, g_pszKey_RADIUS_net_perf_counters, 0x008A9E44);
CSF_GLOBAL(const char*, g_pszKey_FADEIN_TIME, 0x008A9E48);
CSF_GLOBAL(const char*, g_pszKey_FADEOUT_TIME, 0x008A9E4C);
CSF_GLOBAL(const char*, g_pszKey_USE_FRACTAL, 0x008A9E50);
CSF_GLOBAL(const char*, g_pszKey_FRACTAL_TIMING, 0x008A9E54);
CSF_GLOBAL(const char*, g_pszKey_FRACTAL_TIMING_BIAS, 0x008A9E58);
CSF_GLOBAL(const char*, g_pszKey_FRACTAL_SMOOTHNESS, 0x008A9E5C);

static std::uint32_t NetPerfCounters_PackRgb(const void* color)
{
    const auto red = static_cast<std::uint8_t>(
        static_cast<std::int64_t>(CSF_AT(float, color, 0)));
    const auto green = static_cast<std::uint8_t>(
        static_cast<std::int64_t>(CSF_AT(float, color, 4)));
    const auto blue = static_cast<std::uint8_t>(
        static_cast<std::int64_t>(CSF_AT(float, color, 8)));

    return static_cast<std::uint32_t>(red)
        | (static_cast<std::uint32_t>(green) << 8)
        | (static_cast<std::uint32_t>(blue) << 16);
}

static void __stdcall NetPerfCounters_ReadEffectParameters(
    S_00692b80* node, int destination)
{
    alignas(8) std::uint8_t colorBytes[0xC];
    CSF_AT(float, colorBytes, 0) = 0.0f;
    CSF_AT(float, colorBytes, 4) = 0.0f;
    CSF_AT(float, colorBytes, 8) = 0.0f;

    auto* parameters = reinterpret_cast<std::uint8_t*>(destination);
    auto* attributeNode = reinterpret_cast<std::uint8_t*>(node);

    DataNode_ReadVector3Property(
        node, nullptr, const_cast<char*>(g_pszKey_COLOR_INI),
        reinterpret_cast<int>(colorBytes));
    CSF_AT(std::uint32_t, parameters, 0x50) =
        NetPerfCounters_PackRgb(colorBytes);

    DataNode_ReadVector3Property(
        node, nullptr, const_cast<char*>(g_pszKey_COLOR_END),
        reinterpret_cast<int>(colorBytes));
    CSF_AT(std::uint32_t, parameters, 0x65) =
        NetPerfCounters_PackRgb(colorBytes);

    CSF_AT(float, parameters, 0x54) = static_cast<float>(
        ReadFloatAttribute(attributeNode, nullptr,
            const_cast<char*>(g_pszKey_RADIUS_net_perf_counters)));
    CSF_AT(float, parameters, 0x58) = static_cast<float>(
        ReadFloatAttribute(attributeNode, nullptr,
            const_cast<char*>(g_pszKey_DURATION)));
    CSF_AT(float, parameters, 0x5C) = static_cast<float>(
        ReadFloatAttribute(attributeNode, nullptr,
            const_cast<char*>(g_pszKey_FADEIN_TIME)));
    CSF_AT(float, parameters, 0x60) = static_cast<float>(
        ReadFloatAttribute(attributeNode, nullptr,
            const_cast<char*>(g_pszKey_FADEOUT_TIME)));

    CSF_AT(std::uint8_t, parameters, 0x64) =
        ReadIntegerAttribute(attributeNode, nullptr,
            const_cast<char*>(g_pszKey_USE_FRACTAL)) != 0;

    CSF_AT(float, parameters, 0x6C) = static_cast<float>(
        ReadFloatAttribute(attributeNode, nullptr,
            const_cast<char*>(g_pszKey_FRACTAL_TIMING)));
    CSF_AT(float, parameters, 0x70) = static_cast<float>(
        ReadFloatAttribute(attributeNode, nullptr,
            const_cast<char*>(g_pszKey_FRACTAL_TIMING_BIAS)));
    CSF_AT(float, parameters, 0x74) = static_cast<float>(
        ReadFloatAttribute(attributeNode, nullptr,
            const_cast<char*>(g_pszKey_FRACTAL_SMOOTHNESS)));
}

HOOK(0x0078BA10, NetPerfCounters_ReadEffectParameters, "auto");
