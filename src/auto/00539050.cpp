#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(int, __fastcall, Campaign_FindMission, 0x0043E4B0, (void* self, void* , char* sceneName));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __cdecl, MenuLoading_GetMissionName, 0x00538F40, (void* destination));
CSF_FUNC(int, __cdecl, GetGlobalDword_8B0680, 0x0054E520, ());
CSF_FUNC(const char*, __fastcall, C_0082d410_FindSessionMapDefinition, 0x0054E630, (C_0082d410* self, void* , int a2));
CSF_FUNC(int, __cdecl, MenuMpOnline_Release, 0x0054E780, ());
CSF_FUNC(int, __cdecl, OnlineMenu_PrepareEnvironment, 0x0054EC30, ());
CSF_FUNC(const void*, __cdecl, RwImageSetPath, 0x00625270, (const void* path));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));
CSF_FUNC(unsigned int, __cdecl, SceneLoader_CopyMapFilename, 0x006948E0, (char* a1, char* a2, unsigned int a3));
CSF_FUNC(char*, __cdecl, Hud_ExtractResourcePath, 0x00694960, (char* a1, uint8_t* a2, unsigned int a3));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static void MenuLoading_InitializeEmptyString(C_0082d410* string)
{
    CSF_AT(std::uint32_t, string, 0x18) = 15;
    CSF_AT(std::uint32_t, string, 0x14) = 0;
    *reinterpret_cast<uint8_t*>(&string->field_0004) = 0;
}

static char* MenuLoading_GetStringData(C_0082d410* string)
{
    if (CSF_AT(std::uint32_t, string, 0x18) < 0x10)
        return reinterpret_cast<char*>(&string->field_0004);
    return reinterpret_cast<char*>(string->field_0004);
}

