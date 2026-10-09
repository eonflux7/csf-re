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
CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0, (std::uint32_t component));
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

// Expand the 552-byte by-value record into its 138 stack words.
// Together with count, these occupy the callee's 556 popped bytes.
CSF_FUNC(int, __fastcall, WeaponDefs_Resize, 0x00784900, (
    void* self, void* edx, int count,
    uint32_t w0, uint32_t w1, uint32_t w2, uint32_t w3,
    uint32_t w4, uint32_t w5, uint32_t w6, uint32_t w7,
    uint32_t w8, uint32_t w9, uint32_t w10, uint32_t w11,
    uint32_t w12, uint32_t w13, uint32_t w14, uint32_t w15,
    uint32_t w16, uint32_t w17, uint32_t w18, uint32_t w19,
    uint32_t w20, uint32_t w21, uint32_t w22, uint32_t w23,
    uint32_t w24, uint32_t w25, uint32_t w26, uint32_t w27,
    uint32_t w28, uint32_t w29, uint32_t w30, uint32_t w31,
    uint32_t w32, uint32_t w33, uint32_t w34, uint32_t w35,
    uint32_t w36, uint32_t w37, uint32_t w38, uint32_t w39,
    uint32_t w40, uint32_t w41, uint32_t w42, uint32_t w43,
    uint32_t w44, uint32_t w45, uint32_t w46, uint32_t w47,
    uint32_t w48, uint32_t w49, uint32_t w50, uint32_t w51,
    uint32_t w52, uint32_t w53, uint32_t w54, uint32_t w55,
    uint32_t w56, uint32_t w57, uint32_t w58, uint32_t w59,
    uint32_t w60, uint32_t w61, uint32_t w62, uint32_t w63,
    uint32_t w64, uint32_t w65, uint32_t w66, uint32_t w67,
    uint32_t w68, uint32_t w69, uint32_t w70, uint32_t w71,
    uint32_t w72, uint32_t w73, uint32_t w74, uint32_t w75,
    uint32_t w76, uint32_t w77, uint32_t w78, uint32_t w79,
    uint32_t w80, uint32_t w81, uint32_t w82, uint32_t w83,
    uint32_t w84, uint32_t w85, uint32_t w86, uint32_t w87,
    uint32_t w88, uint32_t w89, uint32_t w90, uint32_t w91,
    uint32_t w92, uint32_t w93, uint32_t w94, uint32_t w95,
    uint32_t w96, uint32_t w97, uint32_t w98, uint32_t w99,
    uint32_t w100, uint32_t w101, uint32_t w102, uint32_t w103,
    uint32_t w104, uint32_t w105, uint32_t w106, uint32_t w107,
    uint32_t w108, uint32_t w109, uint32_t w110, uint32_t w111,
    uint32_t w112, uint32_t w113, uint32_t w114, uint32_t w115,
    uint32_t w116, uint32_t w117, uint32_t w118, uint32_t w119,
    uint32_t w120, uint32_t w121, uint32_t w122, uint32_t w123,
    uint32_t w124, uint32_t w125, uint32_t w126, uint32_t w127,
    uint32_t w128, uint32_t w129, uint32_t w130, uint32_t w131,
    uint32_t w132, uint32_t w133, uint32_t w134, uint32_t w135,
    uint32_t w136, uint32_t w137));

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
CSF_GLOBAL(const char*, g_pszKey_BALAS_DISPONIBLES, 0x008A9CF8);
CSF_GLOBAL(const char*, g_pszKey_MAX_BALAS_DISPONIBLES, 0x008A9CFC);
CSF_GLOBAL(const char*, g_pszKey_DAMAGE_8A9D00, 0x008A9D00);
CSF_GLOBAL(const char*, g_pszKey_DISPERSION, 0x008A9D08);
CSF_GLOBAL(const char*, g_pszKey_RADIO_8A9D0C, 0x008A9D0C);
CSF_GLOBAL(const char*, g_pszKey_CADENCIA, 0x008A9D10);
CSF_GLOBAL(const char*, g_pszKey_WAVE_DISPARO, 0x008A9D18);
CSF_GLOBAL(const char*, g_pszKey_WAVE_RECARGA, 0x008A9D24);
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
CSF_GLOBAL(const char*, g_pszKey_PULSO_ANGULO, 0x008A9D70);
CSF_GLOBAL(const char*, g_pszKey_PULSO_TIEMPO, 0x008A9D74);
CSF_GLOBAL(const char*, g_pszKey_PULSO_FACTOR, 0x008A9D78);
CSF_GLOBAL(const char*, g_pszKey_PULSO_ESTABLE_DURACION, 0x008A9D7C);
CSF_GLOBAL(const char*, g_pszKey_PULSO_ESTABLE_COOLDOWN, 0x008A9D80);

