#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(int, __fastcall, DataNode_ReadVector3Property, 0x00693A60, (void* self, void* edx, char* String1, int a3));

CSF_GLOBAL(const char*, g_pszKey_ALPHA_INI, 0x008A9DE4);
CSF_GLOBAL(const char*, g_pszKey_ALPHA_END, 0x008A9DE8);
CSF_GLOBAL(const char*, g_pszKey_COLOR_INI, 0x008A9DEC);
CSF_GLOBAL(const char*, g_pszKey_COLOR_END, 0x008A9DF0);
CSF_GLOBAL(const char*, g_pszKey_LONGITUD, 0x008A9DFC);
CSF_GLOBAL(const char*, g_pszKey_VELOCIDAD_net_perf_counters, 0x008A9E00);
CSF_GLOBAL(const char*, g_pszKey_FRECUENCIA, 0x008A9E04);
CSF_GLOBAL(const char*, g_pszKey_SIZE_net_perf_counters, 0x008A9E08);

static void __stdcall NetPerfCounters_ReadEffectProperties(
    S_00692b80* node, void* settings)
{
    auto* dataNode = reinterpret_cast<std::uint8_t*>(node);
    int alpha = ReadIntegerAttribute(
        dataNode, nullptr, const_cast<char*>(g_pszKey_ALPHA_INI));

    alignas(8) std::uint8_t colorBytes[0xC];
    CSF_AT(float, colorBytes, 0x0) = 0.0f;
    CSF_AT(float, colorBytes, 0x4) = 0.0f;
    CSF_AT(float, colorBytes, 0x8) = 0.0f;

    DataNode_ReadVector3Property(
        node, nullptr, const_cast<char*>(g_pszKey_COLOR_INI),
        reinterpret_cast<int>(colorBytes));

    std::uint8_t red = static_cast<std::uint8_t>(
        static_cast<std::int64_t>(CSF_AT(float, colorBytes, 0x0)));
    std::uint8_t green = static_cast<std::uint8_t>(
        static_cast<std::int64_t>(CSF_AT(float, colorBytes, 0x4)));
    std::uint8_t blue = static_cast<std::uint8_t>(
        static_cast<std::int64_t>(CSF_AT(float, colorBytes, 0x8)));

    CSF_AT(std::uint32_t, settings, 0x50) =
        static_cast<std::uint32_t>(red) |
        (static_cast<std::uint32_t>(green) << 8) |
        (static_cast<std::uint32_t>(blue) << 16) |
        (static_cast<std::uint32_t>(static_cast<std::uint8_t>(alpha)) << 24);

    alpha = ReadIntegerAttribute(
        dataNode, nullptr, const_cast<char*>(g_pszKey_ALPHA_END));
    DataNode_ReadVector3Property(
        node, nullptr, const_cast<char*>(g_pszKey_COLOR_END),
        reinterpret_cast<int>(colorBytes));

    red = static_cast<std::uint8_t>(
        static_cast<std::int64_t>(CSF_AT(float, colorBytes, 0x0)));
    green = static_cast<std::uint8_t>(
        static_cast<std::int64_t>(CSF_AT(float, colorBytes, 0x4)));
    blue = static_cast<std::uint8_t>(
        static_cast<std::int64_t>(CSF_AT(float, colorBytes, 0x8)));

    CSF_AT(std::uint32_t, settings, 0x54) =
        static_cast<std::uint32_t>(red) |
        (static_cast<std::uint32_t>(green) << 8) |
        (static_cast<std::uint32_t>(blue) << 16) |
        (static_cast<std::uint32_t>(static_cast<std::uint8_t>(alpha)) << 24);

    CSF_AT(float, settings, 0x58) = static_cast<float>(
        ReadFloatAttribute(dataNode, nullptr, const_cast<char*>(g_pszKey_LONGITUD)));
    CSF_AT(float, settings, 0x60) = static_cast<float>(
        ReadFloatAttribute(dataNode, nullptr,
                           const_cast<char*>(g_pszKey_VELOCIDAD_net_perf_counters)));
    CSF_AT(float, settings, 0x64) = static_cast<float>(
        ReadFloatAttribute(dataNode, nullptr, const_cast<char*>(g_pszKey_FRECUENCIA)));
    CSF_AT(float, settings, 0x5C) = static_cast<float>(
        ReadFloatAttribute(dataNode, nullptr,
                           const_cast<char*>(g_pszKey_SIZE_net_perf_counters)));
}

HOOK(0x0078B650, NetPerfCounters_ReadEffectProperties, "auto");
