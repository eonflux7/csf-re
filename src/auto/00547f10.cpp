#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770,
         (void* self, void* edx, const char* path));
CSF_FUNC(void*, __fastcall, MenuExtras_FindSection, 0x00693360,
         (void* self, void* edx, const char* name));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80,
         (void* self, void* edx));
CSF_FUNC(void*, __fastcall, MenuExtras_GetChild, 0x00692BD0,
         (void* self, void* edx, std::uint32_t index));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0,
         (void* self, void* edx, const char* name, char* buffer, int capacity));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_ReserveThumbnails, 0x00547160,
         (void* self, void* edx, std::uint32_t count));
CSF_FUNC(std::uint32_t*, __fastcall, MenuExtras_AppendThumbnail, 0x00547930,
         (void* self, void* edx, std::uint32_t* entry));
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90,
         (const char* text));
CSF_FUNC(void*, __fastcall, MenuExtras_AssignString, 0x00403C60,
         (void* self, int registerArgument, char* text, std::uint32_t length));
CSF_FUNC(int, __cdecl, MenuExtras_FreeStringStorage, 0x00691110,
         (void* storage));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380,
         (void* self, void* edx));

static int __fastcall CMenuExtras_LoadThumbnails(CMenuExtras* self, void* /*edx*/)
{
    alignas(4) std::uint32_t file[15];
    char description[8];
    char screenshot[100];
    char scene[100];
    alignas(4) std::uint8_t entry[68];

    MenuExtras_InitializeFile(file, nullptr);
    MenuExtras_LoadFile(file, nullptr,
                       CSF_STR(0x00843D24, "menus\\thumbnails.fbs"));

    void* root = reinterpret_cast<void*>(file[0]);
    if (root)
    {
        void* section = MenuExtras_FindSection(
            root, nullptr, CSF_STR(0x00843D18, ".THUMBNAILS"));
        if (section)
        {
            const std::uint32_t count =
                MenuExtras_GetChildCount(section, nullptr);
            void* thumbnails = reinterpret_cast<std::uint8_t*>(self) + 0x34;
            MenuExtras_ReserveThumbnails(thumbnails, nullptr, count);

            for (std::uint32_t index = 0; index < count; ++index)
            {
                void* child = MenuExtras_GetChild(section, nullptr, index);
                if (!child)
                    continue;

                MenuExtras_ReadText(child, nullptr, CSF_STR(0x00843D10, ".DESC"),
                                    description, 5);
                MenuExtras_ReadText(child, nullptr, CSF_STR(0x00843D08, ".SSHOT"),
                                    screenshot, 100);
                MenuExtras_ReadText(child, nullptr, CSF_STR(0x00843D00, ".SCENE"),
                                    scene, 100);

                CSF_AT(std::uint32_t, entry, 0x1C) = 15;
                CSF_AT(std::uint32_t, entry, 0x18) = 0;
                CSF_AT(std::uint8_t, entry, 0x08) = 0;
                CSF_AT(std::uint32_t, entry, 0x40) = 15;
                CSF_AT(std::uint32_t, entry, 0x3C) = 0;
                CSF_AT(std::uint8_t, entry, 0x2C) = 0;

                CSF_AT(int, entry, 0x00) =
                    MenuExtras_ParseDescriptionId(description);

                std::uint32_t screenshotLength = 0;
                while (screenshot[screenshotLength] != '\0')
                    ++screenshotLength;
                MenuExtras_AssignString(
                    entry + 0x04,
                    static_cast<int>(reinterpret_cast<std::uintptr_t>(screenshot)),
                    screenshot, screenshotLength);

                std::uint32_t sceneLength = 0;
                while (scene[sceneLength] != '\0')
                    ++sceneLength;
                MenuExtras_AssignString(
                    entry + 0x28,
                    static_cast<int>(reinterpret_cast<std::uintptr_t>(scene + 1)),
                    scene, sceneLength);

                CSF_AT(std::uint32_t, entry, 0x20) = 0;
                MenuExtras_AppendThumbnail(
                    thumbnails, nullptr, reinterpret_cast<std::uint32_t*>(entry));

                if (CSF_AT(std::uint32_t, entry, 0x40) >= 16)
                {
                    MenuExtras_FreeStringStorage(reinterpret_cast<void*>(
                        CSF_AT(std::uint32_t, entry, 0x2C)));
                }

                const bool screenshotHasHeapStorage =
                    CSF_AT(std::uint32_t, entry, 0x1C) >= 16;
                CSF_AT(std::uint32_t, entry, 0x40) = 15;
                CSF_AT(std::uint32_t, entry, 0x3C) = 0;
                CSF_AT(std::uint8_t, entry, 0x2C) = 0;

                if (screenshotHasHeapStorage)
                {
                    MenuExtras_FreeStringStorage(reinterpret_cast<void*>(
                        CSF_AT(std::uint32_t, entry, 0x08)));
                }
                CSF_AT(std::uint32_t, entry, 0x1C) = 15;
                CSF_AT(std::uint32_t, entry, 0x18) = 0;
                CSF_AT(std::uint8_t, entry, 0x08) = 0;
            }
        }
    }

    return MenuExtras_CloseFile(file, nullptr);
}

HOOK(0x00547F10, CMenuExtras_LoadThumbnails, "auto");
