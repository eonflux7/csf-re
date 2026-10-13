#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, S_00692b10_InitializeFile, 0x00692B10, (S_00692b10* self, void*));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

static int __stdcall HudPortraits_LoadPortrait(S_00693740* path)
{
    char texturePath[256];
    char imageName[256];
    alignas(8) std::uint8_t fileBytes[0x3C];
    char descriptionText[8];
    auto& file = *reinterpret_cast<S_00692b10*>(fileBytes);

    // The packet also identifies the initializer address as a vtable reference.
    (void)CSF_VTBL(0x00692B10);
    S_00692b10_InitializeFile(&file, nullptr);

    std::uint8_t* portrait = nullptr;
    std::uint8_t* properties = nullptr;

    if (path && static_cast<std::uint8_t>(path->field_0000))
    {
        if (MenuExtras_LoadFile(&file, nullptr, reinterpret_cast<const char*>(path)))
            properties = reinterpret_cast<std::uint8_t*>(file.field_0000);

        if (properties)
        {
            portrait = static_cast<std::uint8_t*>(Mem_Alloc(88));
            if (portrait)
            {
                CSF_AT(void**, portrait, 0x00) = CSF_VTBL(0x008312E8);
                CSF_AT(int, portrait, 0x04) = -1;
                CSF_AT(std::uint32_t, portrait, 0x1C) = 0;
                CSF_AT(std::uint32_t, portrait, 0x20) = 15;
                CSF_AT(char, portrait, 0x0C) = 0;
                CSF_AT(std::uint32_t, portrait, 0x3C) = 15;
                CSF_AT(std::uint32_t, portrait, 0x38) = 0;
                CSF_AT(char, portrait, 0x28) = 0;
                CSF_AT(int, portrait, 0x40) = -1;
                CSF_AT(int, portrait, 0x44) = -1;
                CSF_AT(int, portrait, 0x48) = 0;
                CSF_AT(char, portrait, 0x4C) = 0;
                CSF_AT(int, portrait, 0x50) = 0;
                CSF_AT(int, portrait, 0x54) = 0;

                MenuExtras_ReadText(
                    properties, nullptr,
                    const_cast<char*>(CSF_STR(0x00831380, ".FLI_NAME")),
                    descriptionText, 5);
                CSF_AT(int, portrait, 0x40) =
                    MenuExtras_ParseDescriptionId(descriptionText);

                MenuExtras_ReadText(
                    properties, nullptr,
                    const_cast<char*>(CSF_STR(0x00831378, ".FLI")),
                    descriptionText, 5);
                CSF_AT(int, portrait, 0x44) =
                    MenuExtras_ParseDescriptionId(descriptionText);

                AssignStringRange_429B60(
                    portrait + 0x08, nullptr, reinterpret_cast<char*>(path),
                    static_cast<unsigned int>(
                        std::strlen(reinterpret_cast<const char*>(path))));

                MenuExtras_ReadText(
                    properties, nullptr,
                    const_cast<char*>(CSF_STR(0x00831370, ".IMAGE")),
                    imageName, 256);
                _sprintf(
                    texturePath,
                    CSF_STR(0x00831354, "menus\\textures\\retratos\\%s"),
                    imageName);
                AssignStringRange_429B60(
                    portrait + 0x24, nullptr, imageName,
                    static_cast<unsigned int>(std::strlen(imageName)));

                void* global = reinterpret_cast<void*>(GetGlobalInt());
                void* accessor = reinterpret_cast<void*>(
                    GlobalInt_ResolveAccessor(global, nullptr));
                void* textureManager = reinterpret_cast<void*>(
                    ((int(__fastcall*)(void*, void*))
                        CSF_AT(void**, accessor, 0)[0x10 / 4])(
                            accessor, nullptr));
                ((void(__fastcall*)(void*, void*, const char*))
                    CSF_AT(void**, textureManager, 0)[0x10 / 4])(
                        textureManager, nullptr,
                        CSF_STR(0x0083132C, "menus\\textures\\retratos\\"));

                global = reinterpret_cast<void*>(GetGlobalInt());
                void* controls = MenuControls_ResolveObject(global, nullptr);
                const char* storedImageName =
                    CSF_AT(std::uint32_t, portrait, 0x3C) < 0x10u
                        ? reinterpret_cast<const char*>(portrait + 0x28)
                        : CSF_AT(char*, portrait, 0x28);

                CSF_AT(int, portrait, 0x50) =
                    ((int(__fastcall*)(void*, void*, const char*))
                        CSF_AT(void**, controls, 0)[0x7C / 4])(
                            controls, nullptr, storedImageName);

                global = reinterpret_cast<void*>(GetGlobalInt());
                controls = MenuControls_ResolveObject(global, nullptr);
                CSF_AT(int, portrait, 0x54) =
                    ((int(__fastcall*)(void*, void*, const char*))
                        CSF_AT(void**, controls, 0)[0x7C / 4])(
                            controls, nullptr,
                            CSF_STR(0x00831320, "reTach.png"));

                CSF_AT(char, portrait, 0x4C) =
                    ReadIntegerProperty(
                        properties, nullptr,
                        const_cast<char*>(CSF_STR(0x00831348, ".IS_GENERIC"))) != 0;
            }
        }
    }

    MenuExtras_CloseFile(&file, nullptr);
    return reinterpret_cast<int>(portrait);
}

HOOK(0x00457580, HudPortraits_LoadPortrait, "auto");
