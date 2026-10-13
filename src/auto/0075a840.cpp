#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, MenuManager_AppendPageName, 0x00470FD0, (S_00470ab0* self, void* , void* a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, S_00692b80_ParseIntegerComponent, 0x00692C40, (S_00692b80* self, void*));
CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0, (std::uint32_t component));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40, (std::uint8_t* attribute, void* edx, char* buffer, int capacity));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));

CSF_GLOBAL(const char*, g_pszKey_FILE, 0x008A8CBC);
CSF_GLOBAL(const char*, g_pszKey_VOLUMEN, 0x008A8CC0);
CSF_GLOBAL(const char*, g_pszKey_LOOP, 0x008A8CC4);
CSF_GLOBAL(const char*, g_pszKey_ES3D, 0x008A8CC8);
CSF_GLOBAL(const char*, g_pszKey_ESAMBIENTE, 0x008A8CD0);
CSF_GLOBAL(const char*, g_pszKey_RADIO, 0x008A8CD8);
CSF_GLOBAL(const char*, g_pszKey_RADIO_LOGICO, 0x008A8CDC);

static void __stdcall SoundDefs_ParseSoundProperties(
    S_00692b80* attributes, int soundAddress)
{
    auto* source = reinterpret_cast<std::uint8_t*>(attributes);
    auto* sound = reinterpret_cast<std::uint8_t*>(soundAddress);
    alignas(8) std::uint8_t textBytes[0x100] = {};
    auto* text = reinterpret_cast<char*>(textBytes);

    auto* property = FindProperty(source, nullptr, const_cast<char*>(g_pszKey_FILE));
    if (property)
    {
        CopyAttributeText(property, nullptr, text, 256);

        alignas(8) std::uint8_t nameBytes[0x1C];
        auto& name = *reinterpret_cast<C_0082d410*>(nameBytes);
        CSF_AT(std::uint32_t, &name, 0x18) = 15;
        CSF_AT(std::uint32_t, &name, 0x14) = 0;
        *reinterpret_cast<std::uint8_t*>(&name.field_0004) = 0;

        AssignStringRange_429B60(
            &name, nullptr, text, static_cast<unsigned int>(std::strlen(text)));
        MenuManager_AppendPageName(
            reinterpret_cast<S_00470ab0*>(sound + 4), nullptr, &name);

        if (CSF_AT(std::uint32_t, &name, 0x18) >= 0x10)
        {
            void* allocation = reinterpret_cast<void*>(name.field_0004);
            if (allocation)
                Mem_FreeDebug(allocation);
        }
    }

    property = FindProperty(source, nullptr, const_cast<char*>(g_pszKey_VOLUMEN));
    if (property)
        CSF_AT(int, sound, 0x14) = S_00692b80_ParseIntegerComponent(
            reinterpret_cast<S_00692b80*>(property), nullptr);

    property = FindProperty(source, nullptr, const_cast<char*>(g_pszKey_LOOP));
    if (property)
        CSF_AT(std::uint8_t, sound, 0x20) =
            S_00692b80_ParseIntegerComponent(
                reinterpret_cast<S_00692b80*>(property), nullptr) != 0;

    property = FindProperty(source, nullptr, const_cast<char*>(g_pszKey_ES3D));
    if (property)
        CSF_AT(std::uint8_t, sound, 0x21) =
            S_00692b80_ParseIntegerComponent(
                reinterpret_cast<S_00692b80*>(property), nullptr) != 0;

    property = FindProperty(source, nullptr, const_cast<char*>(g_pszKey_ESAMBIENTE));
    if (property)
        CSF_AT(std::uint8_t, sound, 0x22) =
            S_00692b80_ParseIntegerComponent(
                reinterpret_cast<S_00692b80*>(property), nullptr) != 0;

    property = FindProperty(source, nullptr, const_cast<char*>(g_pszKey_RADIO));
    if (property)
        CSF_AT(float, sound, 0x18) = static_cast<float>(
            ReadComponentFloat(reinterpret_cast<std::uint32_t>(property)));

    property = FindProperty(source, nullptr, const_cast<char*>(g_pszKey_RADIO_LOGICO));
    if (property)
        CSF_AT(float, sound, 0x1C) = static_cast<float>(
            ReadComponentFloat(reinterpret_cast<std::uint32_t>(property)));
    else
        CSF_AT(std::uint32_t, sound, 0x1C) =
            CSF_AT(std::uint32_t, sound, 0x18);
}

HOOK(0x0075A840, SoundDefs_ParseSoundProperties, "auto");
