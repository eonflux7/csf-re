#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_00692b80_ParseIntegerComponent, 0x00692C40, (S_00692b80* self, void*));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(int, __fastcall, DataNode_ReadVector3Property, 0x00693A60, (void* self, void* edx, char* String1, int a3));

CSF_GLOBAL(const char*, g_pszKey_FOLLOW_ROT, 0x008A9DC8);
CSF_GLOBAL(const char*, g_pszKey_USE_STATIC_ANIM, 0x008A9DD4);
CSF_GLOBAL(const char*, g_pszKey_START_IN_LOOP, 0x008A9DD8);
CSF_GLOBAL(const char*, g_pszKey_USE_ALPHA_COLOR, 0x008A9DDC);
CSF_GLOBAL(const char*, g_pszKey_USE_GLOBAL_POS, 0x008A9DE0);
CSF_GLOBAL(const char*, g_pszKey_ALPHA_INI, 0x008A9DE4);
CSF_GLOBAL(const char*, g_pszKey_ALPHA_END, 0x008A9DE8);
CSF_GLOBAL(const char*, g_pszKey_COLOR_INI, 0x008A9DEC);
CSF_GLOBAL(const char*, g_pszKey_COLOR_END, 0x008A9DF0);

static std::uint8_t __stdcall NetPerfCounters_ReadStaticAnimationProperties(
    S_00692b80* node, void* settings)
{
    auto* dataNode = reinterpret_cast<std::uint8_t*>(node);

    CSF_AT(std::uint8_t, settings, 0x51) =
        ReadIntegerAttribute(dataNode, nullptr, const_cast<char*>(g_pszKey_FOLLOW_ROT)) != 0;

    std::uint8_t result =
        ReadIntegerAttribute(dataNode, nullptr, const_cast<char*>(g_pszKey_USE_STATIC_ANIM)) != 0;
    CSF_AT(std::uint8_t, settings, 0x50) = result;
    if (!result)
        return result;

    auto* loopProperty =
        FindProperty(dataNode, nullptr, const_cast<char*>(g_pszKey_START_IN_LOOP));
    if (loopProperty)
    {
        CSF_AT(std::uint8_t, settings, 0x52) =
            S_00692b80_ParseIntegerComponent(
                reinterpret_cast<S_00692b80*>(loopProperty), nullptr) != 0;
    }

    CSF_AT(std::uint8_t, settings, 0x65) =
        ReadIntegerAttribute(dataNode, nullptr, const_cast<char*>(g_pszKey_USE_GLOBAL_POS)) != 0;

    result =
        ReadIntegerAttribute(dataNode, nullptr, const_cast<char*>(g_pszKey_USE_ALPHA_COLOR)) != 0;
    CSF_AT(std::uint8_t, settings, 0x64) = result;
    if (!result)
        return result;

    CSF_AT(int, settings, 0x5C) =
        ReadIntegerAttribute(dataNode, nullptr, const_cast<char*>(g_pszKey_ALPHA_INI));
    CSF_AT(int, settings, 0x60) =
        ReadIntegerAttribute(dataNode, nullptr, const_cast<char*>(g_pszKey_ALPHA_END));

    alignas(8) std::uint8_t colorBytes[0xC];
    CSF_AT(float, colorBytes, 0x0) = 0.0f;
    CSF_AT(float, colorBytes, 0x4) = 0.0f;
    CSF_AT(float, colorBytes, 0x8) = 0.0f;

    DataNode_ReadVector3Property(
        node, nullptr, const_cast<char*>(g_pszKey_COLOR_INI),
        reinterpret_cast<int>(colorBytes));

    std::uint32_t initialColor =
        static_cast<std::uint8_t>(static_cast<std::int64_t>(CSF_AT(float, colorBytes, 0x0)));
    initialColor |= static_cast<std::uint32_t>(
        static_cast<std::uint8_t>(static_cast<std::int64_t>(CSF_AT(float, colorBytes, 0x4)))) << 8;
    initialColor |= static_cast<std::uint32_t>(
        static_cast<std::uint8_t>(static_cast<std::int64_t>(CSF_AT(float, colorBytes, 0x8)))) << 16;
    initialColor |= std::uint32_t{0xFF} << 24;
    CSF_AT(std::uint32_t, settings, 0x53) = initialColor;

    DataNode_ReadVector3Property(
        node, nullptr, const_cast<char*>(g_pszKey_COLOR_END),
        reinterpret_cast<int>(colorBytes));

    std::uint32_t finalColor =
        static_cast<std::uint8_t>(static_cast<std::int64_t>(CSF_AT(float, colorBytes, 0x0)));
    finalColor |= static_cast<std::uint32_t>(
        static_cast<std::uint8_t>(static_cast<std::int64_t>(CSF_AT(float, colorBytes, 0x4)))) << 8;
    finalColor |= static_cast<std::uint32_t>(
        static_cast<std::uint8_t>(static_cast<std::int64_t>(CSF_AT(float, colorBytes, 0x8)))) << 16;
    finalColor |= std::uint32_t{0xFF} << 24;
    CSF_AT(std::uint32_t, settings, 0x57) = finalColor;

    return static_cast<std::uint8_t>(finalColor);
}

HOOK(0x0078B500, NetPerfCounters_ReadStaticAnimationProperties, "auto");
