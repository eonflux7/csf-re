#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082d410_AssignWholeString, 0x00433FA0, (C_0082d410* self, void* , int sourceValue));
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

static char* MissionScript_GetEntryString(void* source)
{
    if (CSF_AT(std::uint32_t, source, 0x30) < 0x10u)
        return reinterpret_cast<char*>(source) + 0x1C;
    return CSF_AT(char*, source, 0x1C);
}

static std::uint32_t* __fastcall MissionScript_CopyEntryToValue(
    std::uint32_t* self, void* /*edx*/, int sourceValue)
{
    auto* source = reinterpret_cast<std::uint8_t*>(sourceValue);
    auto* value = CSF_AT(std::uint8_t*, self, 0x4);

    switch (CSF_AT(std::uint8_t, value, 0x0))
    {
    case 4:
        CSF_AT(std::uint32_t, value, 0x10) =
            static_cast<std::uint32_t>(static_cast<std::int64_t>(
                CSF_AT(float, source, 0x10)));
        break;

    case 5:
        CSF_AT(std::uint32_t, CSF_AT(std::uint8_t*, self, 0x4), 0x10) =
            static_cast<std::uint32_t>(static_cast<std::int64_t>(
                CSF_AT(float, source, 0x38)));
        CSF_AT(std::uint32_t, CSF_AT(std::uint8_t*, self, 0x4), 0x14) =
            static_cast<std::uint32_t>(static_cast<std::int64_t>(
                CSF_AT(float, source, 0x3C)));
        break;

    case 6:
    case 0x24:
    case 0x31:
    case 7:
    case 0xA:
    case 0x30:
    case 0xE:
    case 0x26:
    case 0x32:
        CSF_AT(std::uint32_t, value, 0x4) =
            static_cast<std::uint32_t>(static_cast<std::int64_t>(
                CSF_AT(float, source, 0x10)));
        break;

    case 0xB:
    case 0x17:
        C_0082d410_AssignWholeString(
            reinterpret_cast<C_0082d410*>(value + 0x4), nullptr,
            reinterpret_cast<int>(source + 0x18));
        break;

    case 0x14:
        CSF_AT(std::uint32_t, value, 0x4) = CSF_AT(std::uint32_t, source, 0x38);
        CSF_AT(std::uint32_t, value, 0x8) = CSF_AT(std::uint32_t, source, 0x3C);
        CSF_AT(std::uint32_t, value, 0xC) = CSF_AT(std::uint32_t, source, 0x40);
        break;

    case 0x15:
        CSF_AT(std::uint8_t, value, 0x1) = CSF_AT(std::uint8_t, source, 0xA);
        break;

    case 0x16:
        CSF_AT(std::uint32_t, value, 0x4) = CSF_AT(std::uint32_t, source, 0x10);
        break;

    case 0x18:
    {
        const int index = WeaponDefs_ParseAnimationType(MissionScript_GetEntryString(source));
        CSF_AT(int, CSF_AT(std::uint8_t*, self, 0x4), 0x4) = index;
        break;
    }
    case 0x19:
    {
        const int index = CharacterDefs_ParseDePieEntryIndex(MissionScript_GetEntryString(source));
        CSF_AT(int, CSF_AT(std::uint8_t*, self, 0x4), 0x4) = index;
        break;
    }
    case 0x1A:
    {
        const int index = CharacterDefs_FindDistraidoIndex(MissionScript_GetEntryString(source));
        CSF_AT(int, CSF_AT(std::uint8_t*, self, 0x4), 0x4) = index;
        break;
    }
    case 0x1B:
    {
        const int index = CharacterDefs_FindDistraccionIndex(MissionScript_GetEntryString(source));
        CSF_AT(int, CSF_AT(std::uint8_t*, self, 0x4), 0x4) = index;
        break;
    }
    case 0x1D:
    {
        const int index = CharacterDefs_FindNoRechargeMessageIndex(MissionScript_GetEntryString(source));
        CSF_AT(int, CSF_AT(std::uint8_t*, self, 0x4), 0x4) = index;
        break;
    }
    case 0x1E:
    {
        const int index = CharacterDefs_ParseAlliedFactionIndex(MissionScript_GetEntryString(source));
        CSF_AT(int, CSF_AT(std::uint8_t*, self, 0x4), 0x4) = index;
        break;
    }
    case 0x1F:
    {
        const int index = CharacterDefs_ParseAndarAnimationType(MissionScript_GetEntryString(source));
        CSF_AT(int, CSF_AT(std::uint8_t*, self, 0x4), 0x4) = index;
        [[fallthrough]];
    }
    case 0xC:
        CSF_AT(std::uint32_t, CSF_AT(std::uint8_t*, self, 0x4), 0x4) =
            static_cast<std::uint32_t>(static_cast<std::int64_t>(
                CSF_AT(float, source, 0x10)));
        break;

    case 0x21:
    case 0x22:
    case 0x23:
        CSF_AT(std::uint8_t, value, 0x1) =
            static_cast<std::uint8_t>(static_cast<std::int64_t>(
                CSF_AT(float, source, 0x10)));
        break;

    case 0x2A:
        CSF_AT(int, value, 0x4) =
            CharacterDefs_FindAppConfigNameSlotIndex(MissionScript_GetEntryString(source));
        break;

    case 0x2B:
        CSF_AT(int, value, 0x4) =
            CharacterDefs_FindAppConfigIndex(MissionScript_GetEntryString(source));
        break;

    case 0x2C:
        CSF_AT(int, value, 0x4) =
            CharacterDefs_FindStaticConfigIndex(MissionScript_GetEntryString(source));
        break;

    case 0x2D:
        CSF_AT(int, value, 0x4) =
            CharacterDefs_ParseStaticAnimationType(MissionScript_GetEntryString(source));
        break;

    case 0x2E:
        CSF_AT(int, value, 0x4) =
            CharacterDefs_FindAppConfigEntryIndex(MissionScript_GetEntryString(source));
        break;

    default:
        break;
    }

    return self;
}

HOOK(0x00472D50, MissionScript_CopyEntryToValue, "auto");
