#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));

CSF_GLOBAL(const char*, g_pszKey_CONTROL, 0x00899524);
CSF_GLOBAL(const char*, g_pszKey_DEVICE, 0x00899528);
CSF_GLOBAL(const char*, g_pszKey_DEVICE_ID, 0x0089952C);

static int __cdecl Cheats_LoadControlBindings(
    S_00692b80* node, int bindingsAddress, int /*unused*/)
{
    (void)CSF_VTBL(0x00692B80);

    if (!bindingsAddress || !node)
        return bindingsAddress;

    const std::uint32_t childCount = MenuExtras_GetChildCount(node, nullptr);
    if (!childCount)
        return 0;

    std::uint32_t index = 0;
    do
    {
        auto* component = reinterpret_cast<std::uint8_t*>(
            GetComponent(reinterpret_cast<unsigned char*>(node), nullptr, index));

        auto* controlProperty = FindProperty(
            component, nullptr, const_cast<char*>(g_pszKey_CONTROL));
        auto* deviceProperty = FindProperty(
            component, nullptr, const_cast<char*>(g_pszKey_DEVICE));
        auto* deviceIdProperty = FindProperty(
            component, nullptr, const_cast<char*>(g_pszKey_DEVICE_ID));

        if (controlProperty && deviceProperty && deviceIdProperty)
        {
            const int control = ParseIntegerComponent(
                reinterpret_cast<std::uint32_t>(controlProperty));
            const int device = ParseIntegerComponent(
                reinterpret_cast<std::uint32_t>(deviceProperty));
            const int deviceId = ParseIntegerComponent(
                reinterpret_cast<std::uint32_t>(deviceIdProperty));

            const std::uint32_t entryAddress =
                static_cast<std::uint32_t>(bindingsAddress) +
                20 * static_cast<std::uint32_t>(control);
            auto* entry = reinterpret_cast<std::uint8_t*>(entryAddress);

            CSF_AT(int, entry, 0x00) = -1;
            CSF_AT(int, entry, 0x04) = -1;
            CSF_AT(int, entry, 0x08) = 0;
            CSF_AT(float, entry, 0x0C) = 1.0f;
            CSF_AT(int, entry, 0x10) = 0;
            CSF_AT(int, entry, 0x00) = device;
            CSF_AT(int, entry, 0x04) = deviceId;
        }

        ++index;
    } while (index < childCount);

    return static_cast<int>(index);
}

HOOK(0x00428980, Cheats_LoadControlBindings, "auto");
