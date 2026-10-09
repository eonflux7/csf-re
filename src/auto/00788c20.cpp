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
CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(void, __fastcall, Hud_ReadFloatPair, 0x00693A20, (void* self, void* edx, char* String1, int a3));

CSF_FUNC(int, __cdecl, WeaponDefs_ParseWeaponType, 0x00761530, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseFireMode, 0x007615D0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseLoadingType, 0x00761620, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseRangeType, 0x00761670, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseHand, 0x007616C0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseWeightClass, 0x00761710, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseCharacterType, 0x00761760, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseAnimationType, 0x007617B0, (char* String1));
CSF_FUNC(int, __cdecl, ParsePlayerAnimTypeName, 0x007618A0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseGrade, 0x00761B70, (char* String1));
CSF_FUNC(void, __fastcall, AnimRefList_Add, 0x00771FB0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(bool, __stdcall, WeaponDefs_FinalizeEntry, 0x00783430, (int a1));
CSF_FUNC(int, __fastcall, WeaponDefs_InitializeDefaults, 0x00783EA0, (int a1, int a2));

// The by-value record occupies 138 stack dwords, following the count.
// Spell out its ABI rather than using an aggregate parameter.
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

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(const char*, g_pszKey_LISTADATOS_8A9C74, 0x008A9C74);
CSF_GLOBAL(const char*, g_pszKey_ID_8A9C78, 0x008A9C78);
CSF_GLOBAL(const char*, g_pszKey_NOMBRE_8A9C7C, 0x008A9C7C);
CSF_GLOBAL(const char*, g_pszKey_FILE_8A9C84, 0x008A9C84);
CSF_GLOBAL(const char*, g_pszKey_FILE2, 0x008A9C88);
CSF_GLOBAL(const char*, g_pszKey_TIPO_8A9C8C, 0x008A9C8C);
CSF_GLOBAL(const char*, g_pszKey_AUTOMATISMO, 0x008A9C94);
CSF_GLOBAL(const char*, g_pszKey_GRADO_8A9C98, 0x008A9C98);
CSF_GLOBAL(const char*, g_pszKey_FOGONAZO_DFF, 0x008A9CA0);
CSF_GLOBAL(const char*, g_pszKey_FOGONAZO_TEXTURE, 0x008A9CA4);
CSF_GLOBAL(const char*, g_pszKey_FOGONAZO_COLOR, 0x008A9CA8);
CSF_GLOBAL(const char*, g_pszKey_FOGONAZO_DURACION, 0x008A9CAC);
CSF_GLOBAL(const char*, g_pszKey_FOGONAZO_RADIO, 0x008A9CB0);
CSF_GLOBAL(const char*, g_pszKey_FOGONAZO_ANCHO, 0x008A9CB4);
CSF_GLOBAL(const char*, g_pszKey_FOGONAZO_ALTO, 0x008A9CB8);
CSF_GLOBAL(const char*, g_pszKey_CARGA, 0x008A9CBC);
CSF_GLOBAL(const char*, g_pszKey_ALCANCE, 0x008A9CC0);
CSF_GLOBAL(const char*, g_pszKey_PESO, 0x008A9CC4);
CSF_GLOBAL(const char*, g_pszKey_PERSONAJE, 0x008A9CC8);
CSF_GLOBAL(const char*, g_pszKey_ALCANCE_RANGO_MIN, 0x008A9CD0);
CSF_GLOBAL(const char*, g_pszKey_ALCANCE_RANGO_MAX, 0x008A9CD4);
CSF_GLOBAL(const char*, g_pszKey_LIMITE_ANGULAR_HORIZONTAL, 0x008A9CD8);
CSF_GLOBAL(const char*, g_pszKey_LIMITE_ANGULAR_VERTICAL, 0x008A9CDC);
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

[[maybe_unused]] static auto g_MenuNodeVtable = CSF_VTBL(0x00692B80);

static uint8_t* WeaponDefs_GetEntry(void* self, uint32_t entryOffset)
{
    void* entries = CSF_AT(void*, self, 0x08);
    return CSF_AT(uint8_t*, entries, 0x04) + entryOffset;
}

static void WeaponDefs_ResizeWithDefaults(
    void* entries, uint32_t count, const WeaponDefRecord& value)
{
    WeaponDefs_Resize(entries, nullptr, static_cast<int>(count),
        value.data[0], value.data[1], value.data[2], value.data[3],
        value.data[4], value.data[5], value.data[6], value.data[7],
        value.data[8], value.data[9], value.data[10], value.data[11],
        value.data[12], value.data[13], value.data[14], value.data[15],
        value.data[16], value.data[17], value.data[18], value.data[19],
        value.data[20], value.data[21], value.data[22], value.data[23],
        value.data[24], value.data[25], value.data[26], value.data[27],
        value.data[28], value.data[29], value.data[30], value.data[31],
        value.data[32], value.data[33], value.data[34], value.data[35],
        value.data[36], value.data[37], value.data[38], value.data[39],
        value.data[40], value.data[41], value.data[42], value.data[43],
        value.data[44], value.data[45], value.data[46], value.data[47],
        value.data[48], value.data[49], value.data[50], value.data[51],
        value.data[52], value.data[53], value.data[54], value.data[55],
        value.data[56], value.data[57], value.data[58], value.data[59],
        value.data[60], value.data[61], value.data[62], value.data[63],
        value.data[64], value.data[65], value.data[66], value.data[67],
        value.data[68], value.data[69], value.data[70], value.data[71],
        value.data[72], value.data[73], value.data[74], value.data[75],
        value.data[76], value.data[77], value.data[78], value.data[79],
        value.data[80], value.data[81], value.data[82], value.data[83],
        value.data[84], value.data[85], value.data[86], value.data[87],
        value.data[88], value.data[89], value.data[90], value.data[91],
        value.data[92], value.data[93], value.data[94], value.data[95],
        value.data[96], value.data[97], value.data[98], value.data[99],
        value.data[100], value.data[101], value.data[102], value.data[103],
        value.data[104], value.data[105], value.data[106], value.data[107],
        value.data[108], value.data[109], value.data[110], value.data[111],
        value.data[112], value.data[113], value.data[114], value.data[115],
        value.data[116], value.data[117], value.data[118], value.data[119],
        value.data[120], value.data[121], value.data[122], value.data[123],
        value.data[124], value.data[125], value.data[126], value.data[127],
        value.data[128], value.data[129], value.data[130], value.data[131],
        value.data[132], value.data[133], value.data[134], value.data[135],
        value.data[136], value.data[137]);
}

static void WeaponDefs_ReadAnimationReferences(
    void* self, uint32_t entryOffset, uint8_t* animations,
    uint32_t listOffset, char* buffer, int (__cdecl* parseType)(char*))
{
    const uint32_t count = MenuExtras_GetChildCount(animations, nullptr);
    for (uint32_t index = 0; index < count; ++index) {
        uint8_t* animation =
            reinterpret_cast<uint8_t*>(GetComponent(animations, nullptr, index));

        uint32_t reference[7] = {};
        reference[0] = static_cast<uint32_t>(-1);
        reference[1] = static_cast<uint32_t>(-1);
        reference[2] = static_cast<uint32_t>(-1);

        const int id = ReadIntegerAttribute(
            animation, nullptr, const_cast<char*>(g_pszKey_ID_8A9C78));
        reference[0] = static_cast<uint32_t>(id);

        void* animationTable = CSF_AT(void*, self, 0x04);
        reference[2] = static_cast<uint32_t>(
            ((int (__fastcall*)(void*, void*, int))
                CSF_AT(void**, animationTable, 0)[2])(
                    animationTable, nullptr, id));

        MenuExtras_ReadText(animation, nullptr,
            const_cast<char*>(g_pszKey_TIPO_8A9C8C), buffer, 256);
        reference[1] = static_cast<uint32_t>(parseType(buffer));

        if (reference[2] != static_cast<uint32_t>(-1) &&
            reference[0] != static_cast<uint32_t>(-1) &&
            reference[1] != static_cast<uint32_t>(-1)) {
            AnimRefList_Add(
                WeaponDefs_GetEntry(self, entryOffset) + listOffset,
                nullptr, reference);
        }
    }
}

#define KEY(name) const_cast<char*>(g_pszKey_##name)

#define READ_INT(offset, key) \
    do { \
        entry = WeaponDefs_GetEntry(self, entryOffset); \
        CSF_AT(int, entry, offset) = \
            ReadIntegerAttribute(component, nullptr, KEY(key)); \
    } while (0)

#define READ_FLOAT(offset, key) \
    do { \
        entry = WeaponDefs_GetEntry(self, entryOffset); \
        CSF_AT(float, entry, offset) = static_cast<float>( \
            ReadFloatAttribute(component, nullptr, KEY(key))); \
    } while (0)

#define READ_STRING(offset, key) \
    do { \
        MenuExtras_ReadText(component, nullptr, KEY(key), buffer, 256); \
        entry = WeaponDefs_GetEntry(self, entryOffset); \
        AssignStringRange_429B60(entry + offset, nullptr, buffer, \
            static_cast<unsigned int>(std::strlen(buffer))); \
    } while (0)

#define READ_ENUM(offset, key, parser) \
    do { \
        MenuExtras_ReadText(component, nullptr, KEY(key), buffer, 256); \
        entry = WeaponDefs_GetEntry(self, entryOffset); \
        CSF_AT(int, entry, offset) = parser(buffer); \
    } while (0)

#define READ_OPTIONAL_FLOAT(offset, key) \
    do { \
        uint8_t* property = FindProperty(component, nullptr, KEY(key)); \
        if (property) { \
            entry = WeaponDefs_GetEntry(self, entryOffset); \
            CSF_AT(float, entry, offset) = static_cast<float>( \
                ReadComponentFloat(reinterpret_cast<uint32_t>(property))); \
        } \
    } while (0)

static char __fastcall WeaponDefs_LoadDefinitions(
    void* self, void* /*edx*/, S_00692b80* root)
{
    char buffer[0x108];
    std::memset(buffer, 0, 256);

    uint8_t* definitions = LookupNamedElement(
        reinterpret_cast<uint8_t*>(root), nullptr,
        g_pszKey_LISTADATOS_8A9C74);
    const uint32_t count = MenuExtras_GetChildCount(definitions, nullptr);

    void* entries = CSF_AT(void*, self, 0x08);
    WeaponDefRecord defaults;
    WeaponDefs_InitializeDefaults(reinterpret_cast<int>(&defaults), 0);
    WeaponDefs_ResizeWithDefaults(entries, count, defaults);

    uint32_t entryOffset = 0;
    for (uint32_t index = 0; index < count; ++index, entryOffset += 552) {
        uint8_t* component =
            reinterpret_cast<uint8_t*>(GetComponent(definitions, nullptr, index));
        uint8_t* entry;

        READ_INT(0x000, ID_8A9C78);
        READ_STRING(0x004, NOMBRE_8A9C7C);
        READ_STRING(0x03C, FILE_8A9C84);
        READ_STRING(0x058, FILE2);
        READ_ENUM(0x074, TIPO_8A9C8C, WeaponDefs_ParseWeaponType);
        READ_ENUM(0x07C, AUTOMATISMO, WeaponDefs_ParseFireMode);

        if (FindProperty(component, nullptr, KEY(GRADO_8A9C98))) {
            READ_ENUM(0x080, GRADO_8A9C98, WeaponDefs_ParseGrade);
        }
        if (FindProperty(component, nullptr, KEY(PESO))) {
            READ_ENUM(0x0DC, PESO, WeaponDefs_ParseWeightClass);
        }
        if (FindProperty(component, nullptr, KEY(PERSONAJE))) {
            READ_ENUM(0x0E0, PERSONAJE, WeaponDefs_ParseCharacterType);
        }

        READ_STRING(0x088, FOGONAZO_DFF);
        READ_STRING(0x0A4, FOGONAZO_TEXTURE);
        READ_INT(0x0C0, FOGONAZO_COLOR);
        READ_FLOAT(0x0C4, FOGONAZO_DURACION);
        READ_FLOAT(0x0C8, FOGONAZO_RADIO);

        entry = WeaponDefs_GetEntry(self, entryOffset);
        CSF_AT(float, entry, 0x0CC) = static_cast<float>(
            ReadFloatProperty(component, nullptr, KEY(FOGONAZO_ANCHO)));
        entry = WeaponDefs_GetEntry(self, entryOffset);
        if (CSF_AT(float, entry, 0x0CC) == g_fZero)
            CSF_AT(float, entry, 0x0CC) = 15.0f;

        entry = WeaponDefs_GetEntry(self, entryOffset);
        CSF_AT(float, entry, 0x0D0) = static_cast<float>(
            ReadFloatProperty(component, nullptr, KEY(FOGONAZO_ALTO)));
        entry = WeaponDefs_GetEntry(self, entryOffset);
        if (CSF_AT(float, entry, 0x0D0) == g_fZero)
            CSF_AT(float, entry, 0x0D0) = 15.0f;

        READ_ENUM(0x0D4, CARGA, WeaponDefs_ParseLoadingType);
        READ_ENUM(0x0D8, ALCANCE, WeaponDefs_ParseRangeType);
        READ_FLOAT(0x0E8, ALCANCE_RANGO_MIN);
        READ_FLOAT(0x0EC, ALCANCE_RANGO_MAX);
        READ_FLOAT(0x0F0, LIMITE_ANGULAR_HORIZONTAL);
        READ_FLOAT(0x0F4, LIMITE_ANGULAR_VERTICAL);
        READ_FLOAT(0x0F8, DESVIACION_H);
        READ_FLOAT(0x0FC, DESVIACION_V);
        READ_FLOAT(0x100, DESVIACION_FA);
        READ_INT(0x104, BALAS);
        READ_INT(0x108, CARGADORES);
        READ_INT(0x10C, MAX_CARGADORES);
        READ_INT(0x110, BALAS_DISPONIBLES);
        READ_INT(0x114, MAX_BALAS_DISPONIBLES);
        READ_INT(0x118, DAMAGE_8A9D00);
        READ_FLOAT(0x120, DISPERSION);
        READ_FLOAT(0x124, RADIO_8A9D0C);
        READ_FLOAT(0x128, CADENCIA);
        READ_FLOAT(0x1A4, TIEMPO_DETONACION);
        READ_FLOAT(0x1A8, VELOCIDAD);
        READ_FLOAT(0x1B8, ATENUACION_BOTE);
        READ_FLOAT(0x1AC, ANGULO_LANZAMIENTO);
        READ_FLOAT(0x1BC, APUNTAR_FOV);
        READ_FLOAT(0x1C0, APUNTAR_VELOCIDAD);
        READ_INT(0x130, WAVE_DISPARO);
        READ_INT(0x13C, WAVE_RECARGA);

        Hud_ReadFloatPair(component, nullptr, KEY(DECALUV1),
            reinterpret_cast<int>(WeaponDefs_GetEntry(self, entryOffset) + 320));
        Hud_ReadFloatPair(component, nullptr, KEY(DECALUV2),
            reinterpret_cast<int>(WeaponDefs_GetEntry(self, entryOffset) + 328));

        READ_ENUM(0x150, MANO, WeaponDefs_ParseHand);
        READ_OPTIONAL_FLOAT(0x1C4, PULSO_ANGULO);
        READ_OPTIONAL_FLOAT(0x1C8, PULSO_TIEMPO);
        READ_OPTIONAL_FLOAT(0x1CC, PULSO_FACTOR);
        READ_OPTIONAL_FLOAT(0x1D0, PULSO_ESTABLE_DURACION);
        READ_OPTIONAL_FLOAT(0x1D4, PULSO_ESTABLE_COOLDOWN);

        uint8_t* animations =
            FindProperty(component, nullptr, KEY(ANIMACIONES_8A9D38));
        if (animations) {
            WeaponDefs_ReadAnimationReferences(
                self, entryOffset, animations, 340, buffer,
                WeaponDefs_ParseAnimationType);
        }

        animations = FindProperty(component, nullptr, KEY(ANIMACIONES_PLAYER));
        if (animations) {
            WeaponDefs_ReadAnimationReferences(
                self, entryOffset, animations, 356, buffer,
                ParsePlayerAnimTypeName);
        }

        animations =
            FindProperty(component, nullptr, KEY(ANIMACIONES_PLAYER_TERCERA));
        if (animations) {
            WeaponDefs_ReadAnimationReferences(
                self, entryOffset, animations, 372, buffer,
                ParsePlayerAnimTypeName);
        }

        WeaponDefs_FinalizeEntry(
            reinterpret_cast<int>(WeaponDefs_GetEntry(self, entryOffset)));
    }

    return 1;
}

#undef READ_OPTIONAL_FLOAT
#undef READ_ENUM
#undef READ_STRING
#undef READ_FLOAT
#undef READ_INT
#undef KEY

HOOK(0x00788C20, WeaponDefs_LoadDefinitions, "auto");
