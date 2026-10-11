#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40, (std::uint8_t* attribute, void* edx, char* buffer, int capacity));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, SetOptionsByteAt40, 0x00697330, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte40, 0x00697350, ());
CSF_FUNC(char, __cdecl, SceneLoader_SetFileOption, 0x00698600, (char a1));

CSF_GLOBAL(const char*, g_pszConfig_Control, 0x00899520);
CSF_GLOBAL(const char*, g_pszKey_CONTROL, 0x00899524);
CSF_GLOBAL(const char*, g_pszKey_ID_ESTATICO, 0x00899530);
CSF_GLOBAL(const char*, g_pszKey_LISTADATOS_PC, 0x00899538);

static char __stdcall Cheats_LoadControlLabels(void* labels, int /*unused*/)
{
    if (!labels)
        return 0;

    // These packet-listed references belong to the bound callees.
    (void)CSF_VTBL(0x00692B10);
    (void)CSF_VTBL(0x00692B80);

    alignas(8) std::uint8_t fileBytes[0x48];
    S_00692b10& file = *reinterpret_cast<S_00692b10*>(fileBytes);
    MenuExtras_InitializeFile(&file, nullptr);

    const char savedFileOption = GetOptionsByteAt2();
    if (savedFileOption)
        SceneLoader_SetFileOption(0);

    const char savedOption40 = GetOptionsByte40();
    if (!savedOption40)
        SetOptionsByteAt40(1);

    const char loaded = MenuExtras_LoadFile(&file, nullptr, g_pszConfig_Control);
    SetOptionsByteAt40(savedOption40);
    SceneLoader_SetFileOption(savedFileOption);

    if (loaded)
    {
        auto* list = FindProperty(
            reinterpret_cast<std::uint8_t*>(file.field_0000),
            nullptr, const_cast<char*>(g_pszKey_LISTADATOS_PC));

        if (list)
        {
            const std::uint32_t childCount = MenuExtras_GetChildCount(list, nullptr);
            for (std::uint32_t childIndex = 0; childIndex < childCount; ++childIndex)
            {
                auto* component = reinterpret_cast<std::uint8_t*>(
                    GetComponent(list, nullptr, childIndex));
                auto* control = FindProperty(
                    component, nullptr, const_cast<char*>(g_pszKey_CONTROL));
                auto* staticId = FindProperty(
                    component, nullptr, const_cast<char*>(g_pszKey_ID_ESTATICO));

                if (control && staticId)
                {
                    const int controlIndex = ParseIntegerComponent(
                        reinterpret_cast<std::uint32_t>(control));
                    char text[256];
                    std::memset(text, 0, sizeof(text));
                    CopyAttributeText(staticId, nullptr, text, 256);

                    const unsigned int length =
                        static_cast<unsigned int>(std::strlen(text));
                    const std::uintptr_t destination =
                        reinterpret_cast<std::uintptr_t>(labels) +
                        28 * static_cast<std::uint32_t>(controlIndex);
                    AssignStringRange_429B60(
                        reinterpret_cast<void*>(destination), nullptr, text, length);
                }
            }
        }
    }

    return static_cast<char>(MenuExtras_CloseFile(&file, nullptr));
}

HOOK(0x00429C90, Cheats_LoadControlLabels, "auto");
