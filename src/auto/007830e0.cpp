#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(S_00692b80*, __fastcall, LookupNamedElement, 0x00693360, (S_00692b80* self, void* , char* String1));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(uint32_t*, __fastcall, InitializeWeaponSettings, 0x00782590, (void* self, void* edx));
CSF_FUNC(void, __fastcall, ReadWeaponSettings, 0x007826A0, (void* self, void* edx, void* a1, int a2));
CSF_FUNC(uint32_t*, __fastcall, StoreSceneWeaponSettings, 0x00783000, (void* self, void* edx, void* a3));

CSF_GLOBAL(const char*, g_pszKey_ESCENA, 0x008A9C00);
CSF_GLOBAL(const char*, g_pszKey_SMG, 0x008A9C04);
CSF_GLOBAL(const char*, g_pszKey_MG, 0x008A9C08);
CSF_GLOBAL(const char*, g_pszKey_RIFLE, 0x008A9C0C);
CSF_GLOBAL(const char*, g_pszKey_ALIADOS, 0x008A9C10);

static char __fastcall S_00782f90_LoadSceneWeaponSettings(
    S_00782f90* self, void* /*edx*/, S_00692b80* source)
{
    alignas(8) std::uint8_t textBytes[0x100];
    char* text = reinterpret_cast<char*>(textBytes);
    std::memset(text, 0, sizeof(textBytes));

    MenuExtras_ReadText(
        reinterpret_cast<std::uint8_t*>(source), nullptr,
        const_cast<char*>(g_pszKey_ESCENA), text, 256);

    // The scene string and four settings blocks form one contiguous record.
    alignas(8) std::uint8_t sceneBytes[0x15C];
    C_0082d410& scene = *reinterpret_cast<C_0082d410*>(sceneBytes);
    void* smgSettings = sceneBytes + 0x1C;
    void* mgSettings = sceneBytes + 0x6C;
    void* rifleSettings = sceneBytes + 0xBC;
    void* alliedSettings = sceneBytes + 0x10C;

    CSF_AT(unsigned int, &scene, 0x18) = 15;
    CSF_AT(int, &scene, 0x14) = 0;
    reinterpret_cast<char*>(&scene.field_0004)[0] = 0;

    InitializeWeaponSettings(smgSettings, nullptr);
    InitializeWeaponSettings(mgSettings, nullptr);
    InitializeWeaponSettings(rifleSettings, nullptr);
    InitializeWeaponSettings(alliedSettings, nullptr);

    AssignStringRange_429B60(
        &scene, nullptr, const_cast<char*>(CSF_STR(0x0086760C, "DEFAULT")), 7);
    AssignStringRange_429B60(
        &scene, nullptr, text, static_cast<unsigned int>(std::strlen(text)));

    S_00692b80* element =
        LookupNamedElement(source, nullptr, const_cast<char*>(g_pszKey_SMG));
    if (element)
        ReadWeaponSettings(self, nullptr, element,
            static_cast<int>(reinterpret_cast<std::intptr_t>(smgSettings)));

    element = LookupNamedElement(source, nullptr, const_cast<char*>(g_pszKey_MG));
    if (element)
        ReadWeaponSettings(self, nullptr, element,
            static_cast<int>(reinterpret_cast<std::intptr_t>(mgSettings)));

    element = LookupNamedElement(source, nullptr, const_cast<char*>(g_pszKey_RIFLE));
    if (element)
        ReadWeaponSettings(self, nullptr, element,
            static_cast<int>(reinterpret_cast<std::intptr_t>(rifleSettings)));

    element = LookupNamedElement(source, nullptr, const_cast<char*>(g_pszKey_ALIADOS));
    if (element)
        ReadWeaponSettings(self, nullptr, element,
            static_cast<int>(reinterpret_cast<std::intptr_t>(alliedSettings)));

    char result = static_cast<char>(
        reinterpret_cast<std::uintptr_t>(
            StoreSceneWeaponSettings(self, nullptr, &scene)));

    if (CSF_AT(unsigned int, &scene, 0x18) >= 0x10)
    {
        const std::uint32_t stringPointer = scene.field_0004;
        result = static_cast<char>(stringPointer);
        if (stringPointer)
            result = static_cast<char>(
                Mem_FreeDebug(reinterpret_cast<void*>(stringPointer)));
    }
    return result;
}

HOOK(0x007830E0, S_00782f90_LoadSceneWeaponSettings, "auto");
