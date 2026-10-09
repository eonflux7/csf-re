#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0, (std::uint32_t component));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(void, __fastcall, Hud_ReadFloatPair, 0x00693A20, (void* self, void* edx, char* String1, int a3));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseWeaponType, 0x00761530, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseAutomation, 0x007615D0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseLoadingType, 0x00761620, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseRangeType, 0x00761670, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseHand, 0x007616C0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseWeight, 0x00761710, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseCharacter, 0x00761760, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseAnimationType, 0x007617B0, (char* String1));
CSF_FUNC(int, __cdecl, ParsePlayerAnimTypeName, 0x007618A0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseGrade, 0x00761B70, (char* String1));
CSF_FUNC(void, __fastcall, AnimRefList_Add, 0x00771FB0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(bool, __stdcall, WeaponDefs_FinalizeEntry, 0x00783430, (int a1));
CSF_FUNC(int, __fastcall, WeaponDefs_InitializeDefaults, 0x00783EA0, (int a1, int a2));

// Expose the 552-byte by-value record as 138 stack words, following count.
// This binding has 139 stack arguments and therefore pops exactly 556 bytes.
CSF_FUNC(int, __fastcall, WeaponDefs_Resize, 0x00784900, (
    void* self, void* edx, int count,
    uint32_t r000, uint32_t r001, uint32_t r002, uint32_t r003,
    uint32_t r004, uint32_t r005, uint32_t r006, uint32_t r007,
    uint32_t r008, uint32_t r009, uint32_t r010, uint32_t r011,
    uint32_t r012, uint32_t r013, uint32_t r014, uint32_t r015,
    uint32_t r016, uint32_t r017, uint32_t r018, uint32_t r019,
    uint32_t r020, uint32_t r021, uint32_t r022, uint32_t r023,
    uint32_t r024, uint32_t r025, uint32_t r026, uint32_t r027,
    uint32_t r028, uint32_t r029, uint32_t r030, uint32_t r031,
    uint32_t r032, uint32_t r033, uint32_t r034, uint32_t r035,
    uint32_t r036, uint32_t r037, uint32_t r038, uint32_t r039,
    uint32_t r040, uint32_t r041, uint32_t r042, uint32_t r043,
    uint32_t r044, uint32_t r045, uint32_t r046, uint32_t r047,
    uint32_t r048, uint32_t r049, uint32_t r050, uint32_t r051,
    uint32_t r052, uint32_t r053, uint32_t r054, uint32_t r055,
    uint32_t r056, uint32_t r057, uint32_t r058, uint32_t r059,
    uint32_t r060, uint32_t r061, uint32_t r062, uint32_t r063,
    uint32_t r064, uint32_t r065, uint32_t r066, uint32_t r067,
    uint32_t r068, uint32_t r069, uint32_t r070, uint32_t r071,
    uint32_t r072, uint32_t r073, uint32_t r074, uint32_t r075,
    uint32_t r076, uint32_t r077, uint32_t r078, uint32_t r079,
    uint32_t r080, uint32_t r081, uint32_t r082, uint32_t r083,
    uint32_t r084, uint32_t r085, uint32_t r086, uint32_t r087,
    uint32_t r088, uint32_t r089, uint32_t r090, uint32_t r091,
    uint32_t r092, uint32_t r093, uint32_t r094, uint32_t r095,
    uint32_t r096, uint32_t r097, uint32_t r098, uint32_t r099,
    uint32_t r100, uint32_t r101, uint32_t r102, uint32_t r103,
    uint32_t r104, uint32_t r105, uint32_t r106, uint32_t r107,
    uint32_t r108, uint32_t r109, uint32_t r110, uint32_t r111,
    uint32_t r112, uint32_t r113, uint32_t r114, uint32_t r115,
    uint32_t r116, uint32_t r117, uint32_t r118, uint32_t r119,
    uint32_t r120, uint32_t r121, uint32_t r122, uint32_t r123,
    uint32_t r124, uint32_t r125, uint32_t r126, uint32_t r127,
    uint32_t r128, uint32_t r129, uint32_t r130, uint32_t r131,
    uint32_t r132, uint32_t r133, uint32_t r134, uint32_t r135,
    uint32_t r136, uint32_t r137));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(const char*, g_pszKey_LISTADATOS_8A9C74, 0x008A9C74);
CSF_GLOBAL(const char*, g_pszKey_ID_8A9C78, 0x008A9C78);
CSF_GLOBAL(const char*, g_pszKey_NOMBRE_8A9C7C, 0x008A9C7C);
CSF_GLOBAL(const char*, g_pszKey_FLI_NOMBRE_8A9C80, 0x008A9C80);
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
CSF_GLOBAL(const char*, g_pszKey_FORCE, 0x008A9D04);
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

// The packet lists this direct callee address as a vtable reference.
[[maybe_unused]] static auto g_ChildCountTableReference = CSF_VTBL(0x00692B80);

#define KEY(name) const_cast<char*>(g_pszKey_##name)

static uint8_t* WeaponDefs_GetEntry(void* self, uint32_t byteOffset)
{
    void* records = CSF_AT(void*, self, 0x08);
    return CSF_AT(uint8_t*, records, 0x04) + byteOffset;
}

static void WeaponDefs_ReadString(
    void* self, uint32_t byteOffset, uint8_t* component,
    char* key, char* buffer, uint32_t memberOffset)
{
    MenuExtras_ReadText(component, nullptr, key, buffer, 256);
    uint8_t* entry = WeaponDefs_GetEntry(self, byteOffset);
    AssignStringRange_429B60(
        entry + memberOffset, nullptr, buffer,
        static_cast<unsigned int>(std::strlen(buffer)));
}

static void WeaponDefs_ReadAnimationList(
    void* self, uint32_t byteOffset, uint8_t* property,
    char* buffer, uint32_t memberOffset, bool playerAnimations)
{
    if (!property)
        return;

    uint32_t count = MenuExtras_GetChildCount(property, nullptr);
    for (uint32_t index = 0; index < count; ++index) {
        uint8_t* component = reinterpret_cast<uint8_t*>(
            GetComponent(property, nullptr, index));

        uint32_t reference[7] = {
            uint32_t(-1), uint32_t(-1), uint32_t(-1), 0, 0, 0, 0
        };
        int id = ReadIntegerAttribute(component, nullptr, KEY(ID_8A9C78));
        reference[0] = static_cast<uint32_t>(id);

        void* animations = CSF_AT(void*, self, 0x04);
        reference[2] = static_cast<uint32_t>(
            reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, animations, 0)[2])(animations, nullptr, id));

        MenuExtras_ReadText(component, nullptr, KEY(TIPO_8A9C8C), buffer, 256);
        reference[1] = static_cast<uint32_t>(
            playerAnimations
                ? ParsePlayerAnimTypeName(buffer)
                : WeaponDefs_ParseAnimationType(buffer));

        if (reference[0] != uint32_t(-1) &&
            reference[1] != uint32_t(-1) &&
            reference[2] != uint32_t(-1)) {
            AnimRefList_Add(
                WeaponDefs_GetEntry(self, byteOffset) + memberOffset,
                nullptr, reference);
        }
    }
}

