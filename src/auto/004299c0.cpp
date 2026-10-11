#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, SetOptionsByteAt40, 0x00697330, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte40, 0x00697350, ());
CSF_FUNC(char, __cdecl, SceneLoader_SetFileOption, 0x00698600, (char a1));
CSF_FUNC(char, __fastcall, GrowControlBindingArray, 0x0075C1C0, (void* self, void* edx, int a2));

CSF_GLOBAL(const char*, g_pszConfig_Control, 0x00899520);
CSF_GLOBAL(const char*, g_pszKey_DEVICE, 0x00899528);
CSF_GLOBAL(const char*, g_pszKey_DEVICE_ID, 0x0089952C);
CSF_GLOBAL(const char*, g_pszKey_SYMBOL, 0x00899534);
CSF_GLOBAL(const char*, g_pszKey_LISTADATOS_PC, 0x00899538);

static int __stdcall Cheats_LoadControlBindings(std::uint32_t* bindings)
{
    alignas(8) std::uint8_t fileBytes[0x54];

    // The packet labels these call targets as vtable references.
    (void)CSF_VTBL(0x00692B10);
    (void)CSF_VTBL(0x00692B80);

    MenuExtras_InitializeFile(fileBytes, nullptr);

    const char fileOption = GetOptionsByteAt2();
    CSF_AT(char, fileBytes, 0x4C) = fileOption;
    if (fileOption)
        SceneLoader_SetFileOption(0);

    const char option40 = GetOptionsByte40();
    CSF_AT(char, fileBytes, 0x50) = option40;
    if (!option40)
        SetOptionsByteAt40(1);

    const char loaded = MenuExtras_LoadFile(fileBytes, nullptr, g_pszConfig_Control);
    SetOptionsByteAt40(CSF_AT(char, fileBytes, 0x50));
    SceneLoader_SetFileOption(CSF_AT(char, fileBytes, 0x4C));

    if (loaded)
    {
        auto* property = FindProperty(
            CSF_AT(std::uint8_t*, fileBytes, 0x00),
            nullptr, const_cast<char*>(g_pszKey_LISTADATOS_PC));
        CSF_AT(std::uint8_t*, fileBytes, 0x4C) = property;

        if (property)
        {
            const std::uint32_t capacity = CSF_AT(std::uint32_t, bindings, 0x04);
            CSF_AT(std::uint32_t, bindings, 0x08) = 0;
            if (capacity < 0x20)
                GrowControlBindingArray(bindings, nullptr, 32 - capacity);

            CSF_AT(int, fileBytes, 0x3C) = -1;
            CSF_AT(int, fileBytes, 0x40) = -1;
            CSF_AT(int, fileBytes, 0x44) = -1;

            const std::uint32_t childCount =
                MenuExtras_GetChildCount(property, nullptr);
            CSF_AT(std::uint32_t, fileBytes, 0x48) = childCount;

            std::uint32_t index = 0;
            if (childCount)
            {
                do
                {
                    auto* component = reinterpret_cast<std::uint8_t*>(
                        GetComponent(property, nullptr, index));
                    auto* device = FindProperty(
                        component, nullptr, const_cast<char*>(g_pszKey_DEVICE));
                    CSF_AT(std::uint8_t*, fileBytes, 0x50) = FindProperty(
                        component, nullptr, const_cast<char*>(g_pszKey_DEVICE_ID));
                    auto* symbol = FindProperty(
                        component, nullptr, const_cast<char*>(g_pszKey_SYMBOL));

                    if (device && CSF_AT(std::uint8_t*, fileBytes, 0x50) && symbol)
                    {
                        const int deviceValue = ParseIntegerComponent(
                            reinterpret_cast<std::uint32_t>(device));
                        CSF_AT(int, fileBytes, 0x40) = ParseIntegerComponent(
                            reinterpret_cast<std::uint32_t>(
                                CSF_AT(std::uint8_t*, fileBytes, 0x50)));
                        const int symbolValue = ParseIntegerComponent(
                            reinterpret_cast<std::uint32_t>(symbol));

                        if (CSF_AT(std::uint32_t, bindings, 0x08) >=
                            CSF_AT(std::uint32_t, bindings, 0x04))
                        {
                            GrowControlBindingArray(
                                bindings, nullptr, CSF_AT(int, bindings, 0x0C));
                        }

                        const std::uint32_t count =
                            CSF_AT(std::uint32_t, bindings, 0x08);
                        auto* entry = reinterpret_cast<std::uint8_t*>(
                            CSF_AT(std::uint32_t, bindings, 0x00) + 12 * count);
                        const int deviceId = CSF_AT(int, fileBytes, 0x40);
                        CSF_AT(std::uint32_t, bindings, 0x08) = count + 1;
                        CSF_AT(int, entry, 0x00) = deviceValue;
                        CSF_AT(int, entry, 0x04) = deviceId;
                        CSF_AT(int, entry, 0x08) = symbolValue;
                    }

                    ++index;
                    if (index >= CSF_AT(std::uint32_t, fileBytes, 0x48))
                        break;
                    property = CSF_AT(std::uint8_t*, fileBytes, 0x4C);
                } while (true);
            }
        }
    }

    return MenuExtras_CloseFile(fileBytes, nullptr);
}

HOOK(0x004299C0, Cheats_LoadControlBindings, "auto");
