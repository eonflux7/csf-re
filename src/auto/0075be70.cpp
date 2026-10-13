#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, BddTables_ClearRecord68List, 0x0051BF70, (void* self, void*));
CSF_FUNC(uint32_t*, __fastcall, S_00692b10_InitializeFile, 0x00692B10, (S_00692b10* self, void*));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(char, __fastcall, C_0086c1e0_ParseVersion1, 0x0075B760, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, C_0086c1e0_ParseVersion2, 0x0075B920, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, C_0086c1e0_ParseVersion3, 0x0075BAE0, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, C_0086c1e0_ParseVersion4, 0x0075BCA0, (void* self, void* edx, void* a2));

CSF_GLOBAL(const char*, g_pszKey_VERSION_8A8CAC, 0x008A8CAC);

static char __fastcall C_0086c1e0_LoadSoundDefinitions(
    C_0086c1e0* self, void* /*edx*/, const char* path)
{
    BddTables_ClearRecord68List(
        reinterpret_cast<void*>(self->field_0004), nullptr);

    alignas(8) std::uint8_t fileBytes[0x3C];
    S_00692b10& file = *reinterpret_cast<S_00692b10*>(fileBytes);

    // The packet labels the initializer address as a vtable reference.
    // Bind it without introducing a vtable write absent from the original.
    (void)CSF_VTBL(0x00692B10);
    S_00692b10_InitializeFile(&file, nullptr);

    if (!MenuExtras_LoadFile(&file, nullptr, path))
    {
        MenuExtras_CloseFile(&file, nullptr);
        return 0;
    }

    auto* node = reinterpret_cast<std::uint8_t*>(file.field_0000);
    const int version = ReadIntegerAttribute(
        node, nullptr, const_cast<char*>(g_pszKey_VERSION_8A8CAC));

    char result = 0;
    if (version <= 4)
    {
        switch (version)
        {
        case 1:
            result = C_0086c1e0_ParseVersion1(self, nullptr, node);
            break;
        case 2:
            result = C_0086c1e0_ParseVersion2(self, nullptr, node);
            break;
        case 3:
            result = C_0086c1e0_ParseVersion3(self, nullptr, node);
            break;
        case 4:
            result = C_0086c1e0_ParseVersion4(self, nullptr, node);
            break;
        default:
            break;
        }
    }

    MenuExtras_CloseFile(&file, nullptr);
    return result;
}

HOOK(0x0075BE70, C_0086c1e0_LoadSoundDefinitions, "auto");
