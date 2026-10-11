#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __cdecl, IsStateCodeAllowed, 0x00755810, (unsigned int a1));
CSF_FUNC(int, __cdecl, IsResolvedEntryActive, 0x00755880, (int a1));
CSF_FUNC(int, __cdecl, ParseSoldierAnimTypeName, 0x00761800, (char* String1));
CSF_FUNC(int, __cdecl, ParsePlayerAnimTypeName, 0x007618A0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseMultiplayerAnimationType, 0x007618F0, (char* String1));
CSF_FUNC(int, __cdecl, CharacterDefs_ParseResolvedEntryAnimationType, 0x00761940, (char* String1));
CSF_FUNC(int, __cdecl, CharacterDefs_ParseSpecialStateAnimationType, 0x00761990, (char* String1));
CSF_FUNC(void, __fastcall, AnimRefList_Add, 0x00771FB0, (void* self, void* edx, uint32_t* a2));

CSF_GLOBAL(const char*, g_pszKey_ID_8A9808, 0x008A9808);
CSF_GLOBAL(const char*, g_pszKey_TIPO_8A9820, 0x008A9820);

static char __fastcall C_0086f128_ParseAnimationReferences(
    C_0086f128* self, void* /*edx*/, S_00692b80* node,
    int characterAddress, S_00771b80* animationReferences)
{
    // Packet-listed vtable reference; the instruction actually calls the getter.
    (void)CSF_VTBL(0x00692B80);

    if (!node)
        return 1;

    char buffer[0x10C];
    std::memset(buffer, 0, 256);

    void* character = reinterpret_cast<void*>(characterAddress);
    const std::uint32_t childCount = MenuExtras_GetChildCount(node, nullptr);
    for (std::uint32_t index = 0; index < childCount; ++index)
    {
        auto* component = reinterpret_cast<std::uint8_t*>(
            GetComponent(reinterpret_cast<unsigned char*>(node), nullptr, index));

        std::uint32_t reference[3] = {
            static_cast<std::uint32_t>(-1),
            static_cast<std::uint32_t>(-1),
            static_cast<std::uint32_t>(-1)
        };

        const int id = ReadIntegerAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_ID_8A9808));
        void* resolver = reinterpret_cast<void*>(CSF_AT(std::uint32_t, self, 0x04));
        reference[0] = static_cast<std::uint32_t>(id);
        reference[2] = static_cast<std::uint32_t>(
            ((int(__fastcall*)(void*, void*, int))
                CSF_AT(void**, resolver, 0)[8 / 4])(resolver, nullptr, id));

        if (CSF_AT(int, character, 0xB4) != 0)
        {
            if (static_cast<std::uint8_t>(IsStateCodeAllowed(
                    CSF_AT(unsigned int, character, 0xB4))) != 0)
            {
                MenuExtras_ReadText(component, nullptr,
                    const_cast<char*>(g_pszKey_TIPO_8A9820), buffer, 256);
                reference[1] = static_cast<std::uint32_t>(
                    ParseSoldierAnimTypeName(buffer));
            }
            else if (static_cast<std::uint8_t>(IsResolvedEntryActive(
                         CSF_AT(int, character, 0xB4))) != 0)
            {
                MenuExtras_ReadText(component, nullptr,
                    const_cast<char*>(g_pszKey_TIPO_8A9820), buffer, 256);
                reference[1] = static_cast<std::uint32_t>(
                    CharacterDefs_ParseResolvedEntryAnimationType(buffer));
            }
            else
            {
                const int stateCode = CSF_AT(int, character, 0xB4);
                if (stateCode == 12 || stateCode == 34)
                {
                    MenuExtras_ReadText(component, nullptr,
                        const_cast<char*>(g_pszKey_TIPO_8A9820), buffer, 256);
                    reference[1] = static_cast<std::uint32_t>(
                        CharacterDefs_ParseSpecialStateAnimationType(buffer));
                }
            }
        }
        else
        {
            switch (CSF_AT(int, character, 0xB8))
            {
            case 1:
            case 2:
            case 3:
                MenuExtras_ReadText(component, nullptr,
                    const_cast<char*>(g_pszKey_TIPO_8A9820), buffer, 256);
                reference[1] = static_cast<std::uint32_t>(
                    ParsePlayerAnimTypeName(buffer));
                break;
            case 4:
            case 5:
            case 6:
                MenuExtras_ReadText(component, nullptr,
                    const_cast<char*>(g_pszKey_TIPO_8A9820), buffer, 256);
                reference[1] = static_cast<std::uint32_t>(
                    WeaponDefs_ParseMultiplayerAnimationType(buffer));
                break;
            default:
                break;
            }
        }

        if (reference[2] != static_cast<std::uint32_t>(-1) &&
            reference[0] != static_cast<std::uint32_t>(-1) &&
            reference[1] != static_cast<std::uint32_t>(-1))
        {
            AnimRefList_Add(animationReferences, nullptr, reference);
        }
    }
    return 1;
}

HOOK(0x00761FD0, C_0086f128_ParseAnimationReferences, "auto");
