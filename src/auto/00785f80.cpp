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
CSF_FUNC(int, __cdecl, WeaponDefs_ParseAutomation, 0x007615D0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseReloadType, 0x00761620, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseRangeType, 0x00761670, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseHand, 0x007616C0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseAnimationType, 0x007617B0, (char* String1));
CSF_FUNC(int, __cdecl, ParsePlayerAnimTypeName, 0x007618A0, (char* String1));
CSF_FUNC(void, __fastcall, AnimRefList_Add, 0x00771FB0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(bool, __stdcall, WeaponDefs_FinalizeEntry, 0x00783430, (int a1));
CSF_FUNC(int, __fastcall, WeaponDefs_InitializeDefaults, 0x00783EA0, (int a1, int a2));

// Explicit stack slots for count followed by the 552-byte by-value record.
// The callee pops 556 bytes; self and edx remain register arguments.
CSF_FUNC(int, __fastcall, WeaponDefs_Resize, 0x00784900, (
    void* self, void* edx, int count,
    uint32_t d000, uint32_t d001, uint32_t d002, uint32_t d003,
    uint32_t d004, uint32_t d005, uint32_t d006, uint32_t d007,
    uint32_t d008, uint32_t d009, uint32_t d010, uint32_t d011,
    uint32_t d012, uint32_t d013, uint32_t d014, uint32_t d015,
    uint32_t d016, uint32_t d017, uint32_t d018, uint32_t d019,
    uint32_t d020, uint32_t d021, uint32_t d022, uint32_t d023,
    uint32_t d024, uint32_t d025, uint32_t d026, uint32_t d027,
    uint32_t d028, uint32_t d029, uint32_t d030, uint32_t d031,
    uint32_t d032, uint32_t d033, uint32_t d034, uint32_t d035,
    uint32_t d036, uint32_t d037, uint32_t d038, uint32_t d039,
    uint32_t d040, uint32_t d041, uint32_t d042, uint32_t d043,
    uint32_t d044, uint32_t d045, uint32_t d046, uint32_t d047,
    uint32_t d048, uint32_t d049, uint32_t d050, uint32_t d051,
    uint32_t d052, uint32_t d053, uint32_t d054, uint32_t d055,
    uint32_t d056, uint32_t d057, uint32_t d058, uint32_t d059,
    uint32_t d060, uint32_t d061, uint32_t d062, uint32_t d063,
    uint32_t d064, uint32_t d065, uint32_t d066, uint32_t d067,
    uint32_t d068, uint32_t d069, uint32_t d070, uint32_t d071,
    uint32_t d072, uint32_t d073, uint32_t d074, uint32_t d075,
    uint32_t d076, uint32_t d077, uint32_t d078, uint32_t d079,
    uint32_t d080, uint32_t d081, uint32_t d082, uint32_t d083,
    uint32_t d084, uint32_t d085, uint32_t d086, uint32_t d087,
    uint32_t d088, uint32_t d089, uint32_t d090, uint32_t d091,
    uint32_t d092, uint32_t d093, uint32_t d094, uint32_t d095,
    uint32_t d096, uint32_t d097, uint32_t d098, uint32_t d099,
    uint32_t d100, uint32_t d101, uint32_t d102, uint32_t d103,
    uint32_t d104, uint32_t d105, uint32_t d106, uint32_t d107,
    uint32_t d108, uint32_t d109, uint32_t d110, uint32_t d111,
    uint32_t d112, uint32_t d113, uint32_t d114, uint32_t d115,
    uint32_t d116, uint32_t d117, uint32_t d118, uint32_t d119,
    uint32_t d120, uint32_t d121, uint32_t d122, uint32_t d123,
    uint32_t d124, uint32_t d125, uint32_t d126, uint32_t d127,
    uint32_t d128, uint32_t d129, uint32_t d130, uint32_t d131,
    uint32_t d132, uint32_t d133, uint32_t d134, uint32_t d135,
    uint32_t d136, uint32_t d137));

CSF_GLOBAL(const char*, g_pszKey_LISTADATOS_8A9C74, 0x008A9C74);
CSF_GLOBAL(const char*, g_pszKey_ID_8A9C78, 0x008A9C78);
CSF_GLOBAL(const char*, g_pszKey_NOMBRE_8A9C7C, 0x008A9C7C);
CSF_GLOBAL(const char*, g_pszKey_FILE_8A9C84, 0x008A9C84);
CSF_GLOBAL(const char*, g_pszKey_FILE2, 0x008A9C88);
CSF_GLOBAL(const char*, g_pszKey_TIPO_8A9C8C, 0x008A9C8C);
CSF_GLOBAL(const char*, g_pszKey_AUTOMATISMO, 0x008A9C94);
CSF_GLOBAL(const char*, g_pszKey_FOGONAZO_DFF, 0x008A9CA0);
CSF_GLOBAL(const char*, g_pszKey_FOGONAZO_TEXTURE, 0x008A9CA4);
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
CSF_GLOBAL(const char*, g_pszKey_CARGADORES, 0x008A9CF0);
CSF_GLOBAL(const char*, g_pszKey_DAMAGE_8A9D00, 0x008A9D00);
CSF_GLOBAL(const char*, g_pszKey_DISPERSION, 0x008A9D08);
CSF_GLOBAL(const char*, g_pszKey_RADIO_8A9D0C, 0x008A9D0C);
CSF_GLOBAL(const char*, g_pszKey_CADENCIA, 0x008A9D10);
CSF_GLOBAL(const char*, g_pszKey_DECALUV1, 0x008A9D2C);
CSF_GLOBAL(const char*, g_pszKey_DECALUV2, 0x008A9D30);
CSF_GLOBAL(const char*, g_pszKey_MANO, 0x008A9D34);
CSF_GLOBAL(const char*, g_pszKey_ANIMACIONES_8A9D38, 0x008A9D38);
CSF_GLOBAL(const char*, g_pszKey_ANIMACIONES_PLAYER, 0x008A9D3C);
CSF_GLOBAL(const char*, g_pszKey_TIEMPO_DETONACION, 0x008A9D4C);
CSF_GLOBAL(const char*, g_pszKey_VELOCIDAD, 0x008A9D50);
CSF_GLOBAL(const char*, g_pszKey_ANGULO_LANZAMIENTO, 0x008A9D54);
CSF_GLOBAL(const char*, g_pszKey_ATENUACION_BOTE, 0x008A9D60);

static uint8_t* WeaponDefs_GetEntryAtOffset(void* self, uint32_t offset)
{
    return CSF_AT(uint8_t*, CSF_AT(void*, self, 0x08), 0x04) + offset;
}

static char __fastcall WeaponDefs_LoadData(void* self, void* /*edx*/, S_00692b80* source)
{
    char buffer[0x10C];
    std::memset(buffer, 0, 256);
    (void)CSF_VTBL(0x00692B80);

    uint8_t* list = LookupNamedElement(
        reinterpret_cast<uint8_t*>(source), nullptr, g_pszKey_LISTADATOS_8A9C74);
    uint32_t count = MenuExtras_GetChildCount(list, nullptr);

    void* definitions = CSF_AT(void*, self, 0x08);
    WeaponDefRecord defaults{};
    WeaponDefs_InitializeDefaults(reinterpret_cast<int>(&defaults), 0);
    WeaponDefs_Resize(definitions, nullptr, static_cast<int>(count),
        defaults.data[0], defaults.data[1], defaults.data[2], defaults.data[3],
        defaults.data[4], defaults.data[5], defaults.data[6], defaults.data[7],
        defaults.data[8], defaults.data[9], defaults.data[10], defaults.data[11],
        defaults.data[12], defaults.data[13], defaults.data[14], defaults.data[15],
        defaults.data[16], defaults.data[17], defaults.data[18], defaults.data[19],
        defaults.data[20], defaults.data[21], defaults.data[22], defaults.data[23],
        defaults.data[24], defaults.data[25], defaults.data[26], defaults.data[27],
        defaults.data[28], defaults.data[29], defaults.data[30], defaults.data[31],
        defaults.data[32], defaults.data[33], defaults.data[34], defaults.data[35],
        defaults.data[36], defaults.data[37], defaults.data[38], defaults.data[39],
        defaults.data[40], defaults.data[41], defaults.data[42], defaults.data[43],
        defaults.data[44], defaults.data[45], defaults.data[46], defaults.data[47],
        defaults.data[48], defaults.data[49], defaults.data[50], defaults.data[51],
        defaults.data[52], defaults.data[53], defaults.data[54], defaults.data[55],
        defaults.data[56], defaults.data[57], defaults.data[58], defaults.data[59],
        defaults.data[60], defaults.data[61], defaults.data[62], defaults.data[63],
        defaults.data[64], defaults.data[65], defaults.data[66], defaults.data[67],
        defaults.data[68], defaults.data[69], defaults.data[70], defaults.data[71],
        defaults.data[72], defaults.data[73], defaults.data[74], defaults.data[75],
        defaults.data[76], defaults.data[77], defaults.data[78], defaults.data[79],
        defaults.data[80], defaults.data[81], defaults.data[82], defaults.data[83],
        defaults.data[84], defaults.data[85], defaults.data[86], defaults.data[87],
        defaults.data[88], defaults.data[89], defaults.data[90], defaults.data[91],
        defaults.data[92], defaults.data[93], defaults.data[94], defaults.data[95],
        defaults.data[96], defaults.data[97], defaults.data[98], defaults.data[99],
        defaults.data[100], defaults.data[101], defaults.data[102], defaults.data[103],
        defaults.data[104], defaults.data[105], defaults.data[106], defaults.data[107],
        defaults.data[108], defaults.data[109], defaults.data[110], defaults.data[111],
        defaults.data[112], defaults.data[113], defaults.data[114], defaults.data[115],
        defaults.data[116], defaults.data[117], defaults.data[118], defaults.data[119],
        defaults.data[120], defaults.data[121], defaults.data[122], defaults.data[123],
        defaults.data[124], defaults.data[125], defaults.data[126], defaults.data[127],
        defaults.data[128], defaults.data[129], defaults.data[130], defaults.data[131],
        defaults.data[132], defaults.data[133], defaults.data[134], defaults.data[135],
        defaults.data[136], defaults.data[137]);

    uint32_t offset = 0;
    for (uint32_t index = 0; index < count; ++index, offset += 552) {
        uint8_t* component = reinterpret_cast<uint8_t*>(
            GetComponent(list, nullptr, index));
        uint8_t* entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(int, entry, 0x000) = ReadIntegerAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_ID_8A9C78));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_NOMBRE_8A9C7C), buffer, 256);
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        AssignStringRange_429B60(entry + 4, nullptr, buffer, static_cast<unsigned int>(std::strlen(buffer)));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_FILE_8A9C84), buffer, 256);
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        AssignStringRange_429B60(entry + 60, nullptr, buffer, static_cast<unsigned int>(std::strlen(buffer)));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_FILE2), buffer, 256);
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        AssignStringRange_429B60(entry + 88, nullptr, buffer, static_cast<unsigned int>(std::strlen(buffer)));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_TIPO_8A9C8C), buffer, 256);
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(int, entry, 0x074) = WeaponDefs_ParseWeaponType(buffer);

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_AUTOMATISMO), buffer, 256);
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(int, entry, 0x07C) = WeaponDefs_ParseAutomation(buffer);

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_FOGONAZO_DFF), buffer, 256);
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        AssignStringRange_429B60(entry + 136, nullptr, buffer, static_cast<unsigned int>(std::strlen(buffer)));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_FOGONAZO_TEXTURE), buffer, 256);
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        AssignStringRange_429B60(entry + 164, nullptr, buffer, static_cast<unsigned int>(std::strlen(buffer)));

        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(int, entry, 0x0C0) = ReadIntegerAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_FOGONAZO_COLOR));
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(float, entry, 0x0C4) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_FOGONAZO_DURACION)));
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(float, entry, 0x0C8) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_FOGONAZO_RADIO)));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_CARGA), buffer, 256);
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(int, entry, 0x0D4) = WeaponDefs_ParseReloadType(buffer);

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_ALCANCE), buffer, 256);
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(int, entry, 0x0D8) = WeaponDefs_ParseRangeType(buffer);

        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(float, entry, 0x0E8) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_ALCANCE_RANGO_MIN)));
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(float, entry, 0x0EC) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_ALCANCE_RANGO_MAX)));
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(float, entry, 0x0F8) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_DESVIACION_H)));
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(float, entry, 0x0FC) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_DESVIACION_V)));
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(float, entry, 0x100) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_DESVIACION_FA)));
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(int, entry, 0x104) = ReadIntegerAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_BALAS));
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(int, entry, 0x108) = ReadIntegerAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_CARGADORES));
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(int, entry, 0x118) = ReadIntegerAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_DAMAGE_8A9D00));
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(float, entry, 0x120) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_DISPERSION)));
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(float, entry, 0x124) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_RADIO_8A9D0C)));
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(float, entry, 0x128) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_CADENCIA)));
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(float, entry, 0x1A4) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_TIEMPO_DETONACION)));
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(float, entry, 0x1A8) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_VELOCIDAD)));
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(float, entry, 0x1B8) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_ATENUACION_BOTE)));
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(float, entry, 0x1AC) = static_cast<float>(ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_ANGULO_LANZAMIENTO)));

        CSF_AT(int, WeaponDefs_GetEntryAtOffset(self, offset), 0x130) = -1;
        CSF_AT(int, WeaponDefs_GetEntryAtOffset(self, offset), 0x13C) = -1;
        Hud_ReadFloatPair(component, nullptr, const_cast<char*>(g_pszKey_DECALUV1),
                          reinterpret_cast<int>(WeaponDefs_GetEntryAtOffset(self, offset) + 320));
        Hud_ReadFloatPair(component, nullptr, const_cast<char*>(g_pszKey_DECALUV2),
                          reinterpret_cast<int>(WeaponDefs_GetEntryAtOffset(self, offset) + 328));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_MANO), buffer, 256);
        entry = WeaponDefs_GetEntryAtOffset(self, offset);
        CSF_AT(int, entry, 0x150) = WeaponDefs_ParseHand(buffer);

        uint8_t* animations = FindProperty(
            component, nullptr, const_cast<char*>(g_pszKey_ANIMACIONES_8A9D38));
        if (animations) {
            uint32_t animationCount = MenuExtras_GetChildCount(animations, nullptr);
            for (uint32_t animationIndex = 0; animationIndex < animationCount; ++animationIndex) {
                uint8_t* animationNode = reinterpret_cast<uint8_t*>(
                    GetComponent(animations, nullptr, animationIndex));
                uint32_t animation[3] = {
                    static_cast<uint32_t>(-1),
                    static_cast<uint32_t>(-1),
                    static_cast<uint32_t>(-1)
                };
                int id = ReadIntegerAttribute(
                    animationNode, nullptr, const_cast<char*>(g_pszKey_ID_8A9C78));
                void* resolver = CSF_AT(void*, self, 0x04);
                animation[0] = static_cast<uint32_t>(id);
                animation[2] = static_cast<uint32_t>(
                    reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, resolver, 0)[8 / 4])(resolver, nullptr, id));
                MenuExtras_ReadText(animationNode, nullptr,
                                   const_cast<char*>(g_pszKey_TIPO_8A9C8C), buffer, 256);
                animation[1] = static_cast<uint32_t>(WeaponDefs_ParseAnimationType(buffer));
                if (animation[2] != static_cast<uint32_t>(-1) &&
                    animation[0] != static_cast<uint32_t>(-1) &&
                    animation[1] != static_cast<uint32_t>(-1)) {
                    AnimRefList_Add(WeaponDefs_GetEntryAtOffset(self, offset) + 356,
                                    nullptr, animation);
                }
            }
        }

        uint8_t* playerAnimations = FindProperty(
            component, nullptr, const_cast<char*>(g_pszKey_ANIMACIONES_PLAYER));
        if (playerAnimations) {
            uint32_t animationCount = MenuExtras_GetChildCount(playerAnimations, nullptr);
            for (uint32_t animationIndex = 0; animationIndex < animationCount; ++animationIndex) {
                uint8_t* animationNode = reinterpret_cast<uint8_t*>(
                    GetComponent(playerAnimations, nullptr, animationIndex));
                uint32_t animation[6] = {
                    static_cast<uint32_t>(-1),
                    static_cast<uint32_t>(-1),
                    static_cast<uint32_t>(-1),
                    0, 0, 0
                };
                int id = ReadIntegerAttribute(
                    animationNode, nullptr, const_cast<char*>(g_pszKey_ID_8A9C78));
                void* resolver = CSF_AT(void*, self, 0x04);
                animation[0] = static_cast<uint32_t>(id);
                animation[2] = static_cast<uint32_t>(
                    reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, resolver, 0)[8 / 4])(resolver, nullptr, id));
                MenuExtras_ReadText(animationNode, nullptr,
                                   const_cast<char*>(g_pszKey_TIPO_8A9C8C), buffer, 256);
                animation[1] = static_cast<uint32_t>(ParsePlayerAnimTypeName(buffer));
                if (animation[2] != static_cast<uint32_t>(-1) &&
                    animation[0] != static_cast<uint32_t>(-1) &&
                    animation[1] != static_cast<uint32_t>(-1)) {
                    // Both groups append to the same animation list.
                    AnimRefList_Add(WeaponDefs_GetEntryAtOffset(self, offset) + 356,
                                    nullptr, animation);
                }
            }
        }

        WeaponDefs_FinalizeEntry(
            reinterpret_cast<int>(WeaponDefs_GetEntryAtOffset(self, offset)));
    }
    return 1;
}

HOOK(0x00785F80, WeaponDefs_LoadData, "auto");
