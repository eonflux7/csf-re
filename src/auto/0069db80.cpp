#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_00692b10_GetFileData, 0x00692900, (S_00692b10* self, void* , uint32_t* outFirst, uint32_t* outSecond));
CSF_FUNC(uint32_t*, __fastcall, S_00692b10_InitializeFile, 0x00692B10, (S_00692b10* self, void*));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(int, __stdcall, MpRules_ComputeFileChecksum, 0x0069DB30, (int a1, unsigned int a2));
CSF_FUNC(int, __fastcall, S_00692b80_ApplyComponentCallback, 0x006A2910, (S_00692b80* self, void* , S_00692b80* a1, void* componentList));

CSF_GLOBAL(const char*, g_pszKey_PREFERENCES, 0x008A6B90);

static char __fastcall C_0086c2b4_SetupFieldBuffer(
    C_0086c2b4* self, void* /*edx*/,
    int filePath, int componentList, unsigned int checksumFlag)
{
    alignas(8) std::uint8_t fileBytes[0x3C];
    S_00692b10& file = *reinterpret_cast<S_00692b10*>(fileBytes);

    // Packet-listed reference associated with the initializer.
    (void)CSF_VTBL(0x00692B10);
    S_00692b10_InitializeFile(&file, nullptr);

    if (!MenuExtras_LoadFile(
            &file, nullptr, reinterpret_cast<const char*>(filePath)))
    {
        MenuExtras_CloseFile(&file, nullptr);
        return 0;
    }

    std::uint8_t* property = FindProperty(
        reinterpret_cast<std::uint8_t*>(file.field_0000),
        nullptr, const_cast<char*>(g_pszKey_PREFERENCES));

    if (!property)
    {
        MenuExtras_CloseFile(&file, nullptr);
        return 0;
    }

    if (static_cast<std::uint8_t>(checksumFlag))
    {
        checksumFlag = 0;
        uint32_t fileData = static_cast<uint32_t>(filePath);
        S_00692b10_GetFileData(&file, nullptr, &fileData, &checksumFlag);
        self->field_0004 = MpRules_ComputeFileChecksum(
            static_cast<int>(fileData), checksumFlag);
    }

    S_00692b80_ApplyComponentCallback(
        reinterpret_cast<S_00692b80*>(self), nullptr,
        reinterpret_cast<S_00692b80*>(property),
        reinterpret_cast<void*>(componentList));

    MenuExtras_CloseFile(&file, nullptr);
    return 1;
}

HOOK(0x0069DB80, C_0086c2b4_SetupFieldBuffer, "auto");
