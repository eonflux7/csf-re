#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
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
CSF_GLOBAL(const char*, g_pszKey_LOCALIZABLE, 0x008A8CCC);
CSF_GLOBAL(const char*, g_pszKey_ESAMBIENTE, 0x008A8CD0);
CSF_GLOBAL(const char*, g_pszKey_AGUA, 0x008A8CD4);
CSF_GLOBAL(const char*, g_pszKey_RADIO, 0x008A8CD8);
CSF_GLOBAL(const char*, g_pszKey_RADIO_LOGICO, 0x008A8CDC);
CSF_GLOBAL(const char*, g_pszKey_QUALITY_8A8CE4, 0x008A8CE4);
CSF_GLOBAL(const char*, g_pszKey_QUALITY_PS2, 0x008A8CE8);
CSF_GLOBAL(const char*, g_pszKey_QUALITY_XBOX, 0x008A8CEC);

static std::uint8_t* __stdcall SoundDefs_ParseSoundProperties(
    S_00692b80* source, int destinationAddress, unsigned int preloadLimit)
{
    alignas(8) std::uint8_t textBytes[0x100];
    alignas(8) std::uint8_t stringBytes[0x24];
    char* text = reinterpret_cast<char*>(textBytes);
    C_0082d410& pageName = *reinterpret_cast<C_0082d410*>(stringBytes);
    auto* destination = reinterpret_cast<std::uint8_t*>(destinationAddress);

    std::memset(textBytes, 0, sizeof(textBytes));

    // The packet labels the direct GetChildCount address as a vtable reference.
    (void)CSF_VTBL(0x00692B80);

    auto* files = reinterpret_cast<S_00692b80*>(
        FindProperty(reinterpret_cast<std::uint8_t*>(source), nullptr,
                     const_cast<char*>(g_pszKey_FILE)));

    const bool preload =
        static_cast<std::uint8_t>(GameBddLoader_ShouldPreloadResources(0)) != 0 ||
        static_cast<std::uint8_t>(GameBddLoader_ShouldPreloadResources(1)) != 0;

    if (files && S_00692b80_GetChildCount(files, nullptr) != 0)
    {
        unsigned int count = static_cast<unsigned int>(
            S_00692b80_GetChildCount(files, nullptr));
        if (preload && count >= preloadLimit)
            count = preloadLimit;

        GameBddLoader_ConfigureDisplayState(destination + 4, nullptr, count);

        for (unsigned int index = 0; index < count; ++index)
        {
            S_00692b80* component =
                S_00692b80_GetComponent(files, nullptr, index);
            CopyAttributeText(reinterpret_cast<std::uint8_t*>(component),
                              nullptr, text, 255);

            CSF_AT(std::uint32_t, stringBytes, 0x18) = 15;
            CSF_AT(std::uint32_t, stringBytes, 0x14) = 0;
            *reinterpret_cast<std::uint8_t*>(&pageName.field_0004) = 0;

            AssignStringRange_429B60(
                &pageName, nullptr, text,
                static_cast<unsigned int>(std::strlen(text)));
            MenuManager_AppendPageName(
                reinterpret_cast<S_00470ab0*>(destination + 4),
                nullptr, &pageName);

            if (CSF_AT(std::uint32_t, stringBytes, 0x18) >= 0x10 &&
                pageName.field_0004 != 0)
            {
                Mem_FreeDebug(reinterpret_cast<void*>(pageName.field_0004));
            }

            CSF_AT(std::uint32_t, stringBytes, 0x18) = 15;
            CSF_AT(std::uint32_t, stringBytes, 0x14) = 0;
            *reinterpret_cast<std::uint8_t*>(&pageName.field_0004) = 0;
        }
    }

    auto* property = FindProperty(
        reinterpret_cast<std::uint8_t*>(source), nullptr,
        const_cast<char*>(g_pszKey_VOLUMEN));
    if (property)
        CSF_AT(int, destination, 0x14) = S_00692b80_ParseIntegerComponent(
            reinterpret_cast<S_00692b80*>(property), nullptr);

    property = FindProperty(reinterpret_cast<std::uint8_t*>(source), nullptr,
                            const_cast<char*>(g_pszKey_LOOP));
    if (property)
        CSF_AT(std::uint8_t, destination, 0x20) =
            S_00692b80_ParseIntegerComponent(
                reinterpret_cast<S_00692b80*>(property), nullptr) != 0;

    property = FindProperty(reinterpret_cast<std::uint8_t*>(source), nullptr,
                            const_cast<char*>(g_pszKey_ES3D));
    if (property)
        CSF_AT(std::uint8_t, destination, 0x21) =
            S_00692b80_ParseIntegerComponent(
                reinterpret_cast<S_00692b80*>(property), nullptr) != 0;

    property = FindProperty(reinterpret_cast<std::uint8_t*>(source), nullptr,
                            const_cast<char*>(g_pszKey_LOCALIZABLE));
    if (property)
        CSF_AT(std::uint8_t, destination, 0x23) =
            S_00692b80_ParseIntegerComponent(
                reinterpret_cast<S_00692b80*>(property), nullptr) != 0;

    property = FindProperty(reinterpret_cast<std::uint8_t*>(source), nullptr,
                            const_cast<char*>(g_pszKey_ESAMBIENTE));
    if (property)
        CSF_AT(std::uint8_t, destination, 0x22) =
            S_00692b80_ParseIntegerComponent(
                reinterpret_cast<S_00692b80*>(property), nullptr) != 0;

    property = FindProperty(reinterpret_cast<std::uint8_t*>(source), nullptr,
                            const_cast<char*>(g_pszKey_AGUA));
    if (property)
        CSF_AT(std::uint8_t, destination, 0x24) =
            S_00692b80_ParseIntegerComponent(
                reinterpret_cast<S_00692b80*>(property), nullptr) != 0;

    property = FindProperty(reinterpret_cast<std::uint8_t*>(source), nullptr,
                            const_cast<char*>(g_pszKey_RADIO));
    if (property)
        CSF_AT(float, destination, 0x18) = static_cast<float>(
            ReadComponentFloat(reinterpret_cast<std::uint32_t>(property)));

    property = FindProperty(reinterpret_cast<std::uint8_t*>(source), nullptr,
                            const_cast<char*>(g_pszKey_RADIO_LOGICO));
    if (property)
        CSF_AT(float, destination, 0x1C) = static_cast<float>(
            ReadComponentFloat(reinterpret_cast<std::uint32_t>(property)));
    else
        CSF_AT(std::uint32_t, destination, 0x1C) =
            CSF_AT(std::uint32_t, destination, 0x18);

    property = FindProperty(reinterpret_cast<std::uint8_t*>(source), nullptr,
                            const_cast<char*>(g_pszKey_QUALITY_8A8CE4));
    if (property)
        CSF_AT(std::uint8_t, destination, 0x25) =
            static_cast<std::uint8_t>(S_00692b80_ParseIntegerComponent(
                reinterpret_cast<S_00692b80*>(property), nullptr));

    property = FindProperty(reinterpret_cast<std::uint8_t*>(source), nullptr,
                            const_cast<char*>(g_pszKey_QUALITY_PS2));
    if (property)
        CSF_AT(std::uint8_t, destination, 0x26) =
            static_cast<std::uint8_t>(S_00692b80_ParseIntegerComponent(
                reinterpret_cast<S_00692b80*>(property), nullptr));

    property = FindProperty(reinterpret_cast<std::uint8_t*>(source), nullptr,
                            const_cast<char*>(g_pszKey_QUALITY_XBOX));
    if (!property)
        return nullptr;

    const int quality = S_00692b80_ParseIntegerComponent(
        reinterpret_cast<S_00692b80*>(property), nullptr);
    CSF_AT(std::uint8_t, destination, 0x27) =
        static_cast<std::uint8_t>(quality);
    return reinterpret_cast<std::uint8_t*>(quality);
}

HOOK(0x0075ABB0, SoundDefs_ParseSoundProperties, "auto");