#define READ_INT(member, key)                                                  \
    do {                                                                       \
        uint8_t* destination = WeaponDefs_GetEntry(self, byteOffset);            \
        CSF_AT(int32_t, destination, member) =                                  \
            ReadIntegerAttribute(component, nullptr, KEY(key));                \
    } while (0)

#define READ_FLOAT(member, key)                                                \
    do {                                                                       \
        uint8_t* destination = WeaponDefs_GetEntry(self, byteOffset);            \
        CSF_AT(float, destination, member) = static_cast<float>(                \
            ReadFloatAttribute(component, nullptr, KEY(key)));                 \
    } while (0)

#define READ_FLOAT_PROPERTY(member, key)                                       \
    do {                                                                       \
        uint8_t* destination = WeaponDefs_GetEntry(self, byteOffset);            \
        CSF_AT(float, destination, member) = static_cast<float>(                \
            ReadFloatProperty(component, nullptr, KEY(key)));                  \
    } while (0)

#define READ_ENUM(member, key, parser)                                         \
    do {                                                                       \
        MenuExtras_ReadText(component, nullptr, KEY(key), buffer, 256);          \
        uint8_t* destination = WeaponDefs_GetEntry(self, byteOffset);            \
        CSF_AT(int32_t, destination, member) = parser(buffer);                  \
    } while (0)

