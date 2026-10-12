#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));

static const auto kPropertyVtableBinding = CSF_VTBL(0x00692B80);

static char __fastcall C_00844928_LoadTeamLoadout(
    C_00844928* self, void* /*edx*/, int team, S_00692b80* definition)
{
    auto* node = reinterpret_cast<std::uint8_t*>(definition);
    const auto teamIndex = static_cast<std::uint32_t>(team);

    auto* localClasses = FindProperty(
        node, nullptr, const_cast<char*>(CSF_STR(0x00844914, ".CLASSID_LOCALES")));
    if (!localClasses)
        return 0;

    for (unsigned int i = 0; i < 3; ++i)
    {
        if (i >= MenuExtras_GetChildCount(localClasses, nullptr))
            break;

        const auto component = GetComponent(localClasses, nullptr, i);
        if (component)
        {
            CSF_AT(int, self, 0x08 + 8 * teamIndex + 16 + 4 * teamIndex + 4 * i) =
                ParseIntegerComponent(component);
        }
    }

    auto* remoteClasses = FindProperty(
        node, nullptr, const_cast<char*>(CSF_STR(0x00844900, ".CLASSID_REMOTOS")));
    if (!remoteClasses)
        return 0;

    for (unsigned int i = 0; i < 3; ++i)
    {
        if (i >= MenuExtras_GetChildCount(remoteClasses, nullptr))
            break;

        const auto component = GetComponent(remoteClasses, nullptr, i);
        if (component)
        {
            CSF_AT(int, self, 0x08 + 8 * teamIndex + 40 + 4 * teamIndex + 4 * i) =
                ParseIntegerComponent(component);
        }
    }

    auto* primaryWeapons = FindProperty(
        node, nullptr, const_cast<char*>(CSF_STR(0x008448EC, ".ARMAS_PRIMARIAS")));
    if (!primaryWeapons)
        return 0;

    for (unsigned int slot = 0; slot < 3; ++slot)
    {
        if (slot >= MenuExtras_GetChildCount(primaryWeapons, nullptr))
            break;

        const auto groupComponent = GetComponent(primaryWeapons, nullptr, slot);
        if (groupComponent)
        {
            auto* group = reinterpret_cast<unsigned char*>(groupComponent);
            for (unsigned int weapon = 0; weapon < 0xA; ++weapon)
            {
                if (weapon >= MenuExtras_GetChildCount(group, nullptr))
                    break;

                const auto component = GetComponent(group, nullptr, weapon);
                if (component)
                {
                    CSF_AT(int, self,
                        0x08 + 80 * teamIndex + 64 + 40 * teamIndex +
                        40 * slot + 4 * weapon) = ParseIntegerComponent(component);
                }
            }
        }
    }

    auto* secondaryWeapons = FindProperty(
        node, nullptr, const_cast<char*>(CSF_STR(0x008448D8, ".ARMAS_SECUNDARIAS")));
    if (!secondaryWeapons)
        return 0;

    for (unsigned int slot = 0; slot < 3; ++slot)
    {
        if (slot >= MenuExtras_GetChildCount(secondaryWeapons, nullptr))
            break;

        const auto groupComponent = GetComponent(secondaryWeapons, nullptr, slot);
        if (groupComponent)
        {
            auto* group = reinterpret_cast<unsigned char*>(groupComponent);
            for (unsigned int weapon = 0; weapon < 3; ++weapon)
            {
                if (weapon >= MenuExtras_GetChildCount(group, nullptr))
                    break;

                const auto component = GetComponent(group, nullptr, weapon);
                if (component)
                {
                    CSF_AT(int, self,
                        0x08 + 16 * teamIndex + 304 + 8 * teamIndex +
                        8 * teamIndex + 8 * slot + 4 * teamIndex +
                        4 * slot + 4 * weapon) = ParseIntegerComponent(component);
                }
            }
        }
    }

    auto* postmanClasses = FindProperty(
        node, nullptr, const_cast<char*>(CSF_STR(0x008448C4, ".CLASSID_POSTMAN")));
    if (!postmanClasses)
        return 0;

    for (unsigned int i = 0; i < 3; ++i)
    {
        if (i >= MenuExtras_GetChildCount(postmanClasses, nullptr))
            break;

        const auto component = GetComponent(postmanClasses, nullptr, i);
        if (component)
        {
            CSF_AT(int, self, 0x08 + 8 * teamIndex + 376 + 4 * teamIndex + 4 * i) =
                ParseIntegerComponent(component);
        }
    }

    return 1;
}

HOOK(0x005583A0, C_00844928_LoadTeamLoadout, "auto");
