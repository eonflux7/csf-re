#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, Cheats_FinalizeControlData, 0x004288B0, (void* self, void* edx, uint32_t* a2, int a3));
CSF_FUNC(int, __cdecl, Cheats_ReadControlData, 0x00428980, (void* a1, int a2, int a3));
CSF_FUNC(int, __fastcall, Cheats_ResetControlData, 0x00428AC0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, SetOptionsByteAt40, 0x00697330, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte40, 0x00697350, ());
CSF_FUNC(char, __cdecl, SceneLoader_SetFileOption, 0x00698600, (char a1));

CSF_GLOBAL(const char*, g_pszConfig_Control, 0x00899520);
CSF_GLOBAL(const char*, g_pszKey_LISTADATOS_PC, 0x00899538);

static int __fastcall C_008300c0_LoadControlData(
    C_008300c0* self, void* /*edx*/)
{
    alignas(8) std::uint8_t fileBytes[0x44];

    Cheats_ResetControlData(self, nullptr);
    MenuExtras_InitializeFile(fileBytes, nullptr);
    // The packet identifies this binding as the file object's vtable reference.
    (void)CSF_VTBL(0x00692B10);

    CSF_AT(char, fileBytes, 0x3C) = GetOptionsByteAt2();
    if (CSF_AT(char, fileBytes, 0x3C))
        SceneLoader_SetFileOption(0);

    CSF_AT(char, fileBytes, 0x40) = GetOptionsByte40();
    if (!CSF_AT(char, fileBytes, 0x40))
        SetOptionsByteAt40(1);

    const char loaded =
        MenuExtras_LoadFile(fileBytes, nullptr, g_pszConfig_Control);

    SetOptionsByteAt40(CSF_AT(char, fileBytes, 0x40));
    SceneLoader_SetFileOption(CSF_AT(char, fileBytes, 0x3C));

    if (loaded)
    {
        std::uint8_t* property = FindProperty(
            CSF_AT(std::uint8_t*, fileBytes, 0),
            nullptr,
            const_cast<char*>(g_pszKey_LISTADATOS_PC));

        if (property)
        {
            auto* controlData = reinterpret_cast<uint32_t*>(
                reinterpret_cast<std::uint8_t*>(self) + 0x74C);

            ((void(__fastcall*)(void*, void*, uint32_t*))
                CSF_AT(void**, self, 0)[0x40 / 4])(
                    self, nullptr, controlData);

            Cheats_ReadControlData(
                property, reinterpret_cast<int>(controlData), 32);
            Cheats_FinalizeControlData(self, nullptr, controlData, 93);
            CSF_AT(std::uint32_t, self, 0x748) = 0;
        }
    }

    return MenuExtras_CloseFile(fileBytes, nullptr);
}

HOOK(0x00429420, C_008300c0_LoadControlData, "auto");