static uint8_t* WeaponDefs_GetEntry(void* self, uint32_t byteOffset)
{
    return CSF_AT(uint8_t*, CSF_AT(void*, self, 8), 4) + byteOffset;
}

static void WeaponDefs_ReadAnimationList(
    void* self, uint8_t* property, uint32_t byteOffset,
    uint32_t listOffset, char* buffer, int (__cdecl* parseType)(char*))
{
    if (!property)
        return;

    const uint32_t count = MenuExtras_GetChildCount(property, nullptr);
    for (uint32_t index = 0; index < count; ++index) {
        uint8_t* animation = reinterpret_cast<uint8_t*>(
            GetComponent(property, nullptr, index));

        uint32_t reference[7] = {};
        reference[0] = static_cast<uint32_t>(-1);
        reference[1] = static_cast<uint32_t>(-1);
        reference[2] = static_cast<uint32_t>(-1);

        const int id = ReadIntegerAttribute(
            animation, nullptr, const_cast<char*>(g_pszKey_ID_8A9C78));
        void* resolver = CSF_AT(void*, self, 4);
        reference[0] = static_cast<uint32_t>(id);
        reference[2] = static_cast<uint32_t>(
            (reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, resolver, 0)[8 / 4]))(resolver, nullptr, id));

        MenuExtras_ReadText(animation, nullptr,
                           const_cast<char*>(g_pszKey_TIPO_8A9C8C),
                           buffer, 256);
        reference[1] = static_cast<uint32_t>(parseType(buffer));

        if (reference[2] != static_cast<uint32_t>(-1) &&
            reference[0] != static_cast<uint32_t>(-1) &&
            reference[1] != static_cast<uint32_t>(-1)) {
            AnimRefList_Add(WeaponDefs_GetEntry(self, byteOffset) + listOffset,
                            nullptr, reference);
        }
    }
}

#define WEAPON_READ_INTEGER(offset, key)                                      \
    do {                                                                      \
        uint8_t* entry = WeaponDefs_GetEntry(self, byteOffset);                 \
        const int value = ReadIntegerAttribute(                               \
            component, nullptr, const_cast<char*>(key));                       \
        CSF_AT(int, entry, offset) = value;                                    \
    } while (false)

#define WEAPON_READ_FLOAT(offset, key)                                        \
    do {                                                                      \
        uint8_t* entry = WeaponDefs_GetEntry(self, byteOffset);                 \
        const float value = static_cast<float>(ReadFloatAttribute(             \
            component, nullptr, const_cast<char*>(key)));                      \
        CSF_AT(float, entry, offset) = value;                                  \
    } while (false)

#define WEAPON_READ_STRING(offset, key)                                       \
    do {                                                                      \
        MenuExtras_ReadText(component, nullptr, const_cast<char*>(key),        \
                           buffer, 256);                                      \
        uint8_t* entry = WeaponDefs_GetEntry(self, byteOffset);                 \
        AssignStringRange_429B60(entry + offset, nullptr, buffer,               \
                                static_cast<unsigned int>(std::strlen(buffer))); \
    } while (false)

