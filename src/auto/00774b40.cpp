#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ClearBddTableEntryList, 0x0051AC90, (void* self, void*));
CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));

CSF_FUNC(char, __fastcall, C_00871a18_LoadVersions1And2, 0x007741F0, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, C_00871a18_LoadVersion3, 0x00774420, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, C_00871a18_LoadVersion4, 0x00774680, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, C_00871a18_LoadVersion5, 0x00774950, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, C_00871a18_LoadVersion6, 0x00774A20, (void* self, void* edx, void* a2));

CSF_GLOBAL(const char*, g_pszKey_VERSION_8A9980, 0x008A9980);

static char __fastcall C_00871a18_LoadAnimationTable(
    C_00871a18* self, void* /*edx*/, const char* path)
{
    ClearBddTableEntryList(reinterpret_cast<void*>(self->field_0004), nullptr);

    alignas(8) std::uint8_t fileBytes[0x3C];
    S_00692b10& file = *reinterpret_cast<S_00692b10*>(fileBytes);

    // The packet associates this reference with the initializer call.
    (void)CSF_VTBL(0x00692B10);
    MenuExtras_InitializeFile(&file, nullptr);

    if (!MenuExtras_LoadFile(&file, nullptr, path))
    {
        MenuExtras_CloseFile(&file, nullptr);
        return 0;
    }

    auto* root = reinterpret_cast<std::uint8_t*>(file.field_0000);
    const int version = ReadIntegerAttribute(
        root, nullptr, const_cast<char*>(g_pszKey_VERSION_8A9980));

    if (version > 6)
    {
        MenuExtras_CloseFile(&file, nullptr);
        return 0;
    }

    char result = 0;
    switch (version)
    {
    case 1:
    case 2:
        result = C_00871a18_LoadVersions1And2(self, nullptr, root);
        break;
    case 3:
        result = C_00871a18_LoadVersion3(self, nullptr, root);
        break;
    case 4:
        result = C_00871a18_LoadVersion4(self, nullptr, root);
        break;
    case 5:
        result = C_00871a18_LoadVersion5(self, nullptr, root);
        break;
    case 6:
        result = C_00871a18_LoadVersion6(self, nullptr, root);
        break;
    default:
        break;
    }

    MenuExtras_CloseFile(&file, nullptr);
    return result;
}

HOOK(0x00774B40, C_00871a18_LoadAnimationTable, "auto");
