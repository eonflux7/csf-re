#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, MpGameModes_AppendScene, 0x0054EA20, (void* self, void* edx, uint32_t* a3));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(int, __cdecl, CoreFiles_CheckPageAvailability, 0x00698B20, (const char* path, char bypassList));

static char __fastcall MpGameModes_LoadScenes(void* self, void* /*edx*/)
{
    // The packet also labels these direct-callee addresses as vtables.
    (void)CSF_VTBL(0x00692B10);
    (void)CSF_VTBL(0x00692B80);

    if (static_cast<std::uint8_t>(CoreFiles_CheckPageAvailability(
            CSF_STR(0x0084449C, "Menus/EscenasMP.txt"), 0)))
    {
        alignas(8) std::uint8_t fileBytes[0x44];
        S_00692b10& file = *reinterpret_cast<S_00692b10*>(fileBytes);

        MenuExtras_InitializeFile(&file, nullptr);
        if (MenuExtras_LoadFile(
                &file, nullptr, CSF_STR(0x0084449C, "Menus/EscenasMP.txt")))
        {
            if (file.field_0000)
            {
                auto* scenes = FindProperty(
                    reinterpret_cast<std::uint8_t*>(file.field_0000),
                    nullptr, const_cast<char*>(CSF_STR(0x00844490, ".ESCENAS")));

                if (scenes)
                {
                    alignas(8) std::uint8_t entryBytes[0x8];
                    CSF_AT(std::uint32_t, entryBytes, 0x4) = 0;

                    if (MenuExtras_GetChildCount(scenes, nullptr))
                    {
                        do
                        {
                            auto* component = reinterpret_cast<std::uint8_t*>(
                                GetComponent(scenes, nullptr,
                                    CSF_AT(std::uint32_t, entryBytes, 0x4)));

                            if (component)
                            {
                                auto* scene = static_cast<char*>(Mem_Alloc(769));
                                CSF_AT(std::uint32_t, entryBytes, 0) =
                                    reinterpret_cast<std::uint32_t>(scene);

                                if (scene)
                                {
                                    scene[512] = 0;
                                    scene[0] = 0;
                                    scene[256] = 0;
                                    scene[768] = 0;

                                    MenuExtras_ReadText(component, nullptr,
                                        const_cast<char*>(CSF_STR(0x00844488, ".SCN")),
                                        scene, 256);
                                    MenuExtras_ReadText(component, nullptr,
                                        const_cast<char*>(CSF_STR(0x00830F8C, ".NOMBRE")),
                                        scene + 256, 256);
                                    MenuExtras_ReadText(component, nullptr,
                                        const_cast<char*>(CSF_STR(0x00830F20, ".PANTALLA_CARGA")),
                                        scene + 512, 256);

                                    if (ReadIntegerProperty(component, nullptr,
                                            const_cast<char*>(CSF_STR(0x00844480, ".DMATCH"))))
                                        scene[768] |= 1u;
                                    if (ReadIntegerProperty(component, nullptr,
                                            const_cast<char*>(CSF_STR(0x00844474, ".TDMATCH"))))
                                        scene[768] |= 2u;
                                    if (ReadIntegerProperty(component, nullptr,
                                            const_cast<char*>(CSF_STR(0x00844468, ".POSTMEN"))))
                                        scene[768] |= 4u;
                                    if (ReadIntegerProperty(component, nullptr,
                                            const_cast<char*>(CSF_STR(0x0084445C, ".SABOTAGE"))))
                                        scene[768] |= 8u;

                                    MpGameModes_AppendScene(self, nullptr,
                                        reinterpret_cast<std::uint32_t*>(entryBytes));
                                    ++CSF_AT(std::uint32_t, self, 0x10);
                                }
                            }

                            ++CSF_AT(std::uint32_t, entryBytes, 0x4);
                        }
                        while (CSF_AT(std::uint32_t, entryBytes, 0x4) <
                               MenuExtras_GetChildCount(scenes, nullptr));
                    }
                }
            }
        }
        MenuExtras_CloseFile(&file, nullptr);
    }
    return 0;
}

HOOK(0x0054EA90, MpGameModes_LoadScenes, "auto");
