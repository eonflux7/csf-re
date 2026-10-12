#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082d410_AssignResourceString, 0x00429C60, (void* self, void* , const char* text));
CSF_FUNC(int, __cdecl, Hud_IsStringEqualIgnoreCase, 0x0042D5F0, (char* String1, char* String2));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0, (std::uint32_t component));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40, (std::uint8_t* attribute, void* edx, char* buffer, int capacity));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseAnimationType, 0x007617B0, (char* String1));
CSF_FUNC(int, __cdecl, CharacterDefs_ParseAlliedFactionIndex, 0x00761B20, (char* String1));
CSF_FUNC(int, __cdecl, CharacterDefs_ParseDePieEntryIndex, 0x00761BC0, (char* String1));
CSF_FUNC(int, __cdecl, CharacterDefs_FindDistraidoIndex, 0x00761C10, (char* String1));
CSF_FUNC(int, __cdecl, CharacterDefs_FindDistraccionIndex, 0x00761C60, (char* String1));
CSF_FUNC(int, __cdecl, CharacterDefs_FindNoRechargeMessageIndex, 0x00761CB0, (char* String1));
CSF_FUNC(int, __cdecl, CharacterDefs_ParseAndarAnimationType, 0x00761D00, (char* String1));
CSF_FUNC(int, __cdecl, CharacterDefs_FindAppConfigNameSlotIndex, 0x00761DA0, (char* name));
CSF_FUNC(int, __cdecl, CharacterDefs_FindAppConfigIndex, 0x00761DF0, (char* String1));
CSF_FUNC(int, __cdecl, CharacterDefs_FindStaticConfigIndex, 0x00761E40, (char* String1));
CSF_FUNC(int, __cdecl, CharacterDefs_ParseStaticAnimationType, 0x00761E90, (char* String1));
CSF_FUNC(int, __cdecl, CharacterDefs_FindAppConfigEntryIndex, 0x00761EE0, (char* String1));

static void MissionScript_CopyFirstComponentText(S_00692b80* source, char* text)
{
    const std::uint32_t component =
        GetComponent(reinterpret_cast<unsigned char*>(source), nullptr, 1u);
    CopyAttributeText(reinterpret_cast<std::uint8_t*>(component), nullptr, text, 256);
}

