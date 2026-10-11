#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0, (char* Str, int a2, char* a3));
CSF_FUNC(bool, __cdecl, MenuManager_CheckPageAvailability, 0x00698B20, (const char* name, char mode));
CSF_FUNC(char*, __cdecl, _strncpy, 0x007AFEC0, (char* dest, const char* src, size_t count));

[[maybe_unused]] static const auto kFileInitializerVtable = CSF_VTBL(0x00692B10);

static int __stdcall SceneLoader_CheckSceneCompanionFiles(S_00693740* source)
{
    char destination[512];
    // sized by the original's frame (0x23C bytes): struct sizes in auto_classes.h are lower bounds
    alignas(8) uint8_t fileBytes[0x23C];
    S_00692b10& file = *reinterpret_cast<S_00692b10*>(fileBytes);

    MenuExtras_InitializeFile(&file, nullptr);
    if (MenuExtras_LoadFile(&file, nullptr, reinterpret_cast<const char*>(source)))
    {
        auto* properties = reinterpret_cast<std::uint8_t*>(file.field_0000);
        auto* state = reinterpret_cast<std::uint8_t*>(GetValue8B0554()) + 0x54;

        _strncpy(destination, reinterpret_cast<const char*>(source), 0x200);
        destination[511] = 0;

        Path_ReplaceExtension(
            destination, 512, const_cast<char*>(CSF_STR(0x0083F514, ".csc")));
        MenuManager_CheckPageAvailability(destination, 0);

        Path_ReplaceExtension(
            destination, 512, const_cast<char*>(CSF_STR(0x0083F51C, ".gsc")));
        MenuManager_CheckPageAvailability(destination, 0);

        Path_ReplaceExtension(
            destination, 512, const_cast<char*>(CSF_STR(0x0083F534, ".vis")));
        MenuManager_CheckPageAvailability(destination, 0);

        if (!CSF_AT(std::uint8_t, state, 0x1ED8) &&
            !CSF_AT(std::uint8_t, state, 0x1ED9) &&
            !CSF_AT(std::uint8_t, state, 0x1EDA) &&
            !CSF_AT(std::uint8_t, state, 0x1EE0) &&
            FindProperty(properties, nullptr,
                         const_cast<char*>(CSF_STR(0x0083F524, ".MAPA_SECTORES"))))
        {
            MenuExtras_ReadText(
                properties, nullptr,
                const_cast<char*>(CSF_STR(0x0083F524, ".MAPA_SECTORES")),
                destination, 512);
            if (std::strlen(destination))
                MenuManager_CheckPageAvailability(destination, 0);
        }
    }

    return MenuExtras_CloseFile(&file, nullptr);
}

HOOK(0x004FE2D0, SceneLoader_CheckSceneCompanionFiles, "auto");