#define WEAPON_READ_ENUM(offset, key, parser)                                 \
    do {                                                                      \
        MenuExtras_ReadText(component, nullptr, const_cast<char*>(key),        \
                           buffer, 256);                                      \
        uint8_t* entry = WeaponDefs_GetEntry(self, byteOffset);                 \
        const int value = parser(buffer);                                     \
        CSF_AT(int, entry, offset) = value;                                    \
    } while (false)

#define WEAPON_READ_OPTIONAL_FLOAT(offset, key)                               \
    do {                                                                      \
        uint8_t* property = FindProperty(                                     \
            component, nullptr, const_cast<char*>(key));                       \
        if (property) {                                                       \
            uint8_t* entry = WeaponDefs_GetEntry(self, byteOffset);             \
            const float value = static_cast<float>(ReadComponentFloat(         \
                reinterpret_cast<uint32_t>(property)));                        \
            CSF_AT(float, entry, offset) = value;                              \
        }                                                                     \
    } while (false)

static char __fastcall WeaponDefs_LoadDefinitions(
    void* self, void* /*edx*/, S_00692b80* source)
{
    char buffer[0x108] = {};
    uint8_t* list = LookupNamedElement(
        reinterpret_cast<uint8_t*>(source), nullptr,
        g_pszKey_LISTADATOS_8A9C74);

    (void)CSF_VTBL(0x00692B80);
    const uint32_t count = MenuExtras_GetChildCount(list, nullptr);
    void* entries = CSF_AT(void*, self, 8);
    WeaponDefRecord defaults = {};
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

    uint32_t byteOffset = 0;
    for (uint32_t index = 0; index < count; ++index, byteOffset += 552) {
        uint8_t* component = reinterpret_cast<uint8_t*>(
            GetComponent(list, nullptr, index));

        WEAPON_READ_INTEGER(0, g_pszKey_ID_8A9C78);
        WEAPON_READ_STRING(4, g_pszKey_NOMBRE_8A9C7C);
        WEAPON_READ_STRING(60, g_pszKey_FILE_8A9C84);
        WEAPON_READ_STRING(88, g_pszKey_FILE2);
        WEAPON_READ_ENUM(116, g_pszKey_TIPO_8A9C8C, WeaponDefs_ParseWeaponType);
        WEAPON_READ_ENUM(124, g_pszKey_AUTOMATISMO, WeaponDefs_ParseAutomaticMode);
        WEAPON_READ_STRING(136, g_pszKey_FOGONAZO_DFF);
        WEAPON_READ_STRING(164, g_pszKey_FOGONAZO_TEXTURE);
        WEAPON_READ_INTEGER(192, g_pszKey_FOGONAZO_COLOR);
        WEAPON_READ_FLOAT(196, g_pszKey_FOGONAZO_DURACION);
        WEAPON_READ_FLOAT(200, g_pszKey_FOGONAZO_RADIO);
        WEAPON_READ_ENUM(212, g_pszKey_CARGA, WeaponDefs_ParseLoadingMode);
        WEAPON_READ_ENUM(216, g_pszKey_ALCANCE, WeaponDefs_ParseRangeType);
        WEAPON_READ_FLOAT(232, g_pszKey_ALCANCE_RANGO_MIN);
        WEAPON_READ_FLOAT(236, g_pszKey_ALCANCE_RANGO_MAX);
        WEAPON_READ_FLOAT(248, g_pszKey_DESVIACION_H);
        WEAPON_READ_FLOAT(252, g_pszKey_DESVIACION_V);
        WEAPON_READ_FLOAT(256, g_pszKey_DESVIACION_FA);
        WEAPON_READ_INTEGER(260, g_pszKey_BALAS);
        WEAPON_READ_INTEGER(264, g_pszKey_CARGADORES);
        WEAPON_READ_INTEGER(268, g_pszKey_MAX_CARGADORES);
        WEAPON_READ_INTEGER(272, g_pszKey_BALAS_DISPONIBLES);
        WEAPON_READ_INTEGER(276, g_pszKey_MAX_BALAS_DISPONIBLES);
        WEAPON_READ_INTEGER(280, g_pszKey_DAMAGE_8A9D00);
        WEAPON_READ_FLOAT(288, g_pszKey_DISPERSION);
        WEAPON_READ_FLOAT(292, g_pszKey_RADIO_8A9D0C);
        WEAPON_READ_FLOAT(296, g_pszKey_CADENCIA);
        WEAPON_READ_FLOAT(420, g_pszKey_TIEMPO_DETONACION);
        WEAPON_READ_FLOAT(424, g_pszKey_VELOCIDAD);
        WEAPON_READ_FLOAT(440, g_pszKey_ATENUACION_BOTE);
        WEAPON_READ_FLOAT(428, g_pszKey_ANGULO_LANZAMIENTO);
        WEAPON_READ_FLOAT(444, g_pszKey_APUNTAR_FOV);
        WEAPON_READ_FLOAT(448, g_pszKey_APUNTAR_VELOCIDAD);
        WEAPON_READ_INTEGER(304, g_pszKey_WAVE_DISPARO);
        WEAPON_READ_INTEGER(316, g_pszKey_WAVE_RECARGA);

        Hud_ReadFloatPair(component, nullptr,
                          const_cast<char*>(g_pszKey_DECALUV1),
                          reinterpret_cast<int>(
                              WeaponDefs_GetEntry(self, byteOffset) + 320));
        Hud_ReadFloatPair(component, nullptr,
                          const_cast<char*>(g_pszKey_DECALUV2),
                          reinterpret_cast<int>(
                              WeaponDefs_GetEntry(self, byteOffset) + 328));
        WEAPON_READ_ENUM(336, g_pszKey_MANO, WeaponDefs_ParseHand);

        WEAPON_READ_OPTIONAL_FLOAT(452, g_pszKey_PULSO_ANGULO);
        WEAPON_READ_OPTIONAL_FLOAT(456, g_pszKey_PULSO_TIEMPO);
        WEAPON_READ_OPTIONAL_FLOAT(460, g_pszKey_PULSO_FACTOR);
        WEAPON_READ_OPTIONAL_FLOAT(464, g_pszKey_PULSO_ESTABLE_DURACION);
        WEAPON_READ_OPTIONAL_FLOAT(468, g_pszKey_PULSO_ESTABLE_COOLDOWN);

        uint8_t* animations = FindProperty(
            component, nullptr, const_cast<char*>(g_pszKey_ANIMACIONES_8A9D38));
        WeaponDefs_ReadAnimationList(self, animations, byteOffset, 340,
                                     buffer, WeaponDefs_ParseAnimationType);

        uint8_t* playerAnimations = FindProperty(
            component, nullptr, const_cast<char*>(g_pszKey_ANIMACIONES_PLAYER));
        WeaponDefs_ReadAnimationList(self, playerAnimations, byteOffset, 356,
                                     buffer, ParsePlayerAnimTypeName);

        uint8_t* thirdPersonAnimations = FindProperty(
            component, nullptr,
            const_cast<char*>(g_pszKey_ANIMACIONES_PLAYER_TERCERA));
        WeaponDefs_ReadAnimationList(self, thirdPersonAnimations, byteOffset, 372,
                                     buffer, ParsePlayerAnimTypeName);

        WeaponDefs_FinalizeEntry(
            reinterpret_cast<int>(WeaponDefs_GetEntry(self, byteOffset)));
    }
    return 1;
}

#undef WEAPON_READ_INTEGER
#undef WEAPON_READ_FLOAT
#undef WEAPON_READ_STRING
#undef WEAPON_READ_ENUM
#undef WEAPON_READ_OPTIONAL_FLOAT

HOOK(0x007881C0, WeaponDefs_LoadDefinitions, "auto");
