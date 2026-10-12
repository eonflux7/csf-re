#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(unsigned int, __fastcall, Campaign_ReserveCampaigns, 0x00440020, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __cdecl, Campaign_ClearCampaigns, 0x00440BA0, (int a3));
CSF_FUNC(uint32_t*, __fastcall, Campaign_AddCampaign, 0x00441320, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));

CSF_GLOBAL(const char*, g_pszKey_CAMPANYAS, 0x008996B8);
CSF_GLOBAL(const char*, g_pszKey_NOMBRE, 0x008996BC);
CSF_GLOBAL(const char*, g_pszKey_INICIO_SCN, 0x008996C0);

static char __fastcall Campaign_LoadCampaigns(
    S_00441520* self, void* /*edx*/, S_00693740* path)
{
    alignas(8) uint8_t fileBytes[0x3C];
    S_00692b10& file = *reinterpret_cast<S_00692b10*>(fileBytes);

    (void)CSF_VTBL(0x00692B10);
    (void)CSF_VTBL(0x00692B80);

    MenuExtras_InitializeFile(&file, nullptr);
    if (!MenuExtras_LoadFile(&file, nullptr, reinterpret_cast<const char*>(path)))
    {
        MenuExtras_CloseFile(&file, nullptr);
        return 0;
    }

    uint8_t* campaigns = LookupNamedElement(
        reinterpret_cast<uint8_t*>(file.field_0000),
        nullptr, g_pszKey_CAMPANYAS);

    // Preserve the adjacent stack objects and their trailing stack scalars.
    alignas(8) uint8_t entryBytes[0x3C];
    C_0082d410& name = *reinterpret_cast<C_0082d410*>(entryBytes);
    C_0082d410& initialScene =
        *reinterpret_cast<C_0082d410*>(entryBytes + 0x1C);

    unsigned int& nameCapacity =
        reinterpret_cast<unsigned int*>(entryBytes)[6];
    int& nameLength = reinterpret_cast<int*>(entryBytes)[5];
    unsigned int& sceneCapacity =
        reinterpret_cast<unsigned int*>(entryBytes)[13];
    int& sceneLength = reinterpret_cast<int*>(entryBytes)[12];

    nameCapacity = 15;
    nameLength = 0;
    CSF_AT(uint8_t, &name.field_0004, 0) = 0;
    sceneCapacity = 15;
    sceneLength = 0;
    CSF_AT(uint8_t, &initialScene.field_0004, 0) = 0;

    CSF_AT(void*, &initialScene, 0x1C) =
        reinterpret_cast<uint8_t*>(self) + 0x20;

    Campaign_ClearCampaigns(
        reinterpret_cast<int>(CSF_AT(void*, &initialScene, 0x1C)));

    const unsigned int count = MenuExtras_GetChildCount(campaigns, nullptr);
    if (count != 0)
    {
        Campaign_ReserveCampaigns(
            CSF_AT(void*, &initialScene, 0x1C), nullptr, count);

        char buffer[256];
        for (unsigned int index = 0; index < count; ++index)
        {
            uint8_t* component = reinterpret_cast<uint8_t*>(
                GetComponent(campaigns, nullptr, index));

            MenuExtras_ReadText(component, nullptr,
                const_cast<char*>(g_pszKey_NOMBRE), buffer, 256);
            AssignStringRange_429B60(&name, nullptr, buffer,
                static_cast<unsigned int>(std::strlen(buffer)));

            MenuExtras_ReadText(component, nullptr,
                const_cast<char*>(g_pszKey_INICIO_SCN), buffer, 256);
            AssignStringRange_429B60(&initialScene, nullptr, buffer,
                static_cast<unsigned int>(std::strlen(buffer)));

            Campaign_AddCampaign(
                CSF_AT(void*, &initialScene, 0x1C), nullptr,
                reinterpret_cast<int>(&name));
        }
    }

    if (sceneCapacity >= 0x10 && initialScene.field_0004 != 0)
        Mem_FreeDebug(reinterpret_cast<void*>(initialScene.field_0004));

    sceneCapacity = 15;
    sceneLength = 0;
    CSF_AT(uint8_t, &initialScene.field_0004, 0) = 0;

    if (nameCapacity >= 0x10 && name.field_0004 != 0)
        Mem_FreeDebug(reinterpret_cast<void*>(name.field_0004));

    nameCapacity = 15;
    nameLength = 0;
    CSF_AT(uint8_t, &name.field_0004, 0) = 0;

    MenuExtras_CloseFile(&file, nullptr);
    return 1;
}

HOOK(0x00441520, Campaign_LoadCampaigns, "auto");