static void __fastcall S_00692b80_CopyComponent(
    S_00692b80* self, void* /*edx*/, S_00692b80* source)
{
    alignas(8) std::uint8_t textBytes[0x114];
    char* text = reinterpret_cast<char*>(textBytes);
    std::memset(text, 0, 256);

    std::uint8_t* destination = CSF_AT(std::uint8_t*, self, 0x4);
    switch (CSF_AT(std::uint8_t, destination, 0x0))
    {
    case 4:
    {
        const auto component =
            GetComponent(reinterpret_cast<unsigned char*>(source), nullptr, 1u);
        const int value = ParseIntegerComponent(component);
        CSF_AT(int, destination, 0x10) = value;
        break;
    }

    case 5:
    {
        const auto first =
            GetComponent(reinterpret_cast<unsigned char*>(source), nullptr, 1u);
        const int firstValue = ParseIntegerComponent(first);
        CSF_AT(int, destination, 0x10) = firstValue;

        destination = CSF_AT(std::uint8_t*, self, 0x4);
        const auto second =
            GetComponent(reinterpret_cast<unsigned char*>(source), nullptr, 2u);
        const int secondValue = ParseIntegerComponent(second);
        CSF_AT(int, destination, 0x14) = secondValue;
        break;
    }

    case 6:
    case 7:
    case 0xA:
    case 0xC:
    case 0xD:
    case 0xE:
    case 0x11:
    case 0x20:
    case 0x24:
    case 0x26:
    case 0x27:
    case 0x28:
    case 0x29:
    case 0x2F:
    case 0x30:
    case 0x31:
    case 0x32:
    {
        const auto component =
            GetComponent(reinterpret_cast<unsigned char*>(source), nullptr, 1u);
        const int value = ParseIntegerComponent(component);
        CSF_AT(int, destination, 0x4) = value;
        break;
    }

    case 0xB:
    case 0x10:
    case 0x12:
    case 0x17:
    case 0x25:
        MissionScript_CopyFirstComponentText(source, text);
        C_0082d410_AssignResourceString(
            CSF_AT(std::uint8_t*, self, 0x4) + 0x4, nullptr, text);
        break;

    case 0x14:
    {
        const auto second =
            GetComponent(reinterpret_cast<unsigned char*>(source), nullptr, 2u);
        const auto first =
            GetComponent(reinterpret_cast<unsigned char*>(source), nullptr, 1u);
        const auto third =
            GetComponent(reinterpret_cast<unsigned char*>(source), nullptr, 3u);

        const float thirdValue = static_cast<float>(ReadComponentFloat(third));
        const float secondValue = static_cast<float>(ReadComponentFloat(second));
        const float firstValue = static_cast<float>(ReadComponentFloat(first));

        destination = CSF_AT(std::uint8_t*, self, 0x4);
        CSF_AT(float, destination, 0x4) = firstValue;
        CSF_AT(float, destination, 0x8) = secondValue;
        CSF_AT(float, destination, 0xC) = thirdValue;
        break;
    }

    case 0x15:
        MissionScript_CopyFirstComponentText(source, text);
        if (static_cast<std::uint8_t>(Hud_IsStringEqualIgnoreCase(
                text, const_cast<char*>(CSF_STR(0x00831610, "TRUE")))))
        {
            CSF_AT(std::uint8_t, CSF_AT(std::uint8_t*, self, 0x4), 0x1) = 1;
        }
        else if (static_cast<std::uint8_t>(Hud_IsStringEqualIgnoreCase(
                     text, const_cast<char*>(CSF_STR(0x00831608, "FALSE")))))
        {
            CSF_AT(std::uint8_t, CSF_AT(std::uint8_t*, self, 0x4), 0x1) = 0;
        }
        break;

    case 0x16:
    {
        const auto component =
            GetComponent(reinterpret_cast<unsigned char*>(source), nullptr, 1u);
        const float value = static_cast<float>(ReadComponentFloat(component));
        CSF_AT(float, destination, 0x4) = value;
        break;
    }

    case 0x18:
    {
        MissionScript_CopyFirstComponentText(source, text);
        destination = CSF_AT(std::uint8_t*, self, 0x4);
        const int value = WeaponDefs_ParseAnimationType(text);
        CSF_AT(int, destination, 0x4) = value;
        break;
    }

    case 0x19:
    {
        MissionScript_CopyFirstComponentText(source, text);
        const int value = CharacterDefs_ParseDePieEntryIndex(text);
        CSF_AT(int, CSF_AT(std::uint8_t*, self, 0x4), 0x4) = value;
        break;
    }

    case 0x1A:
    {
        MissionScript_CopyFirstComponentText(source, text);
        const int value = CharacterDefs_FindDistraidoIndex(text);
        CSF_AT(int, CSF_AT(std::uint8_t*, self, 0x4), 0x4) = value;
        break;
    }

    case 0x1B:
    {
        MissionScript_CopyFirstComponentText(source, text);
        const int value = CharacterDefs_FindDistraccionIndex(text);
        CSF_AT(int, CSF_AT(std::uint8_t*, self, 0x4), 0x4) = value;
        break;
    }

    case 0x1D:
    {
        MissionScript_CopyFirstComponentText(source, text);
        const int value = CharacterDefs_FindNoRechargeMessageIndex(text);
        CSF_AT(int, CSF_AT(std::uint8_t*, self, 0x4), 0x4) = value;
        break;
    }

    case 0x1E:
    {
        MissionScript_CopyFirstComponentText(source, text);
        const int value = CharacterDefs_ParseAlliedFactionIndex(text);
        CSF_AT(int, CSF_AT(std::uint8_t*, self, 0x4), 0x4) = value;
        break;
    }

    case 0x1F:
    {
        MissionScript_CopyFirstComponentText(source, text);
        const int value = CharacterDefs_ParseAndarAnimationType(text);
        CSF_AT(int, CSF_AT(std::uint8_t*, self, 0x4), 0x4) = value;
        break;
    }

    case 0x21:
    case 0x22:
    case 0x23:
    {
        const auto component =
            GetComponent(reinterpret_cast<unsigned char*>(source), nullptr, 1u);
        const int value = ParseIntegerComponent(component);
        CSF_AT(std::uint8_t, destination, 0x1) = static_cast<std::uint8_t>(value);
        break;
    }

    case 0x2A:
    {
        MissionScript_CopyFirstComponentText(source, text);
        destination = CSF_AT(std::uint8_t*, self, 0x4);
        const int value = CharacterDefs_FindAppConfigNameSlotIndex(text);
        CSF_AT(int, destination, 0x4) = value;
        break;
    }

    case 0x2B:
    {
        MissionScript_CopyFirstComponentText(source, text);
        destination = CSF_AT(std::uint8_t*, self, 0x4);
        const int value = CharacterDefs_FindAppConfigIndex(text);
        CSF_AT(int, destination, 0x4) = value;
        break;
    }

    case 0x2C:
    {
        MissionScript_CopyFirstComponentText(source, text);
        destination = CSF_AT(std::uint8_t*, self, 0x4);
        const int value = CharacterDefs_FindStaticConfigIndex(text);
        CSF_AT(int, destination, 0x4) = value;
        break;
    }

    case 0x2D:
    {
        MissionScript_CopyFirstComponentText(source, text);
        destination = CSF_AT(std::uint8_t*, self, 0x4);
        const int value = CharacterDefs_ParseStaticAnimationType(text);
        CSF_AT(int, destination, 0x4) = value;
        break;
    }

    case 0x2E:
    {
        MissionScript_CopyFirstComponentText(source, text);
        destination = CSF_AT(std::uint8_t*, self, 0x4);
        const int value = CharacterDefs_FindAppConfigEntryIndex(text);
        CSF_AT(int, destination, 0x4) = value;
        break;
    }

    default:
        return;
    }
}

HOOK(0x004734F0, S_00692b80_CopyComponent, "auto");