#define READ_OPTIONAL_FLOAT(member, key)                                       \
    do {                                                                       \
        uint8_t* property = FindProperty(component, nullptr, KEY(key));          \
        if (property) {                                                        \
            uint8_t* destination = WeaponDefs_GetEntry(self, byteOffset);        \
            CSF_AT(float, destination, member) = static_cast<float>(            \
                ReadComponentFloat(reinterpret_cast<uint32_t>(property)));     \
        }                                                                      \
    } while (0)

static char __fastcall WeaponDefs_LoadDefinitions(
    void* self, void* /*edx*/, S_00692b80* source)
{
    char buffer[0x108] = {};
    uint8_t* list = LookupNamedElement(
        reinterpret_cast<uint8_t*>(source), nullptr,
        g_pszKey_LISTADATOS_8A9C74);
    uint32_t count = MenuExtras_GetChildCount(list, nullptr);

    void* records = CSF_AT(void*, self, 0x08);
    alignas(4) uint8_t defaultStorage[0x228] = {};
    WeaponDefs_InitializeDefaults(reinterpret_cast<int>(defaultStorage), 0);
    const uint32_t* defaults = reinterpret_cast<const uint32_t*>(defaultStorage);

    // The callee owns destruction of the record copied into these stack words.
    WeaponDefs_Resize(
        records, nullptr, static_cast<int>(count),
        defaults[0], defaults[1], defaults[2], defaults[3],
        defaults[4], defaults[5], defaults[6], defaults[7],
        defaults[8], defaults[9], defaults[10], defaults[11],
        defaults[12], defaults[13], defaults[14], defaults[15],
        defaults[16], defaults[17], defaults[18], defaults[19],
        defaults[20], defaults[21], defaults[22], defaults[23],
        defaults[24], defaults[25], defaults[26], defaults[27],
        defaults[28], defaults[29], defaults[30], defaults[31],
        defaults[32], defaults[33], defaults[34], defaults[35],
        defaults[36], defaults[37], defaults[38], defaults[39],
        defaults[40], defaults[41], defaults[42], defaults[43],
        defaults[44], defaults[45], defaults[46], defaults[47],
        defaults[48], defaults[49], defaults[50], defaults[51],
        defaults[52], defaults[53], defaults[54], defaults[55],
        defaults[56], defaults[57], defaults[58], defaults[59],
        defaults[60], defaults[61], defaults[62], defaults[63],
        defaults[64], defaults[65], defaults[66], defaults[67],
        defaults[68], defaults[69], defaults[70], defaults[71],
        defaults[72], defaults[73], defaults[74], defaults[75],
        defaults[76], defaults[77], defaults[78], defaults[79],
        defaults[80], defaults[81], defaults[82], defaults[83],
        defaults[84], defaults[85], defaults[86], defaults[87],
        defaults[88], defaults[89], defaults[90], defaults[91],
        defaults[92], defaults[93], defaults[94], defaults[95],
        defaults[96], defaults[97], defaults[98], defaults[99],
        defaults[100], defaults[101], defaults[102], defaults[103],
        defaults[104], defaults[105], defaults[106], defaults[107],
        defaults[108], defaults[109], defaults[110], defaults[111],
        defaults[112], defaults[113], defaults[114], defaults[115],
        defaults[116], defaults[117], defaults[118], defaults[119],
        defaults[120], defaults[121], defaults[122], defaults[123],
        defaults[124], defaults[125], defaults[126], defaults[127],
        defaults[128], defaults[129], defaults[130], defaults[131],
        defaults[132], defaults[133], defaults[134], defaults[135],
        defaults[136], defaults[137]);

    uint32_t byteOffset = 0;
    for (uint32_t index = 0; index < count; ++index, byteOffset += 552) {
        uint8_t* component =
            reinterpret_cast<uint8_t*>(GetComponent(list, nullptr, index));

        READ_INT(0x000, ID_8A9C78);
        WeaponDefs_ReadString(
            self, byteOffset, component, KEY(NOMBRE_8A9C7C), buffer, 4);
        WeaponDefs_ReadString(
            self, byteOffset, component, KEY(FLI_NOMBRE_8A9C80), buffer, 32);
        WeaponDefs_ReadString(
            self, byteOffset, component, KEY(FILE_8A9C84), buffer, 60);
        WeaponDefs_ReadString(
            self, byteOffset, component, KEY(FILE2), buffer, 88);

        READ_ENUM(0x074, TIPO_8A9C8C, WeaponDefs_ParseWeaponType);
        READ_ENUM(0x07C, AUTOMATISMO, WeaponDefs_ParseAutomation);

        if (FindProperty(component, nullptr, KEY(GRADO_8A9C98)))
            READ_ENUM(0x080, GRADO_8A9C98, WeaponDefs_ParseGrade);
        if (FindProperty(component, nullptr, KEY(PESO)))
            READ_ENUM(0x0DC, PESO, WeaponDefs_ParseWeight);
        if (FindProperty(component, nullptr, KEY(PERSONAJE)))
            READ_ENUM(0x0E0, PERSONAJE, WeaponDefs_ParseCharacter);

        uint8_t* force = FindProperty(component, nullptr, KEY(FORCE));
        if (force) {
            uint8_t* destination = WeaponDefs_GetEntry(self, byteOffset);
            CSF_AT(int32_t, destination, 0x11C) =
                ParseIntegerComponent(reinterpret_cast<uint32_t>(force));
        }

        WeaponDefs_ReadString(
            self, byteOffset, component, KEY(FOGONAZO_DFF), buffer, 136);
        WeaponDefs_ReadString(
            self, byteOffset, component, KEY(FOGONAZO_TEXTURE), buffer, 164);
        READ_INT(0x0C0, FOGONAZO_COLOR);
        READ_FLOAT(0x0C4, FOGONAZO_DURACION);
        READ_FLOAT(0x0C8, FOGONAZO_RADIO);
        READ_FLOAT_PROPERTY(0x0CC, FOGONAZO_ANCHO);
        {
            uint8_t* entry = WeaponDefs_GetEntry(self, byteOffset);
            if (CSF_AT(float, entry, 0x0CC) == g_fZero)
                CSF_AT(float, entry, 0x0CC) = 15.0f;
        }
        READ_FLOAT_PROPERTY(0x0D0, FOGONAZO_ALTO);
        {
            uint8_t* entry = WeaponDefs_GetEntry(self, byteOffset);
            if (CSF_AT(float, entry, 0x0D0) == g_fZero)
                CSF_AT(float, entry, 0x0D0) = 15.0f;
        }

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

        Hud_ReadFloatPair(
            component, nullptr, KEY(DECALUV1),
            reinterpret_cast<int>(WeaponDefs_GetEntry(self, byteOffset) + 320));
        Hud_ReadFloatPair(
            component, nullptr, KEY(DECALUV2),
            reinterpret_cast<int>(WeaponDefs_GetEntry(self, byteOffset) + 328));
        READ_ENUM(0x150, MANO, WeaponDefs_ParseHand);

        READ_OPTIONAL_FLOAT(0x1C4, PULSO_ANGULO);
        READ_OPTIONAL_FLOAT(0x1C8, PULSO_TIEMPO);
        READ_OPTIONAL_FLOAT(0x1CC, PULSO_FACTOR);
        READ_OPTIONAL_FLOAT(0x1D0, PULSO_ESTABLE_DURACION);
        READ_OPTIONAL_FLOAT(0x1D4, PULSO_ESTABLE_COOLDOWN);

        uint8_t* animations =
            FindProperty(component, nullptr, KEY(ANIMACIONES_8A9D38));
        WeaponDefs_ReadAnimationList(
            self, byteOffset, animations, buffer, 340, false);

        uint8_t* playerAnimations =
            FindProperty(component, nullptr, KEY(ANIMACIONES_PLAYER));
        WeaponDefs_ReadAnimationList(
            self, byteOffset, playerAnimations, buffer, 356, true);

        uint8_t* thirdPersonAnimations =
            FindProperty(component, nullptr, KEY(ANIMACIONES_PLAYER_TERCERA));
        WeaponDefs_ReadAnimationList(
            self, byteOffset, thirdPersonAnimations, buffer, 372, true);

        WeaponDefs_FinalizeEntry(
            reinterpret_cast<int>(WeaponDefs_GetEntry(self, byteOffset)));
    }

    return 1;
}

#undef READ_OPTIONAL_FLOAT
#undef READ_ENUM
#undef READ_FLOAT_PROPERTY
#undef READ_FLOAT
#undef READ_INT
#undef KEY

HOOK(0x00789850, WeaponDefs_LoadDefinitions, "auto");
