#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

struct WeaponDefRecord {
    uint32_t data[138];
};

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(void, __fastcall, Hud_ReadFloatPair, 0x00693A20, (void* self, void* edx, char* String1, int a3));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseWeaponType, 0x00761530, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseAutomaticMode, 0x007615D0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseLoadingMode, 0x00761620, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseRangeType, 0x00761670, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseHand, 0x007616C0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseAnimationType, 0x007617B0, (char* String1));
CSF_FUNC(int, __cdecl, ParsePlayerAnimTypeName, 0x007618A0, (char* String1));
CSF_FUNC(void, __fastcall, AnimRefList_Add, 0x00771FB0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(bool, __stdcall, WeaponDefs_FinalizeEntry, 0x00783430, (int a1));
CSF_FUNC(int, __fastcall, WeaponDefs_InitializeDefaults, 0x00783EA0, (int a1, int a2));

// Explicitly describe the 552-byte by-value record as 138 stack words.
// Together with count, this callee pops 556 bytes.
CSF_FUNC(int, __fastcall, WeaponDefs_Resize, 0x00784900, (
    void* self, void* edx, int count,
    uint32_t word000, uint32_t word001, uint32_t word002, uint32_t word003,
    uint32_t word004, uint32_t word005, uint32_t word006, uint32_t word007,
    uint32_t word008, uint32_t word009, uint32_t word010, uint32_t word011,
    uint32_t word012, uint32_t word013, uint32_t word014, uint32_t word015,
    uint32_t word016, uint32_t word017, uint32_t word018, uint32_t word019,
    uint32_t word020, uint32_t word021, uint32_t word022, uint32_t word023,
    uint32_t word024, uint32_t word025, uint32_t word026, uint32_t word027,
    uint32_t word028, uint32_t word029, uint32_t word030, uint32_t word031,
    uint32_t word032, uint32_t word033, uint32_t word034, uint32_t word035,
    uint32_t word036, uint32_t word037, uint32_t word038, uint32_t word039,
    uint32_t word040, uint32_t word041, uint32_t word042, uint32_t word043,
    uint32_t word044, uint32_t word045, uint32_t word046, uint32_t word047,
    uint32_t word048, uint32_t word049, uint32_t word050, uint32_t word051,
    uint32_t word052, uint32_t word053, uint32_t word054, uint32_t word055,
    uint32_t word056, uint32_t word057, uint32_t word058, uint32_t word059,
    uint32_t word060, uint32_t word061, uint32_t word062, uint32_t word063,
    uint32_t word064, uint32_t word065, uint32_t word066, uint32_t word067,
    uint32_t word068, uint32_t word069, uint32_t word070, uint32_t word071,
    uint32_t word072, uint32_t word073, uint32_t word074, uint32_t word075,
    uint32_t word076, uint32_t word077, uint32_t word078, uint32_t word079,
    uint32_t word080, uint32_t word081, uint32_t word082, uint32_t word083,
    uint32_t word084, uint32_t word085, uint32_t word086, uint32_t word087,
    uint32_t word088, uint32_t word089, uint32_t word090, uint32_t word091,
    uint32_t word092, uint32_t word093, uint32_t word094, uint32_t word095,
    uint32_t word096, uint32_t word097, uint32_t word098, uint32_t word099,
    uint32_t word100, uint32_t word101, uint32_t word102, uint32_t word103,
    uint32_t word104, uint32_t word105, uint32_t word106, uint32_t word107,
    uint32_t word108, uint32_t word109, uint32_t word110, uint32_t word111,
    uint32_t word112, uint32_t word113, uint32_t word114, uint32_t word115,
    uint32_t word116, uint32_t word117, uint32_t word118, uint32_t word119,
    uint32_t word120, uint32_t word121, uint32_t word122, uint32_t word123,
    uint32_t word124, uint32_t word125, uint32_t word126, uint32_t word127,
    uint32_t word128, uint32_t word129, uint32_t word130, uint32_t word131,
    uint32_t word132, uint32_t word133, uint32_t word134, uint32_t word135,
    uint32_t word136, uint32_t word137));

CSF_GLOBAL(const char*, g_pszKey_LISTADATOS_8A9C74, 0x008A9C74);
CSF_GLOBAL(const char*, g_pszKey_ID_8A9C78, 0x008A9C78);
CSF_GLOBAL(const char*, g_pszKey_NOMBRE_8A9C7C, 0x008A9C7C);
CSF_GLOBAL(const char*, g_pszKey_FILE_8A9C84, 0x008A9C84);
CSF_GLOBAL(const char*, g_pszKey_TIPO_8A9C8C, 0x008A9C8C);
CSF_GLOBAL(const char*, g_pszKey_AUTOMATISMO, 0x008A9C94);
CSF_GLOBAL(const char*, g_pszKey_FOGONAZO_DFF, 0x008A9CA0);
CSF_GLOBAL(const char*, g_pszKey_FOGONAZO_COLOR, 0x008A9CA8);
CSF_GLOBAL(const char*, g_pszKey_FOGONAZO_DURACION, 0x008A9CAC);
CSF_GLOBAL(const char*, g_pszKey_FOGONAZO_RADIO, 0x008A9CB0);
CSF_GLOBAL(const char*, g_pszKey_CARGA, 0x008A9CBC);
CSF_GLOBAL(const char*, g_pszKey_ALCANCE, 0x008A9CC0);
CSF_GLOBAL(const char*, g_pszKey_ALCANCE_RANGO_MIN, 0x008A9CD0);
CSF_GLOBAL(const char*, g_pszKey_ALCANCE_RANGO_MAX, 0x008A9CD4);
CSF_GLOBAL(const char*, g_pszKey_DESVIACION_H, 0x008A9CE0);
CSF_GLOBAL(const char*, g_pszKey_DESVIACION_V, 0x008A9CE4);
CSF_GLOBAL(const char*, g_pszKey_DESVIACION_FA, 0x008A9CE8);
CSF_GLOBAL(const char*, g_pszKey_BALAS, 0x008A9CEC);
CSF_GLOBAL(const char*, g_pszKey_DAMAGE_8A9D00, 0x008A9D00);
CSF_GLOBAL(const char*, g_pszKey_DISPERSION, 0x008A9D08);
CSF_GLOBAL(const char*, g_pszKey_RADIO_8A9D0C, 0x008A9D0C);
CSF_GLOBAL(const char*, g_pszKey_CADENCIA, 0x008A9D10);
CSF_GLOBAL(const char*, g_pszKey_DECALUV1, 0x008A9D2C);
CSF_GLOBAL(const char*, g_pszKey_DECALUV2, 0x008A9D30);
CSF_GLOBAL(const char*, g_pszKey_MANO, 0x008A9D34);
CSF_GLOBAL(const char*, g_pszKey_ANIMACIONES_8A9D38, 0x008A9D38);
CSF_GLOBAL(const char*, g_pszKey_ANIMACIONES_PLAYER, 0x008A9D3C);

[[maybe_unused]] static const auto g_MenuExtrasChildCountVtable =
    CSF_VTBL(0x00692B80);

static uint8_t* WeaponDefs_GetEntryAddress(void* self, uint32_t byteOffset)
{
    void* entries = CSF_AT(void*, self, 0x08);
    return CSF_AT(uint8_t*, entries, 0x04) + byteOffset;
}

static char __fastcall WeaponDefs_LoadDefinitions(
    void* self, void* /*edx*/, S_00692b80* root)
{
    char buffer[0x10C] = {};
    uint8_t* definitions = LookupNamedElement(
        reinterpret_cast<uint8_t*>(root), nullptr, g_pszKey_LISTADATOS_8A9C74);
    uint32_t count = MenuExtras_GetChildCount(definitions, nullptr);

    void* entries = CSF_AT(void*, self, 0x08);
    WeaponDefRecord defaultEntry{};
    WeaponDefs_InitializeDefaults(reinterpret_cast<int>(&defaultEntry), 0);
    WeaponDefs_Resize(entries, nullptr, static_cast<int>(count),
        defaultEntry.data[0], defaultEntry.data[1],
        defaultEntry.data[2], defaultEntry.data[3],
        defaultEntry.data[4], defaultEntry.data[5],
        defaultEntry.data[6], defaultEntry.data[7],
        defaultEntry.data[8], defaultEntry.data[9],
        defaultEntry.data[10], defaultEntry.data[11],
        defaultEntry.data[12], defaultEntry.data[13],
        defaultEntry.data[14], defaultEntry.data[15],
        defaultEntry.data[16], defaultEntry.data[17],
        defaultEntry.data[18], defaultEntry.data[19],
        defaultEntry.data[20], defaultEntry.data[21],
        defaultEntry.data[22], defaultEntry.data[23],
        defaultEntry.data[24], defaultEntry.data[25],
        defaultEntry.data[26], defaultEntry.data[27],
        defaultEntry.data[28], defaultEntry.data[29],
        defaultEntry.data[30], defaultEntry.data[31],
        defaultEntry.data[32], defaultEntry.data[33],
        defaultEntry.data[34], defaultEntry.data[35],
        defaultEntry.data[36], defaultEntry.data[37],
        defaultEntry.data[38], defaultEntry.data[39],
        defaultEntry.data[40], defaultEntry.data[41],
        defaultEntry.data[42], defaultEntry.data[43],
        defaultEntry.data[44], defaultEntry.data[45],
        defaultEntry.data[46], defaultEntry.data[47],
        defaultEntry.data[48], defaultEntry.data[49],
        defaultEntry.data[50], defaultEntry.data[51],
        defaultEntry.data[52], defaultEntry.data[53],
        defaultEntry.data[54], defaultEntry.data[55],
        defaultEntry.data[56], defaultEntry.data[57],
        defaultEntry.data[58], defaultEntry.data[59],
        defaultEntry.data[60], defaultEntry.data[61],
        defaultEntry.data[62], defaultEntry.data[63],
        defaultEntry.data[64], defaultEntry.data[65],
        defaultEntry.data[66], defaultEntry.data[67],
        defaultEntry.data[68], defaultEntry.data[69],
        defaultEntry.data[70], defaultEntry.data[71],
        defaultEntry.data[72], defaultEntry.data[73],
        defaultEntry.data[74], defaultEntry.data[75],
        defaultEntry.data[76], defaultEntry.data[77],
        defaultEntry.data[78], defaultEntry.data[79],
        defaultEntry.data[80], defaultEntry.data[81],
        defaultEntry.data[82], defaultEntry.data[83],
        defaultEntry.data[84], defaultEntry.data[85],
        defaultEntry.data[86], defaultEntry.data[87],
        defaultEntry.data[88], defaultEntry.data[89],
        defaultEntry.data[90], defaultEntry.data[91],
        defaultEntry.data[92], defaultEntry.data[93],
        defaultEntry.data[94], defaultEntry.data[95],
        defaultEntry.data[96], defaultEntry.data[97],
        defaultEntry.data[98], defaultEntry.data[99],
        defaultEntry.data[100], defaultEntry.data[101],
        defaultEntry.data[102], defaultEntry.data[103],
        defaultEntry.data[104], defaultEntry.data[105],
        defaultEntry.data[106], defaultEntry.data[107],
        defaultEntry.data[108], defaultEntry.data[109],
        defaultEntry.data[110], defaultEntry.data[111],
        defaultEntry.data[112], defaultEntry.data[113],
        defaultEntry.data[114], defaultEntry.data[115],
        defaultEntry.data[116], defaultEntry.data[117],
        defaultEntry.data[118], defaultEntry.data[119],
        defaultEntry.data[120], defaultEntry.data[121],
        defaultEntry.data[122], defaultEntry.data[123],
        defaultEntry.data[124], defaultEntry.data[125],
        defaultEntry.data[126], defaultEntry.data[127],
        defaultEntry.data[128], defaultEntry.data[129],
        defaultEntry.data[130], defaultEntry.data[131],
        defaultEntry.data[132], defaultEntry.data[133],
        defaultEntry.data[134], defaultEntry.data[135],
        defaultEntry.data[136], defaultEntry.data[137]);

    uint32_t byteOffset = 0;
    for (uint32_t index = 0; index < count; ++index, byteOffset += 552) {
        uint8_t* component = reinterpret_cast<uint8_t*>(
            GetComponent(definitions, nullptr, index));

        uint8_t* entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(int, entry, 0x00) = ReadIntegerAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_ID_8A9C78));

        MenuExtras_ReadText(component, nullptr,
            const_cast<char*>(g_pszKey_NOMBRE_8A9C7C), buffer, 256);
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        AssignStringRange_429B60(entry + 4, nullptr, buffer,
            static_cast<unsigned int>(std::strlen(buffer)));

        MenuExtras_ReadText(component, nullptr,
            const_cast<char*>(g_pszKey_FILE_8A9C84), buffer, 256);
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        AssignStringRange_429B60(entry + 60, nullptr, buffer,
            static_cast<unsigned int>(std::strlen(buffer)));

        MenuExtras_ReadText(component, nullptr,
            const_cast<char*>(g_pszKey_TIPO_8A9C8C), buffer, 256);
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(int, entry, 0x74) = WeaponDefs_ParseWeaponType(buffer);

        MenuExtras_ReadText(component, nullptr,
            const_cast<char*>(g_pszKey_AUTOMATISMO), buffer, 256);
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(int, entry, 0x7C) = WeaponDefs_ParseAutomaticMode(buffer);

        MenuExtras_ReadText(component, nullptr,
            const_cast<char*>(g_pszKey_FOGONAZO_DFF), buffer, 256);
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        AssignStringRange_429B60(entry + 136, nullptr, buffer,
            static_cast<unsigned int>(std::strlen(buffer)));

        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(int, entry, 0xC0) = ReadIntegerAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_FOGONAZO_COLOR));

        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0xC4) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_FOGONAZO_DURACION)));

        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0xC8) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_FOGONAZO_RADIO)));

        MenuExtras_ReadText(component, nullptr,
            const_cast<char*>(g_pszKey_CARGA), buffer, 256);
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(int, entry, 0xD4) = WeaponDefs_ParseLoadingMode(buffer);

        MenuExtras_ReadText(component, nullptr,
            const_cast<char*>(g_pszKey_ALCANCE), buffer, 256);
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(int, entry, 0xD8) = WeaponDefs_ParseRangeType(buffer);

        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0xE8) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_ALCANCE_RANGO_MIN)));

        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0xEC) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_ALCANCE_RANGO_MAX)));

        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0xF8) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_DESVIACION_H)));

        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0xFC) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_DESVIACION_V)));

        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0x100) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_DESVIACION_FA)));

        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(int, entry, 0x104) = ReadIntegerAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_BALAS));

        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(int, entry, 0x118) = ReadIntegerAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_DAMAGE_8A9D00));

        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0x120) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_DISPERSION)));

        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0x124) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_RADIO_8A9D0C)));

        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0x128) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_CADENCIA)));

        CSF_AT(int, WeaponDefs_GetEntryAddress(self, byteOffset), 0x130) = -1;
        CSF_AT(int, WeaponDefs_GetEntryAddress(self, byteOffset), 0x13C) = -1;

        Hud_ReadFloatPair(component, nullptr,
            const_cast<char*>(g_pszKey_DECALUV1),
            reinterpret_cast<int>(WeaponDefs_GetEntryAddress(self, byteOffset) + 320));
        Hud_ReadFloatPair(component, nullptr,
            const_cast<char*>(g_pszKey_DECALUV2),
            reinterpret_cast<int>(WeaponDefs_GetEntryAddress(self, byteOffset) + 328));

        MenuExtras_ReadText(component, nullptr,
            const_cast<char*>(g_pszKey_MANO), buffer, 256);
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(int, entry, 0x150) = WeaponDefs_ParseHand(buffer);

        uint8_t* animations = FindProperty(component, nullptr,
            const_cast<char*>(g_pszKey_ANIMACIONES_8A9D38));
        if (animations) {
            uint32_t animationCount = MenuExtras_GetChildCount(animations, nullptr);
            for (uint32_t animationIndex = 0;
                 animationIndex < animationCount; ++animationIndex) {
                uint8_t* animation = reinterpret_cast<uint8_t*>(
                    GetComponent(animations, nullptr, animationIndex));
                uint32_t reference[3] = {
                    static_cast<uint32_t>(-1),
                    static_cast<uint32_t>(-1),
                    static_cast<uint32_t>(-1)
                };

                int id = ReadIntegerAttribute(animation, nullptr,
                    const_cast<char*>(g_pszKey_ID_8A9C78));
                reference[0] = static_cast<uint32_t>(id);
                void* animationTable = CSF_AT(void*, self, 0x04);
                reference[2] = static_cast<uint32_t>(
                    reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, animationTable, 0)[2])(
                            animationTable, nullptr, id));

                MenuExtras_ReadText(animation, nullptr,
                    const_cast<char*>(g_pszKey_TIPO_8A9C8C), buffer, 256);
                reference[1] = static_cast<uint32_t>(
                    WeaponDefs_ParseAnimationType(buffer));

                if (reference[2] != static_cast<uint32_t>(-1) &&
                    reference[0] != static_cast<uint32_t>(-1) &&
                    reference[1] != static_cast<uint32_t>(-1)) {
                    AnimRefList_Add(
                        WeaponDefs_GetEntryAddress(self, byteOffset) + 340,
                        nullptr, reference);
                }
            }
        }

        uint8_t* playerAnimations = FindProperty(component, nullptr,
            const_cast<char*>(g_pszKey_ANIMACIONES_PLAYER));
        if (playerAnimations) {
            uint32_t animationCount =
                MenuExtras_GetChildCount(playerAnimations, nullptr);
            for (uint32_t animationIndex = 0;
                 animationIndex < animationCount; ++animationIndex) {
                uint8_t* animation = reinterpret_cast<uint8_t*>(
                    GetComponent(playerAnimations, nullptr, animationIndex));
                uint32_t reference[6] = {
                    static_cast<uint32_t>(-1),
                    static_cast<uint32_t>(-1),
                    static_cast<uint32_t>(-1),
                    0, 0, 0
                };

                int id = ReadIntegerAttribute(animation, nullptr,
                    const_cast<char*>(g_pszKey_ID_8A9C78));
                reference[0] = static_cast<uint32_t>(id);
                void* animationTable = CSF_AT(void*, self, 0x04);
                reference[2] = static_cast<uint32_t>(
                    reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, animationTable, 0)[2])(
                            animationTable, nullptr, id));

                MenuExtras_ReadText(animation, nullptr,
                    const_cast<char*>(g_pszKey_TIPO_8A9C8C), buffer, 256);
                reference[1] = static_cast<uint32_t>(
                    ParsePlayerAnimTypeName(buffer));

                if (reference[2] != static_cast<uint32_t>(-1) &&
                    reference[0] != static_cast<uint32_t>(-1) &&
                    reference[1] != static_cast<uint32_t>(-1)) {
                    AnimRefList_Add(
                        WeaponDefs_GetEntryAddress(self, byteOffset) + 356,
                        nullptr, reference);
                }
            }
        }

        WeaponDefs_FinalizeEntry(
            reinterpret_cast<int>(WeaponDefs_GetEntryAddress(self, byteOffset)));
    }

    return 1;
}

HOOK(0x007849E0, WeaponDefs_LoadDefinitions, "auto");