static void* __fastcall CMenuLoading_InitializePanelContents(
    CMenuLoading* self, void* /*edx*/)
{
    alignas(8) uint8_t filenameBytes[0x100];
    alignas(8) uint8_t directoryBytes[0x100];
    alignas(8) uint8_t emptyDescriptionBytes[0x1C];
    alignas(8) uint8_t missionNameBytes[0x1C];
    alignas(8) uint8_t sceneListBytes[0x1C];
    alignas(8) uint8_t loadingImageBytes[0x28];

    auto& emptyDescription =
        *reinterpret_cast<C_0082d410*>(emptyDescriptionBytes);
    auto& missionName =
        *reinterpret_cast<C_0082d410*>(missionNameBytes);
    auto& sceneList =
        *reinterpret_cast<C_0082d410*>(sceneListBytes);
    auto& loadingImage =
        *reinterpret_cast<C_0082d410*>(loadingImageBytes);

    std::memset(directoryBytes, 0, sizeof(directoryBytes));
    std::memset(filenameBytes, 0, sizeof(filenameBytes));

    MenuLoading_InitializeEmptyString(&missionName);
    MenuLoading_GetMissionName(&missionName);

    MenuLoading_InitializeEmptyString(&sceneList);
    AssignStringRange_429B60(
        &sceneList, nullptr,
        const_cast<char*>(CSF_STR(0x00831038, "Menus/Escenas.fbs")), 17);

    Hud_ExtractResourcePath(
        const_cast<char*>(CSF_STR(0x008421D4, "Menus/Textures/loading.png")),
        directoryBytes, 256);
    SceneLoader_CopyMapFilename(
        const_cast<char*>(CSF_STR(0x008421D4, "Menus/Textures/loading.png")),
        reinterpret_cast<char*>(filenameBytes), 256);

    char* emptyText = reinterpret_cast<char*>(&g_byte_82E65C);
    MenuLoading_InitializeEmptyString(&loadingImage);
    AssignStringRange_429B60(&loadingImage, nullptr, emptyText, 0);

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
    {
        bool environmentAlreadyPresent = true;
        if (!GetGlobalDword_8B0680())
        {
            environmentAlreadyPresent = false;
            OnlineMenu_PrepareEnvironment();
        }

        auto* session =
            reinterpret_cast<C_0082d410*>(GetGlobalDword_8B0680());
        const char* definition = C_0082d410_FindSessionMapDefinition(
            session, nullptr, reinterpret_cast<int>(&missionName));
        char* imagePath = definition
            ? const_cast<char*>(definition + 512)
            : emptyText;
        AssignStringRange_429B60(
            &loadingImage, nullptr, imagePath,
            static_cast<unsigned int>(std::strlen(imagePath)));

        if (!environmentAlreadyPresent)
            MenuMpOnline_Release();
    }
    else
    {
        char* name = MenuLoading_GetStringData(&missionName);
        void* campaign = reinterpret_cast<void*>(GetGlobal8afc7c());
        void* mission =
            reinterpret_cast<void*>(Campaign_FindMission(campaign, nullptr, name));

        const void* description;
        bool usedEmptyDescription = false;
        if (mission)
        {
            description = static_cast<uint8_t*>(mission) + 0x9C;
        }
        else
        {
            usedEmptyDescription = true;
            MenuLoading_InitializeEmptyString(&emptyDescription);
            AssignStringRange_429B60(&emptyDescription, nullptr, emptyText, 0);
            description = &emptyDescription;
        }

        AssignStringSubstring(&loadingImage, nullptr, description, 0, -1);
        if (usedEmptyDescription &&
            CSF_AT(std::uint32_t, &emptyDescription, 0x18) >= 0x10 &&
            emptyDescription.field_0004)
        {
            Mem_FreeDebug(reinterpret_cast<void*>(emptyDescription.field_0004));
        }
    }

    int image = 0;
    if (CompareStringSubstring(
            &loadingImage, nullptr, 0,
            CSF_AT(std::uint32_t, &loadingImage, 0x14), emptyText, 0))
    {
        AssignStringSubstring(&sceneList, nullptr, &loadingImage, 0, -1);
        Hud_ExtractResourcePath(
            MenuLoading_GetStringData(&loadingImage), directoryBytes, 256);
        SceneLoader_CopyMapFilename(
            MenuLoading_GetStringData(&loadingImage),
            reinterpret_cast<char*>(filenameBytes), 256);
        RwImageSetPath(directoryBytes);

        void* controls = MenuControls_ResolveObject(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        image = reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*)>(
            CSF_AT(void**, controls, 0)[0x40 / 4])(
                controls, nullptr, filenameBytes);
    }

    if (!image)
    {
        RwImageSetPath(directoryBytes);
        void* controls = MenuControls_ResolveObject(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        image = reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*)>(
            CSF_AT(void**, controls, 0)[0x40 / 4])(
                controls, nullptr, filenameBytes);
    }

    if (image)
    {
        void* panel = reinterpret_cast<void*>(self->m_ownedObjectA);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, panel, 0)[0xD8 / 4])(panel, nullptr, image);
        ReleaseObjectReference(
            reinterpret_cast<uint8_t*>(image) + 4, nullptr);
    }

    void* result = reinterpret_cast<void*>(
        CSF_AT(std::uint32_t, &loadingImage, 0x18));
    if (CSF_AT(std::uint32_t, &loadingImage, 0x18) >= 0x10)
    {
        result = reinterpret_cast<void*>(loadingImage.field_0004);
        if (loadingImage.field_0004)
            result = reinterpret_cast<void*>(
                Mem_FreeDebug(reinterpret_cast<void*>(loadingImage.field_0004)));
    }
    MenuLoading_InitializeEmptyString(&loadingImage);

    if (CSF_AT(std::uint32_t, &sceneList, 0x18) >= 0x10)
    {
        result = reinterpret_cast<void*>(sceneList.field_0004);
        if (sceneList.field_0004)
            result = reinterpret_cast<void*>(
                Mem_FreeDebug(reinterpret_cast<void*>(sceneList.field_0004)));
    }
    MenuLoading_InitializeEmptyString(&sceneList);

    if (CSF_AT(std::uint32_t, &missionName, 0x18) >= 0x10)
    {
        result = reinterpret_cast<void*>(missionName.field_0004);
        if (missionName.field_0004)
            return reinterpret_cast<void*>(
                Mem_FreeDebug(reinterpret_cast<void*>(missionName.field_0004)));
    }
    return result;
}

HOOK(0x00539050, CMenuLoading_InitializePanelContents, "auto");
