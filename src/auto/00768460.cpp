#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharacterDefs_ResetOutput, 0x00510FF0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));

CSF_FUNC(char, __fastcall, C_0086f128_ReadVersions1To4, 0x00763390, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, C_0086f128_ReadVersion5, 0x00763970, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, C_0086f128_ReadVersions6And7, 0x00763FA0, (void* self, void* edx, int* a2, void* a3, char* String1));
CSF_FUNC(char, __fastcall, C_0086f128_ReadVersion8, 0x00764E90, (void* self, void* edx, int* a2, void* a3, char* String1));
CSF_FUNC(char, __fastcall, C_0086f128_ReadVersion9, 0x00765DD0, (void* self, void* edx, int* a2, void* a3, char* String1));
CSF_FUNC(char, __fastcall, C_0086f128_ReadVersion10, 0x00766F10, (void* self, void* edx, int* a2, void* a3, char* String1));

CSF_GLOBAL(const char*, g_pszKey_VERSION_8A9800, 0x008A9800);

static char __fastcall C_0086f128_LoadCharacterDefinitions(
    C_0086f128* self, void* /*edx*/, int* output, int filePath, char* name)
{
    CharacterDefs_ResetOutput(output, nullptr);

    alignas(4) std::uint8_t file[0x3C];
    // The packet labels the initializer address as a vtable reference.
    (void)CSF_VTBL(0x00692B10);
    MenuExtras_InitializeFile(file, nullptr);

    char result = 0;
    if (MenuExtras_LoadFile(file, nullptr, reinterpret_cast<const char*>(filePath)))
    {
        std::uint8_t* root = CSF_AT(std::uint8_t*, file, 0x00);
        int version = ReadIntegerAttribute(
            root, nullptr, const_cast<char*>(g_pszKey_VERSION_8A9800));

        if (version <= 10)
        {
            switch (version)
            {
            case 1:
            case 2:
            case 3:
            case 4:
                result = C_0086f128_ReadVersions1To4(self, nullptr, root);
                break;
            case 5:
                result = C_0086f128_ReadVersion5(self, nullptr, root);
                break;
            case 6:
            case 7:
                result = C_0086f128_ReadVersions6And7(self, nullptr, output, root, name);
                break;
            case 8:
                result = C_0086f128_ReadVersion8(self, nullptr, output, root, name);
                break;
            case 9:
                result = C_0086f128_ReadVersion9(self, nullptr, output, root, name);
                break;
            case 10:
                result = C_0086f128_ReadVersion10(self, nullptr, output, root, name);
                break;
            default:
                break;
            }
        }
    }

    MenuExtras_CloseFile(file, nullptr);
    return result;
}

HOOK(0x00768460, C_0086f128_LoadCharacterDefinitions, "auto");
