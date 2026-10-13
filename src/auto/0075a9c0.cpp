#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, MenuManager_AppendPageName, 0x00470FD0, (S_00470ab0* self, void* , void* a3));
CSF_FUNC(unsigned int, __fastcall, GameBddLoader_ConfigureDisplayState, 0x0050C450, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, S_00692b80_GetChildCount, 0x00692B80, (S_00692b80* self, void*));
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_GetComponent, 0x00692BD0, (S_00692b80* self, void* , unsigned int index));
CSF_FUNC(int, __fastcall, S_00692b80_ParseIntegerComponent, 0x00692C40, (S_00692b80* self, void*));
CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0, (std::uint32_t component));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40, (std::uint8_t* attribute, void* edx, char* buffer, int capacity));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __cdecl, GameBddLoader_ShouldPreloadResources, 0x0069D5C0, (int a1));

CSF_GLOBAL(const char*, g_pszKey_FILE, 0x008A8CBC);
CSF_GLOBAL(const char*, g_pszKey_VOLUMEN, 0x008A8CC0);
CSF_GLOBAL(const char*, g_pszKey_LOOP, 0x008A8CC4);
CSF_GLOBAL(const char*, g_pszKey_ES3D, 0x008A8CC8);
CSF_GLOBAL(const char*, g_pszKey_ESAMBIENTE, 0x008A8CD0);
CSF_GLOBAL(const char*, g_pszKey_RADIO, 0x008A8CD8);
CSF_GLOBAL(const char*, g_pszKey_RADIO_LOGICO, 0x008A8CDC);

static void __stdcall SoundDefs_ParseFilesAndProperties(S_00692b80* definition, int destinationAddress)
{
    auto* destination = reinterpret_cast<std::uint8_t*>(destinationAddress);
    auto* node = reinterpret_cast<std::uint8_t*>(definition);

    alignas(8) char buffer[0x100];
    std::memset(buffer, 0, sizeof(buffer));

    alignas(8) std::uint8_t stringBytes[0x20];
    auto& filename = *reinterpret_cast<C_0082d410*>(stringBytes);

    // The packet also identifies the child-count entry point as this vtable.
    (void)CSF_VTBL(0x00692B80);

    auto* files = reinterpret_cast<S_00692b80*>(
        FindProperty(node, nullptr, const_cast<char*>(g_pszKey_FILE)));
    if (files && static_cast<unsigned int>(
                     S_00692b80_GetChildCount(files, nullptr)) > 0)
    {
        CSF_AT(unsigned int, stringBytes, 0x1C) =
            static_cast<unsigned int>(S_00692b80_GetChildCount(files, nullptr));
        if (static_cast<std::uint8_t>(GameBddLoader_ShouldPreloadResources(0)))
            CSF_AT(unsigned int, stringBytes, 0x1C) = 1;

        GameBddLoader_ConfigureDisplayState(
            destination + 4, nullptr, CSF_AT(unsigned int, stringBytes, 0x1C));

        unsigned int index = 0;
        bool moreFiles = index < CSF_AT(unsigned int, stringBytes, 0x1C);
        while (moreFiles)
        {
            auto* component = S_00692b80_GetComponent(files, nullptr, index);
            CopyAttributeText(reinterpret_cast<std::uint8_t*>(component),
                              nullptr, buffer, 255);

            CSF_AT(unsigned int, stringBytes, 0x18) = 15;
            CSF_AT(unsigned int, stringBytes, 0x14) = 0;
            reinterpret_cast<std::uint8_t*>(&filename.field_0004)[0] = 0;
            AssignStringRange_429B60(
                &filename, nullptr, buffer,
                static_cast<unsigned int>(std::strlen(buffer)));
            MenuManager_AppendPageName(
                reinterpret_cast<S_00470ab0*>(destination + 4), nullptr, &filename);

            if (CSF_AT(unsigned int, stringBytes, 0x18) >= 0x10 &&
                filename.field_0004)
            {
                Mem_FreeDebug(reinterpret_cast<void*>(filename.field_0004));
            }

            ++index;
            moreFiles = index < CSF_AT(unsigned int, stringBytes, 0x1C);
            CSF_AT(unsigned int, stringBytes, 0x18) = 15;
            CSF_AT(unsigned int, stringBytes, 0x14) = 0;
            reinterpret_cast<std::uint8_t*>(&filename.field_0004)[0] = 0;
        }
    }

    auto* property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_VOLUMEN));
    if (property)
        CSF_AT(int, destination, 0x14) = S_00692b80_ParseIntegerComponent(
            reinterpret_cast<S_00692b80*>(property), nullptr);

    property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_LOOP));
    if (property)
        CSF_AT(std::uint8_t, destination, 0x20) =
            S_00692b80_ParseIntegerComponent(
                reinterpret_cast<S_00692b80*>(property), nullptr) != 0;

    property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_ES3D));
    if (property)
        CSF_AT(std::uint8_t, destination, 0x21) =
            S_00692b80_ParseIntegerComponent(
                reinterpret_cast<S_00692b80*>(property), nullptr) != 0;

    property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_ESAMBIENTE));
    if (property)
        CSF_AT(std::uint8_t, destination, 0x22) =
            S_00692b80_ParseIntegerComponent(
                reinterpret_cast<S_00692b80*>(property), nullptr) != 0;

    property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_RADIO));
    if (property)
        CSF_AT(float, destination, 0x18) = static_cast<float>(
            ReadComponentFloat(reinterpret_cast<std::uint32_t>(property)));

    property = FindProperty(node, nullptr, const_cast<char*>(g_pszKey_RADIO_LOGICO));
    if (property)
        CSF_AT(float, destination, 0x1C) = static_cast<float>(
            ReadComponentFloat(reinterpret_cast<std::uint32_t>(property)));
    else
        CSF_AT(std::uint32_t, destination, 0x1C) =
            CSF_AT(std::uint32_t, destination, 0x18);
}

HOOK(0x0075A9C0, SoundDefs_ParseFilesAndProperties, "auto");
