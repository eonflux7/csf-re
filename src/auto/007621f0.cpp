#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __cdecl, IsStateCodeAllowed, 0x00755810, (unsigned int a1));
CSF_FUNC(int, __cdecl, IsResolvedEntryActive, 0x00755880, (int a1));
CSF_FUNC(int, __cdecl, ParseSoldierAnimTypeName, 0x00761800, (char* String1));
CSF_FUNC(int, __cdecl, ParsePlayerAnimTypeName, 0x007618A0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseMultiplayerAnimationType, 0x007618F0, (char* String1));
CSF_FUNC(int, __cdecl, CharacterDefs_ParseActiveEntryAnimTypeName, 0x00761940, (char* String1));
CSF_FUNC(int, __cdecl, CharacterDefs_ParseSpecialStateAnimTypeName, 0x00761990, (char* String1));
CSF_FUNC(void, __fastcall, AnimRefList_Add, 0x00771FB0, (void* self, void* edx, uint32_t* a2));

CSF_GLOBAL(char*, g_pszKey_ID_8A9808, 0x008A9808);
CSF_GLOBAL(char*, g_pszKey_TIPO_8A9820, 0x008A9820);

// The packet also identifies the child-count callee address as a vtable.
[[maybe_unused]] static const auto kChildNodeVtable = CSF_VTBL(0x00692B80);

static char __fastcall C_0086f128_ReadAnimationReferences(
    C_0086f128* self, void* /*edx*/, S_00692b80* node,
    int characterDefinition, S_00771b80* references, char* variantName)
{
    if (!node)
        return 1;

    char typeName[0x10C];
    std::memset(typeName, 0, 256);

    const std::uint32_t childCount = MenuExtras_GetChildCount(node, nullptr);
    for (std::uint32_t index = 0; index < childCount; ++index)
    {
        auto* component = reinterpret_cast<std::uint8_t*>(
            GetComponent(reinterpret_cast<unsigned char*>(node), nullptr, index));

        int reference[3] = {-1, -1, -1};
        auto* property = FindProperty(component, nullptr, g_pszKey_ID_8A9808);
        auto* idNode = property
            ? FindProperty(property, nullptr, variantName)
            : nullptr;
        if (!idNode)
            idNode = component;

        reference[0] = ReadIntegerAttribute(idNode, nullptr, g_pszKey_ID_8A9808);
        if (reference[0] == -1)
            continue;

        void* resolver = reinterpret_cast<void*>(CSF_AT(std::uint32_t, self, 0x4));
        reference[2] =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, resolver, 0)[2])(resolver, nullptr, reference[0]);

        auto* definition = reinterpret_cast<void*>(characterDefinition);
        if (CSF_AT(int, definition, 0xB4) == 0)
        {
            switch (CSF_AT(int, definition, 0xB8))
            {
            case 1:
            case 2:
            case 3:
                MenuExtras_ReadText(component, nullptr, g_pszKey_TIPO_8A9820,
                                    typeName, 256);
                reference[1] = ParsePlayerAnimTypeName(typeName);
                break;
            case 4:
            case 5:
            case 6:
                MenuExtras_ReadText(component, nullptr, g_pszKey_TIPO_8A9820,
                                    typeName, 256);
                reference[1] = WeaponDefs_ParseMultiplayerAnimationType(typeName);
                break;
            default:
                break;
            }
        }
        else if (static_cast<std::uint8_t>(
                     IsStateCodeAllowed(CSF_AT(unsigned int, definition, 0xB4))))
        {
            MenuExtras_ReadText(component, nullptr, g_pszKey_TIPO_8A9820,
                                typeName, 256);
            reference[1] = ParseSoldierAnimTypeName(typeName);
        }
        else if (static_cast<std::uint8_t>(
                     IsResolvedEntryActive(CSF_AT(int, definition, 0xB4))))
        {
            MenuExtras_ReadText(component, nullptr, g_pszKey_TIPO_8A9820,
                                typeName, 256);
            reference[1] = CharacterDefs_ParseActiveEntryAnimTypeName(typeName);
        }
        else
        {
            const int stateCode = CSF_AT(int, definition, 0xB4);
            if (stateCode == 31)
            {
                MenuExtras_ReadText(component, nullptr, g_pszKey_TIPO_8A9820,
                                    typeName, 256);
                reference[1] = WeaponDefs_ParseMultiplayerAnimationType(typeName);
            }
            else if (stateCode == 12 || stateCode == 34)
            {
                MenuExtras_ReadText(component, nullptr, g_pszKey_TIPO_8A9820,
                                    typeName, 256);
                reference[1] = CharacterDefs_ParseSpecialStateAnimTypeName(typeName);
            }
        }

        if (reference[2] != -1 && reference[0] != -1 && reference[1] != -1)
            AnimRefList_Add(references, nullptr,
                            reinterpret_cast<std::uint32_t*>(reference));
    }

    return 1;
}

HOOK(0x007621F0, C_0086f128_ReadAnimationReferences, "auto");
