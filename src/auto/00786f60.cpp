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
CSF_FUNC(int, __cdecl, WeaponDefs_ParseLoadingType, 0x00761620, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseRangeType, 0x00761670, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseHand, 0x007616C0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseAnimationType, 0x007617B0, (char* String1));
CSF_FUNC(int, __cdecl, ParsePlayerAnimTypeName, 0x007618A0, (char* String1));
CSF_FUNC(void, __fastcall, AnimRefList_Add, 0x00771FB0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(bool, __stdcall, WeaponDefs_FinalizeEntry, 0x00783430, (int a1));
CSF_FUNC(int, __fastcall, WeaponDefs_InitializeDefaults, 0x00783EA0, (int a1, int a2));

// Flatten the 552-byte by-value record into its 138 stack words.
// Including count, this callee pops 556 bytes.
CSF_FUNC(int, __fastcall, WeaponDefs_Resize, 0x00784900, (
    void* self, void* edx, int count,
    uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3,
    uint32_t d4, uint32_t d5, uint32_t d6, uint32_t d7,
    uint32_t d8, uint32_t d9, uint32_t d10, uint32_t d11,
    uint32_t d12, uint32_t d13, uint32_t d14, uint32_t d15,
    uint32_t d16, uint32_t d17, uint32_t d18, uint32_t d19,
    uint32_t d20, uint32_t d21, uint32_t d22, uint32_t d23,
    uint32_t d24, uint32_t d25, uint32_t d26, uint32_t d27,
    uint32_t d28, uint32_t d29, uint32_t d30, uint32_t d31,
    uint32_t d32, uint32_t d33, uint32_t d34, uint32_t d35,
    uint32_t d36, uint32_t d37, uint32_t d38, uint32_t d39,
    uint32_t d40, uint32_t d41, uint32_t d42, uint32_t d43,
    uint32_t d44, uint32_t d45, uint32_t d46, uint32_t d47,
    uint32_t d48, uint32_t d49, uint32_t d50, uint32_t d51,
    uint32_t d52, uint32_t d53, uint32_t d54, uint32_t d55,
    uint32_t d56, uint32_t d57, uint32_t d58, uint32_t d59,
    uint32_t d60, uint32_t d61, uint32_t d62, uint32_t d63,
    uint32_t d64, uint32_t d65, uint32_t d66, uint32_t d67,
    uint32_t d68, uint32_t d69, uint32_t d70, uint32_t d71,
    uint32_t d72, uint32_t d73, uint32_t d74, uint32_t d75,
    uint32_t d76, uint32_t d77, uint32_t d78, uint32_t d79,
    uint32_t d80, uint32_t d81, uint32_t d82, uint32_t d83,
    uint32_t d84, uint32_t d85, uint32_t d86, uint32_t d87,
    uint32_t d88, uint32_t d89, uint32_t d90, uint32_t d91,
    uint32_t d92, uint32_t d93, uint32_t d94, uint32_t d95,
    uint32_t d96, uint32_t d97, uint32_t d98, uint32_t d99,
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
CSF_GLOBAL(const char*, g_pszKey_MAX_CARGADORES, 0x008A9CF4);
CSF_GLOBAL(const char*, g_pszKey_DAMAGE_8A9D00, 0x008A9D00);
CSF_GLOBAL(const char*, g_pszKey_DISPERSION, 0x008A9D08);
CSF_GLOBAL(const char*, g_pszKey_RADIO_8A9D0C, 0x008A9D0C);
CSF_GLOBAL(const char*, g_pszKey_CADENCIA, 0x008A9D10);
CSF_GLOBAL(const char*, g_pszKey_DECALUV1, 0x008A9D2C);
CSF_GLOBAL(const char*, g_pszKey_DECALUV2, 0x008A9D30);
CSF_GLOBAL(const char*, g_pszKey_MANO, 0x008A9D34);
CSF_GLOBAL(const char*, g_pszKey_ANIMACIONES_8A9D38, 0x008A9D38);
CSF_GLOBAL(const char*, g_pszKey_ANIMACIONES_PLAYER, 0x008A9D3C);
CSF_GLOBAL(const char*, g_pszKey_ANIMACIONES_PLAYER_TERCERA, 0x008A9D40);
CSF_GLOBAL(const char*, g_pszKey_TIEMPO_DETONACION, 0x008A9D4C);
CSF_GLOBAL(const char*, g_pszKey_VELOCIDAD, 0x008A9D50);
CSF_GLOBAL(const char*, g_pszKey_ANGULO_LANZAMIENTO, 0x008A9D54);
CSF_GLOBAL(const char*, g_pszKey_ATENUACION_BOTE, 0x008A9D60);
CSF_GLOBAL(const char*, g_pszKey_APUNTAR_FOV, 0x008A9D64);
CSF_GLOBAL(const char*, g_pszKey_APUNTAR_VELOCIDAD, 0x008A9D68);

static auto const g_WeaponDataNodeVtable = CSF_VTBL(0x00692B80);

static uint8_t* WeaponDefs_GetEntryAddress(void* self, uint32_t byteOffset)
{
    void* definitions = CSF_AT(void*, self, 0x08);
    return CSF_AT(uint8_t*, definitions, 0x04) + byteOffset;
}

static char __fastcall WeaponDefs_LoadDefinitions(void* self, void* /*edx*/, S_00692b80* source)
{
    char buffer[0x108]{};
    uint8_t* list = LookupNamedElement(reinterpret_cast<uint8_t*>(source), nullptr,
                                       g_pszKey_LISTADATOS_8A9C74);
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

    uint32_t byteOffset = 0;
    for (uint32_t index = 0; index < count; ++index, byteOffset += 552) {
        uint8_t* component = reinterpret_cast<uint8_t*>(GetComponent(list, nullptr, index));
        uint8_t* entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(int, entry, 0x00) =
            ReadIntegerAttribute(component, nullptr, const_cast<char*>(g_pszKey_ID_8A9C78));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_NOMBRE_8A9C7C), buffer, 256);
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        AssignStringRange_429B60(entry + 4, nullptr, buffer,
                                static_cast<unsigned int>(std::strlen(buffer)));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_FILE_8A9C84), buffer, 256);
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        AssignStringRange_429B60(entry + 60, nullptr, buffer,
                                static_cast<unsigned int>(std::strlen(buffer)));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_FILE2), buffer, 256);
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        AssignStringRange_429B60(entry + 88, nullptr, buffer,
                                static_cast<unsigned int>(std::strlen(buffer)));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_TIPO_8A9C8C), buffer, 256);
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(int, entry, 0x74) = WeaponDefs_ParseWeaponType(buffer);

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_AUTOMATISMO), buffer, 256);
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(int, entry, 0x7C) = WeaponDefs_ParseAutomaticMode(buffer);

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_FOGONAZO_DFF), buffer, 256);
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        AssignStringRange_429B60(entry + 136, nullptr, buffer,
                                static_cast<unsigned int>(std::strlen(buffer)));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_FOGONAZO_TEXTURE), buffer, 256);
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        AssignStringRange_429B60(entry + 164, nullptr, buffer,
                                static_cast<unsigned int>(std::strlen(buffer)));

        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(int, entry, 0xC0) =
            ReadIntegerAttribute(component, nullptr, const_cast<char*>(g_pszKey_FOGONAZO_COLOR));
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0xC4) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_FOGONAZO_DURACION)));
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0xC8) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_FOGONAZO_RADIO)));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_CARGA), buffer, 256);
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(int, entry, 0xD4) = WeaponDefs_ParseLoadingType(buffer);

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_ALCANCE), buffer, 256);
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(int, entry, 0xD8) = WeaponDefs_ParseRangeType(buffer);

        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0xE8) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_ALCANCE_RANGO_MIN)));
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0xEC) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_ALCANCE_RANGO_MAX)));
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0xF8) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_DESVIACION_H)));
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0xFC) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_DESVIACION_V)));
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0x100) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_DESVIACION_FA)));

        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(int, entry, 0x104) =
            ReadIntegerAttribute(component, nullptr, const_cast<char*>(g_pszKey_BALAS));
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(int, entry, 0x108) =
            ReadIntegerAttribute(component, nullptr, const_cast<char*>(g_pszKey_CARGADORES));
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(int, entry, 0x10C) =
            ReadIntegerAttribute(component, nullptr, const_cast<char*>(g_pszKey_MAX_CARGADORES));
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(int, entry, 0x118) =
            ReadIntegerAttribute(component, nullptr, const_cast<char*>(g_pszKey_DAMAGE_8A9D00));

        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0x120) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_DISPERSION)));
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0x124) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_RADIO_8A9D0C)));
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0x128) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_CADENCIA)));
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0x1A4) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_TIEMPO_DETONACION)));
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0x1A8) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_VELOCIDAD)));
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0x1B8) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_ATENUACION_BOTE)));
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0x1AC) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_ANGULO_LANZAMIENTO)));
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0x1BC) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_APUNTAR_FOV)));
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(float, entry, 0x1C0) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_APUNTAR_VELOCIDAD)));

        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(int, entry, 0x130) = -1;
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(int, entry, 0x13C) = -1;

        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        Hud_ReadFloatPair(component, nullptr, const_cast<char*>(g_pszKey_DECALUV1),
                          reinterpret_cast<int>(entry + 320));
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        Hud_ReadFloatPair(component, nullptr, const_cast<char*>(g_pszKey_DECALUV2),
                          reinterpret_cast<int>(entry + 328));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_MANO), buffer, 256);
        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        CSF_AT(int, entry, 0x150) = WeaponDefs_ParseHand(buffer);

        uint8_t* animations =
            FindProperty(component, nullptr, const_cast<char*>(g_pszKey_ANIMACIONES_8A9D38));
        if (animations) {
            uint32_t animationCount = MenuExtras_GetChildCount(animations, nullptr);
            for (uint32_t animationIndex = 0; animationIndex < animationCount; ++animationIndex) {
                uint8_t* animation =
                    reinterpret_cast<uint8_t*>(GetComponent(animations, nullptr, animationIndex));
                uint32_t reference[7]{};
                reference[0] = reference[1] = reference[2] = static_cast<uint32_t>(-1);
                int id = ReadIntegerAttribute(animation, nullptr, const_cast<char*>(g_pszKey_ID_8A9C78));
                reference[0] = static_cast<uint32_t>(id);
                void* animationTable = CSF_AT(void*, self, 0x04);
                reference[2] = static_cast<uint32_t>(
                    ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, animationTable, 0)[2])
                    (animationTable, nullptr, id));
                MenuExtras_ReadText(animation, nullptr, const_cast<char*>(g_pszKey_TIPO_8A9C8C), buffer, 256);
                reference[1] = static_cast<uint32_t>(WeaponDefs_ParseAnimationType(buffer));
                if (reference[2] != static_cast<uint32_t>(-1) &&
                    reference[0] != static_cast<uint32_t>(-1) &&
                    reference[1] != static_cast<uint32_t>(-1)) {
                    entry = WeaponDefs_GetEntryAddress(self, byteOffset);
                    AnimRefList_Add(entry + 340, nullptr, reference);
                }
            }
        }

        uint8_t* playerAnimations =
            FindProperty(component, nullptr, const_cast<char*>(g_pszKey_ANIMACIONES_PLAYER));
        if (playerAnimations) {
            uint32_t animationCount = MenuExtras_GetChildCount(playerAnimations, nullptr);
            for (uint32_t animationIndex = 0; animationIndex < animationCount; ++animationIndex) {
                uint8_t* animation =
                    reinterpret_cast<uint8_t*>(GetComponent(playerAnimations, nullptr, animationIndex));
                uint32_t reference[3];
                reference[0] = reference[1] = reference[2] = static_cast<uint32_t>(-1);
                int id = ReadIntegerAttribute(animation, nullptr, const_cast<char*>(g_pszKey_ID_8A9C78));
                reference[0] = static_cast<uint32_t>(id);
                void* animationTable = CSF_AT(void*, self, 0x04);
                reference[2] = static_cast<uint32_t>(
                    ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, animationTable, 0)[2])
                    (animationTable, nullptr, id));
                MenuExtras_ReadText(animation, nullptr, const_cast<char*>(g_pszKey_TIPO_8A9C8C), buffer, 256);
                reference[1] = static_cast<uint32_t>(ParsePlayerAnimTypeName(buffer));
                if (reference[2] != static_cast<uint32_t>(-1) &&
                    reference[0] != static_cast<uint32_t>(-1) &&
                    reference[1] != static_cast<uint32_t>(-1)) {
                    entry = WeaponDefs_GetEntryAddress(self, byteOffset);
                    AnimRefList_Add(entry + 356, nullptr, reference);
                }
            }
        }

        uint8_t* thirdPersonAnimations =
            FindProperty(component, nullptr, const_cast<char*>(g_pszKey_ANIMACIONES_PLAYER_TERCERA));
        if (thirdPersonAnimations) {
            uint32_t animationCount = MenuExtras_GetChildCount(thirdPersonAnimations, nullptr);
            for (uint32_t animationIndex = 0; animationIndex < animationCount; ++animationIndex) {
                uint8_t* animation =
                    reinterpret_cast<uint8_t*>(GetComponent(thirdPersonAnimations, nullptr, animationIndex));
                uint32_t reference[3];
                reference[0] = reference[1] = reference[2] = static_cast<uint32_t>(-1);
                int id = ReadIntegerAttribute(animation, nullptr, const_cast<char*>(g_pszKey_ID_8A9C78));
                reference[0] = static_cast<uint32_t>(id);
                void* animationTable = CSF_AT(void*, self, 0x04);
                reference[2] = static_cast<uint32_t>(
                    ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, animationTable, 0)[2])
                    (animationTable, nullptr, id));
                MenuExtras_ReadText(animation, nullptr, const_cast<char*>(g_pszKey_TIPO_8A9C8C), buffer, 256);
                reference[1] = static_cast<uint32_t>(ParsePlayerAnimTypeName(buffer));
                if (reference[2] != static_cast<uint32_t>(-1) &&
                    reference[0] != static_cast<uint32_t>(-1) &&
                    reference[1] != static_cast<uint32_t>(-1)) {
                    entry = WeaponDefs_GetEntryAddress(self, byteOffset);
                    AnimRefList_Add(entry + 372, nullptr, reference);
                }
            }
        }

        entry = WeaponDefs_GetEntryAddress(self, byteOffset);
        WeaponDefs_FinalizeEntry(reinterpret_cast<int>(entry));
    }
    return 1;
}

HOOK(0x00786F60, WeaponDefs_LoadDefinitions, "auto");
