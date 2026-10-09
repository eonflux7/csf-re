#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

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
CSF_FUNC(int, __cdecl, WeaponDefs_ParseFireMode, 0x007615D0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseRangeType, 0x00761670, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseHand, 0x007616C0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseAnimationType, 0x007617B0, (char* String1));
CSF_FUNC(int, __cdecl, ParsePlayerAnimTypeName, 0x007618A0, (char* String1));
CSF_FUNC(void, __fastcall, AnimRefList_Add, 0x00771FB0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(bool, __stdcall, WeaponDefs_FinalizeEntry, 0x00783430, (int a1));
CSF_FUNC(int, __fastcall, WeaponDefs_InitializeDefaults, 0x00783EA0, (int a1, int a2));

// Expand the 552-byte by-value record into stack words:
// count plus 138 words gives the original ret 556 ABI.
CSF_FUNC(int, __fastcall, WeaponDefs_Resize, 0x00784900, (
    void* self, void* edx, int count,
    uint32_t word0, uint32_t word1, uint32_t word2, uint32_t word3,
    uint32_t word4, uint32_t word5, uint32_t word6, uint32_t word7,
    uint32_t word8, uint32_t word9, uint32_t word10, uint32_t word11,
    uint32_t word12, uint32_t word13, uint32_t word14, uint32_t word15,
    uint32_t word16, uint32_t word17, uint32_t word18, uint32_t word19,
    uint32_t word20, uint32_t word21, uint32_t word22, uint32_t word23,
    uint32_t word24, uint32_t word25, uint32_t word26, uint32_t word27,
    uint32_t word28, uint32_t word29, uint32_t word30, uint32_t word31,
    uint32_t word32, uint32_t word33, uint32_t word34, uint32_t word35,
    uint32_t word36, uint32_t word37, uint32_t word38, uint32_t word39,
    uint32_t word40, uint32_t word41, uint32_t word42, uint32_t word43,
    uint32_t word44, uint32_t word45, uint32_t word46, uint32_t word47,
    uint32_t word48, uint32_t word49, uint32_t word50, uint32_t word51,
    uint32_t word52, uint32_t word53, uint32_t word54, uint32_t word55,
    uint32_t word56, uint32_t word57, uint32_t word58, uint32_t word59,
    uint32_t word60, uint32_t word61, uint32_t word62, uint32_t word63,
    uint32_t word64, uint32_t word65, uint32_t word66, uint32_t word67,
    uint32_t word68, uint32_t word69, uint32_t word70, uint32_t word71,
    uint32_t word72, uint32_t word73, uint32_t word74, uint32_t word75,
    uint32_t word76, uint32_t word77, uint32_t word78, uint32_t word79,
    uint32_t word80, uint32_t word81, uint32_t word82, uint32_t word83,
    uint32_t word84, uint32_t word85, uint32_t word86, uint32_t word87,
    uint32_t word88, uint32_t word89, uint32_t word90, uint32_t word91,
    uint32_t word92, uint32_t word93, uint32_t word94, uint32_t word95,
    uint32_t word96, uint32_t word97, uint32_t word98, uint32_t word99,
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
CSF_GLOBAL(const char*, g_pszKey_FILE2, 0x008A9C88);
CSF_GLOBAL(const char*, g_pszKey_TIPO_8A9C8C, 0x008A9C8C);
CSF_GLOBAL(const char*, g_pszKey_AUTOMATISMO, 0x008A9C94);
CSF_GLOBAL(const char*, g_pszKey_FOGONAZO_DFF, 0x008A9CA0);
CSF_GLOBAL(const char*, g_pszKey_FOGONAZO_COLOR, 0x008A9CA8);
CSF_GLOBAL(const char*, g_pszKey_FOGONAZO_DURACION, 0x008A9CAC);
CSF_GLOBAL(const char*, g_pszKey_FOGONAZO_RADIO, 0x008A9CB0);
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
CSF_GLOBAL(const char*, g_pszKey_TIEMPO_DETONACION, 0x008A9D4C);
CSF_GLOBAL(const char*, g_pszKey_VELOCIDAD, 0x008A9D50);
CSF_GLOBAL(const char*, g_pszKey_ANGULO_LANZAMIENTO, 0x008A9D54);
CSF_GLOBAL(const char*, g_pszKey_ATENUACION_BOTE, 0x008A9D60);

static uint8_t* WeaponDefs_GetEntry(void* self, std::size_t byteOffset)
{
    return CSF_AT(uint8_t*, CSF_AT(void*, self, 0x8), 0x4) + byteOffset;
}

static char __fastcall WeaponDefs_LoadData(void* self, void* /*edx*/, uint8_t* root)
{
    (void)CSF_VTBL(0x00692B80);

    char buffer[0x10C];
    std::memset(buffer, 0, 256);

    uint8_t* dataList = LookupNamedElement(root, nullptr, g_pszKey_LISTADATOS_8A9C74);
    const uint32_t count = MenuExtras_GetChildCount(dataList, nullptr);
    void* entries = CSF_AT(void*, self, 0x8);
    WeaponDefRecord defaults{};
    WeaponDefs_InitializeDefaults(reinterpret_cast<int>(&defaults), 0);
    WeaponDefs_Resize(entries, nullptr, static_cast<int>(count),
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

    std::size_t byteOffset = 0;
    for (uint32_t index = 0; index < count; ++index, byteOffset += 552) {
        uint8_t* component = reinterpret_cast<uint8_t*>(GetComponent(dataList, nullptr, index));
        uint8_t* entry = WeaponDefs_GetEntry(self, byteOffset);
        CSF_AT(int, entry, 0x0) =
            ReadIntegerAttribute(component, nullptr, const_cast<char*>(g_pszKey_ID_8A9C78));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_NOMBRE_8A9C7C), buffer, 256);
        entry = WeaponDefs_GetEntry(self, byteOffset);
        AssignStringRange_429B60(entry + 4, nullptr, buffer, static_cast<unsigned int>(std::strlen(buffer)));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_FILE_8A9C84), buffer, 256);
        entry = WeaponDefs_GetEntry(self, byteOffset);
        AssignStringRange_429B60(entry + 60, nullptr, buffer, static_cast<unsigned int>(std::strlen(buffer)));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_FILE2), buffer, 256);
        entry = WeaponDefs_GetEntry(self, byteOffset);
        AssignStringRange_429B60(entry + 88, nullptr, buffer, static_cast<unsigned int>(std::strlen(buffer)));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_TIPO_8A9C8C), buffer, 256);
        entry = WeaponDefs_GetEntry(self, byteOffset);
        CSF_AT(int, entry, 0x74) = WeaponDefs_ParseWeaponType(buffer);

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_AUTOMATISMO), buffer, 256);
        entry = WeaponDefs_GetEntry(self, byteOffset);
        CSF_AT(int, entry, 0x7C) = WeaponDefs_ParseFireMode(buffer);

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_FOGONAZO_DFF), buffer, 256);
        entry = WeaponDefs_GetEntry(self, byteOffset);
        AssignStringRange_429B60(entry + 136, nullptr, buffer, static_cast<unsigned int>(std::strlen(buffer)));

        entry = WeaponDefs_GetEntry(self, byteOffset);
        CSF_AT(int, entry, 0xC0) =
            ReadIntegerAttribute(component, nullptr, const_cast<char*>(g_pszKey_FOGONAZO_COLOR));
        entry = WeaponDefs_GetEntry(self, byteOffset);
        CSF_AT(float, entry, 0xC4) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_FOGONAZO_DURACION)));
        entry = WeaponDefs_GetEntry(self, byteOffset);
        CSF_AT(float, entry, 0xC8) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_FOGONAZO_RADIO)));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_ALCANCE), buffer, 256);
        entry = WeaponDefs_GetEntry(self, byteOffset);
        CSF_AT(int, entry, 0xD8) = WeaponDefs_ParseRangeType(buffer);

        entry = WeaponDefs_GetEntry(self, byteOffset);
        CSF_AT(float, entry, 0xE8) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_ALCANCE_RANGO_MIN)));
        entry = WeaponDefs_GetEntry(self, byteOffset);
        CSF_AT(float, entry, 0xEC) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_ALCANCE_RANGO_MAX)));
        entry = WeaponDefs_GetEntry(self, byteOffset);
        CSF_AT(float, entry, 0xF8) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_DESVIACION_H)));
        entry = WeaponDefs_GetEntry(self, byteOffset);
        CSF_AT(float, entry, 0xFC) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_DESVIACION_V)));
        entry = WeaponDefs_GetEntry(self, byteOffset);
        CSF_AT(float, entry, 0x100) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_DESVIACION_FA)));
        entry = WeaponDefs_GetEntry(self, byteOffset);
        CSF_AT(int, entry, 0x104) =
            ReadIntegerAttribute(component, nullptr, const_cast<char*>(g_pszKey_BALAS));
        entry = WeaponDefs_GetEntry(self, byteOffset);
        CSF_AT(int, entry, 0x118) =
            ReadIntegerAttribute(component, nullptr, const_cast<char*>(g_pszKey_DAMAGE_8A9D00));
        entry = WeaponDefs_GetEntry(self, byteOffset);
        CSF_AT(float, entry, 0x120) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_DISPERSION)));
        entry = WeaponDefs_GetEntry(self, byteOffset);
        CSF_AT(float, entry, 0x124) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_RADIO_8A9D0C)));
        entry = WeaponDefs_GetEntry(self, byteOffset);
        CSF_AT(float, entry, 0x128) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_CADENCIA)));
        entry = WeaponDefs_GetEntry(self, byteOffset);
        CSF_AT(float, entry, 0x1A4) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_TIEMPO_DETONACION)));
        entry = WeaponDefs_GetEntry(self, byteOffset);
        CSF_AT(float, entry, 0x1A8) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_VELOCIDAD)));
        entry = WeaponDefs_GetEntry(self, byteOffset);
        CSF_AT(float, entry, 0x1B8) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_ATENUACION_BOTE)));
        entry = WeaponDefs_GetEntry(self, byteOffset);
        CSF_AT(float, entry, 0x1AC) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_ANGULO_LANZAMIENTO)));

        CSF_AT(int, WeaponDefs_GetEntry(self, byteOffset), 304) = -1;
        CSF_AT(int, WeaponDefs_GetEntry(self, byteOffset), 316) = -1;
        Hud_ReadFloatPair(component, nullptr, const_cast<char*>(g_pszKey_DECALUV1),
                          reinterpret_cast<int>(WeaponDefs_GetEntry(self, byteOffset) + 320));
        Hud_ReadFloatPair(component, nullptr, const_cast<char*>(g_pszKey_DECALUV2),
                          reinterpret_cast<int>(WeaponDefs_GetEntry(self, byteOffset) + 328));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_MANO), buffer, 256);
        entry = WeaponDefs_GetEntry(self, byteOffset);
        CSF_AT(int, entry, 0x150) = WeaponDefs_ParseHand(buffer);

        uint8_t* animations = FindProperty(component, nullptr, const_cast<char*>(g_pszKey_ANIMACIONES_8A9D38));
        if (animations) {
            const uint32_t animationCount = MenuExtras_GetChildCount(animations, nullptr);
            for (uint32_t animationIndex = 0; animationIndex < animationCount; ++animationIndex) {
                uint8_t* animation = reinterpret_cast<uint8_t*>(GetComponent(animations, nullptr, animationIndex));
                uint32_t reference[3] = {uint32_t(-1), uint32_t(-1), uint32_t(-1)};
                const int id = ReadIntegerAttribute(animation, nullptr, const_cast<char*>(g_pszKey_ID_8A9C78));
                reference[0] = static_cast<uint32_t>(id);
                void* resolver = CSF_AT(void*, self, 0x4);
                reference[2] = static_cast<uint32_t>(
                    (reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, resolver, 0)[8 / 4]))(resolver, nullptr, id));
                MenuExtras_ReadText(animation, nullptr, const_cast<char*>(g_pszKey_TIPO_8A9C8C), buffer, 256);
                reference[1] = static_cast<uint32_t>(WeaponDefs_ParseAnimationType(buffer));
                if (reference[2] != uint32_t(-1) &&
                    reference[0] != uint32_t(-1) && reference[1] != uint32_t(-1)) {
                    AnimRefList_Add(WeaponDefs_GetEntry(self, byteOffset) + 340, nullptr, reference);
                }
            }
        }

        uint8_t* playerAnimations = FindProperty(component, nullptr, const_cast<char*>(g_pszKey_ANIMACIONES_PLAYER));
        if (playerAnimations) {
            const uint32_t animationCount = MenuExtras_GetChildCount(playerAnimations, nullptr);
            for (uint32_t animationIndex = 0; animationIndex < animationCount; ++animationIndex) {
                uint8_t* animation = reinterpret_cast<uint8_t*>(GetComponent(playerAnimations, nullptr, animationIndex));
                uint32_t reference[6] = {uint32_t(-1), uint32_t(-1), uint32_t(-1), 0, 0, 0};
                const int id = ReadIntegerAttribute(animation, nullptr, const_cast<char*>(g_pszKey_ID_8A9C78));
                reference[0] = static_cast<uint32_t>(id);
                void* resolver = CSF_AT(void*, self, 0x4);
                reference[2] = static_cast<uint32_t>(
                    (reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, resolver, 0)[8 / 4]))(resolver, nullptr, id));
                MenuExtras_ReadText(animation, nullptr, const_cast<char*>(g_pszKey_TIPO_8A9C8C), buffer, 256);
                reference[1] = static_cast<uint32_t>(ParsePlayerAnimTypeName(buffer));
                if (reference[2] != uint32_t(-1) &&
                    reference[0] != uint32_t(-1) && reference[1] != uint32_t(-1)) {
                    AnimRefList_Add(WeaponDefs_GetEntry(self, byteOffset) + 356, nullptr, reference);
                }
            }
        }

        WeaponDefs_FinalizeEntry(reinterpret_cast<int>(WeaponDefs_GetEntry(self, byteOffset)));
    }
    return 1;
}

HOOK(0x00785090, WeaponDefs_LoadData, "auto");
