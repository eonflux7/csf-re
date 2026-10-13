#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, OptionsFile_ParseV8Block, 0x0050BCB0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));

CSF_GLOBAL(char*, g_pszKey_VERSION, 0x00899A9C);
CSF_GLOBAL(char*, g_pszKey_OPCIONES, 0x00899AA0);

// Reads the file's VERSION attribute; if it is 8 (and the OPCIONES node exists),
// parses the options block for version 8. Returns 0 otherwise.
static char __fastcall OptionsFile_TryLoadOptionsV8(C_0083fdf4* self, void* /*edx*/, int a2)
{
    // a2 points to a slot holding the root node pointer.
    std::uint8_t* rootNode = *reinterpret_cast<std::uint8_t**>(static_cast<std::uintptr_t>(a2));

    int fileVersion = ReadIntegerAttribute(rootNode, nullptr, g_pszKey_VERSION);
    if (fileVersion <= 8)
    {
        std::uint8_t* optionsNode = FindProperty(rootNode, nullptr, g_pszKey_OPCIONES);
        if (optionsNode != nullptr && fileVersion == 8)
        {
            return OptionsFile_ParseV8Block(self, nullptr, static_cast<int>(reinterpret_cast<std::uintptr_t>(optionsNode)), 8);
        }
    }
    return 0;
}

HOOK(0x0050BDA0, OptionsFile_TryLoadOptionsV8, "auto");
