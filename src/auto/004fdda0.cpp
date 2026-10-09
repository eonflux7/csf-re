#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0, (char* Str, int a2, char* a3));
CSF_FUNC(unsigned int, __cdecl, SceneLoader_CopyMapFilename, 0x006948E0, (char* a1, char* a2, unsigned int a3));
CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, SceneLoader_SetFileOption, 0x00698600, (char a1));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

static int __cdecl SceneLoader_LoadMapProperties(char* mapFilename, void* properties)
{
    const char savedOption = GetOptionsByteAt2();
    SceneLoader_SetFileOption(0);

    char filename[0x100] = {};
    char path[0x200] = {};

    SceneLoader_CopyMapFilename(mapFilename, filename, 256);
    Path_ReplaceExtension(filename, 256,
                          const_cast<char*>(CSF_STR(0x0083F50C, ".prp")));
    _sprintf(path, CSF_STR(0x0083F500, "Maps/Prp/%s"), filename);

    alignas(S_00692b10) std::uint8_t fileStorage[0x50];
    auto* file = reinterpret_cast<S_00692b10*>(fileStorage);

    // The packet identifies this constructor address as the file's vtable.
    (void)CSF_VTBL(0x00692B10);
    MenuExtras_InitializeFile(file, nullptr);

    int loaded = 0;
    if (MenuExtras_LoadFile(file, nullptr, path))
    {
        auto* root = reinterpret_cast<std::uint8_t*>(file->field_0000);
        auto* doors = FindProperty(
            root, nullptr, const_cast<char*>(CSF_STR(0x0083F4F4, ".NUMDOORS")));
        auto* commando = FindProperty(
            root, nullptr, const_cast<char*>(CSF_STR(0x0083F4E8, ".COMMANDO")));
        auto* sniper = FindProperty(
            root, nullptr, const_cast<char*>(CSF_STR(0x0083F4E0, ".SNIPER")));
        auto* spy = FindProperty(
            root, nullptr, const_cast<char*>(CSF_STR(0x0083F4D8, ".SPY")));
        auto* streaming = FindProperty(
            root, nullptr, const_cast<char*>(CSF_STR(0x0083F4CC, ".STREAMING")));
        auto* gasMask = FindProperty(
            root, nullptr, const_cast<char*>(CSF_STR(0x0083F4C0, ".GASMASK")));

        if (doors)
            CSF_AT(int, properties, 0x00) =
                ParseIntegerComponent(reinterpret_cast<std::uint32_t>(doors));
        if (commando)
            CSF_AT(std::uint8_t, properties, 0x04) =
                ParseIntegerComponent(reinterpret_cast<std::uint32_t>(commando)) != 0;
        if (sniper)
            CSF_AT(std::uint8_t, properties, 0x05) =
                ParseIntegerComponent(reinterpret_cast<std::uint32_t>(sniper)) != 0;
        if (spy)
            CSF_AT(std::uint8_t, properties, 0x06) =
                ParseIntegerComponent(reinterpret_cast<std::uint32_t>(spy)) != 0;
        if (streaming)
            CSF_AT(std::uint8_t, properties, 0x07) =
                ParseIntegerComponent(reinterpret_cast<std::uint32_t>(streaming)) != 0;
        if (gasMask)
            CSF_AT(std::uint8_t, properties, 0x08) =
                ParseIntegerComponent(reinterpret_cast<std::uint32_t>(gasMask)) != 0;

        loaded = 1;
    }

    SceneLoader_SetFileOption(savedOption);
    MenuExtras_CloseFile(file, nullptr);
    return loaded;
}

HOOK(0x004FDDA0, SceneLoader_LoadMapProperties, "auto");
