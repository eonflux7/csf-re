#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, MenuManager_AppendPageName, 0x00470FD0, (S_00470ab0* self, void* , void* a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, S_00692b80_ParseIntegerComponent, 0x00692C40, (S_00692b80* self, void*));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40, (std::uint8_t* attribute, void* edx, char* buffer, int capacity));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));

CSF_GLOBAL(const char*, g_pszKey_FILE, 0x008A8CBC);
CSF_GLOBAL(const char*, g_pszKey_VOLUMEN, 0x008A8CC0);
CSF_GLOBAL(const char*, g_pszKey_LOOP, 0x008A8CC4);
CSF_GLOBAL(const char*, g_pszKey_ES3D, 0x008A8CC8);

static std::uint8_t* __stdcall BddSoundDefs_ParseSoundProperties(
    S_00692b80* attributes, int destinationAddress)
{
    char buffer[0x100] = {};
    auto* destination = reinterpret_cast<std::uint8_t*>(destinationAddress);

    auto* property = FindProperty(
        reinterpret_cast<std::uint8_t*>(attributes), nullptr,
        const_cast<char*>(g_pszKey_FILE));

    if (property)
    {
        CopyAttributeText(property, nullptr, buffer, 256);

        alignas(8) std::uint8_t nameBytes[0x1C];
        C_0082d410& name = *reinterpret_cast<C_0082d410*>(nameBytes);
        CSF_AT(std::uint32_t, &name, 0x18) = 15;
        CSF_AT(std::uint32_t, &name, 0x14) = 0;
        reinterpret_cast<char*>(&name.field_0004)[0] = 0;

        AssignStringRange_429B60(
            &name, nullptr, buffer,
            static_cast<unsigned int>(std::strlen(buffer)));
        MenuManager_AppendPageName(
            reinterpret_cast<S_00470ab0*>(destination + 4), nullptr, &name);

        if (CSF_AT(std::uint32_t, &name, 0x18) >= 0x10)
        {
            if (name.field_0004)
                Mem_FreeDebug(reinterpret_cast<void*>(name.field_0004));
        }
    }

    property = FindProperty(
        reinterpret_cast<std::uint8_t*>(attributes), nullptr,
        const_cast<char*>(g_pszKey_VOLUMEN));
    if (property)
    {
        CSF_AT(int, destination, 0x14) =
            S_00692b80_ParseIntegerComponent(
                reinterpret_cast<S_00692b80*>(property), nullptr);
    }

    property = FindProperty(
        reinterpret_cast<std::uint8_t*>(attributes), nullptr,
        const_cast<char*>(g_pszKey_LOOP));
    if (property)
    {
        CSF_AT(std::uint8_t, destination, 0x20) =
            S_00692b80_ParseIntegerComponent(
                reinterpret_cast<S_00692b80*>(property), nullptr) != 0;
    }

    property = FindProperty(
        reinterpret_cast<std::uint8_t*>(attributes), nullptr,
        const_cast<char*>(g_pszKey_ES3D));
    if (property)
    {
        int value = S_00692b80_ParseIntegerComponent(
            reinterpret_cast<S_00692b80*>(property), nullptr);
        CSF_AT(std::uint8_t, destination, 0x21) = value != 0;
        return reinterpret_cast<std::uint8_t*>(value);
    }

    return property;
}

HOOK(0x0075A720, BddSoundDefs_ParseSoundProperties, "auto");
