#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

struct WeaponDefRecord {
    std::uint32_t data[138];
};
static_assert(sizeof(WeaponDefRecord) == 552);

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0, (std::uint32_t component));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40, (std::uint8_t* attribute, void* edx, char* buffer, int capacity));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(void, __fastcall, Hud_ReadFloatPair, 0x00693A20, (void* self, void* edx, char* String1, int a3));
CSF_FUNC(int, __fastcall, DataNode_ReadVector3Property, 0x00693A60, (void* self, void* edx, char* String1, int a3));

CSF_FUNC(int, __cdecl, WeaponDefs_ParseWeaponType, 0x00761530, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseAimType, 0x00761580, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseAutomaticMode, 0x007615D0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseLoadingType, 0x00761620, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseRangeType, 0x00761670, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseHand, 0x007616C0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseWeightType, 0x00761710, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseCharacterType, 0x00761760, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseAnimationType, 0x007617B0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseBotAnimationType, 0x00761850, (char* String1));
CSF_FUNC(int, __cdecl, ParsePlayerAnimTypeName, 0x007618A0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseMultiplayerAnimationType, 0x007618F0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseGrade, 0x00761B70, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParsePhysicsType, 0x00761D50, (char* String1));

CSF_FUNC(void, __fastcall, AnimRefList_Add, 0x00771FB0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(bool, __stdcall, WeaponDefs_FinalizeRecord, 0x00783430, (int a1));
CSF_FUNC(int, __fastcall, WeaponDefs_InitializeDefaults, 0x00783EA0, (int a1, int a2));

// Flatten the trivially copied record into its 138 stack dwords.
// count plus the 552-byte record gives the original ret 556 ABI.
CSF_FUNC(int, __fastcall, WeaponDefs_Resize, 0x00784900, (
    void* self, void* edx, int count,
    std::uint32_t word000, std::uint32_t word001,
    std::uint32_t word002, std::uint32_t word003,
    std::uint32_t word004, std::uint32_t word005,
    std::uint32_t word006, std::uint32_t word007,
    std::uint32_t word008, std::uint32_t word009,
    std::uint32_t word010, std::uint32_t word011,
    std::uint32_t word012, std::uint32_t word013,
    std::uint32_t word014, std::uint32_t word015,
    std::uint32_t word016, std::uint32_t word017,
    std::uint32_t word018, std::uint32_t word019,
    std::uint32_t word020, std::uint32_t word021,
    std::uint32_t word022, std::uint32_t word023,
    std::uint32_t word024, std::uint32_t word025,
    std::uint32_t word026, std::uint32_t word027,
    std::uint32_t word028, std::uint32_t word029,
    std::uint32_t word030, std::uint32_t word031,
    std::uint32_t word032, std::uint32_t word033,
    std::uint32_t word034, std::uint32_t word035,
    std::uint32_t word036, std::uint32_t word037,
    std::uint32_t word038, std::uint32_t word039,
    std::uint32_t word040, std::uint32_t word041,
    std::uint32_t word042, std::uint32_t word043,
    std::uint32_t word044, std::uint32_t word045,
    std::uint32_t word046, std::uint32_t word047,
    std::uint32_t word048, std::uint32_t word049,
    std::uint32_t word050, std::uint32_t word051,
    std::uint32_t word052, std::uint32_t word053,
    std::uint32_t word054, std::uint32_t word055,
    std::uint32_t word056, std::uint32_t word057,
    std::uint32_t word058, std::uint32_t word059,
    std::uint32_t word060, std::uint32_t word061,
    std::uint32_t word062, std::uint32_t word063,
    std::uint32_t word064, std::uint32_t word065,
    std::uint32_t word066, std::uint32_t word067,
    std::uint32_t word068, std::uint32_t word069,
    std::uint32_t word070, std::uint32_t word071,
    std::uint32_t word072, std::uint32_t word073,
    std::uint32_t word074, std::uint32_t word075,
    std::uint32_t word076, std::uint32_t word077,
    std::uint32_t word078, std::uint32_t word079,
    std::uint32_t word080, std::uint32_t word081,
    std::uint32_t word082, std::uint32_t word083,
    std::uint32_t word084, std::uint32_t word085,
    std::uint32_t word086, std::uint32_t word087,
    std::uint32_t word088, std::uint32_t word089,
    std::uint32_t word090, std::uint32_t word091,
    std::uint32_t word092, std::uint32_t word093,
    std::uint32_t word094, std::uint32_t word095,
    std::uint32_t word096, std::uint32_t word097,
    std::uint32_t word098, std::uint32_t word099,
    std::uint32_t word100, std::uint32_t word101,
    std::uint32_t word102, std::uint32_t word103,
    std::uint32_t word104, std::uint32_t word105,
    std::uint32_t word106, std::uint32_t word107,
    std::uint32_t word108, std::uint32_t word109,
    std::uint32_t word110, std::uint32_t word111,
    std::uint32_t word112, std::uint32_t word113,
    std::uint32_t word114, std::uint32_t word115,
    std::uint32_t word116, std::uint32_t word117,
    std::uint32_t word118, std::uint32_t word119,
    std::uint32_t word120, std::uint32_t word121,
    std::uint32_t word122, std::uint32_t word123,
    std::uint32_t word124, std::uint32_t word125,
    std::uint32_t word126, std::uint32_t word127,
    std::uint32_t word128, std::uint32_t word129,
    std::uint32_t word130, std::uint32_t word131,
    std::uint32_t word132, std::uint32_t word133,
    std::uint32_t word134, std::uint32_t word135,
    std::uint32_t word136, std::uint32_t word137));

static auto g_DataNodeVtable = CSF_VTBL(0x00692B80);

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(const char*, g_pszKey_LISTADATOS_8A9C74, 0x008A9C74);
CSF_GLOBAL(const char*, g_pszKey_ID_8A9C78, 0x008A9C78);
CSF_GLOBAL(const char*, g_pszKey_NOMBRE_8A9C7C, 0x008A9C7C);
CSF_GLOBAL(const char*, g_pszKey_FLI_NOMBRE_8A9C80, 0x008A9C80);
CSF_GLOBAL(const char*, g_pszKey_FILE_8A9C84, 0x008A9C84);
CSF_GLOBAL(const char*, g_pszKey_FILE2, 0x008A9C88);
CSF_GLOBAL(const char*, g_pszKey_TIPO_8A9C8C, 0x008A9C8C);
CSF_GLOBAL(const char*, g_pszKey_PUNTERIA_8A9C90, 0x008A9C90);
CSF_GLOBAL(const char*, g_pszKey_AUTOMATISMO, 0x008A9C94);
CSF_GLOBAL(const char*, g_pszKey_GRADO_8A9C98, 0x008A9C98);
CSF_GLOBAL(const char*, g_pszKey_ORDEN, 0x008A9C9C);
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
CSF_GLOBAL(const char*, g_pszKey_NODROP, 0x008A9CCC);
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
CSF_GLOBAL(const char*, g_pszKey_WAVE_DISPARO_SIN_BALAS, 0x008A9D1C);
CSF_GLOBAL(const char*, g_pszKey_WAVE_DISPARO_PLAYER, 0x008A9D20);
CSF_GLOBAL(const char*, g_pszKey_WAVE_RECARGA, 0x008A9D24);
CSF_GLOBAL(const char*, g_pszKey_DECALUV1, 0x008A9D2C);
CSF_GLOBAL(const char*, g_pszKey_DECALUV2, 0x008A9D30);
CSF_GLOBAL(const char*, g_pszKey_MANO, 0x008A9D34);
CSF_GLOBAL(const char*, g_pszKey_ANIMACIONES_8A9D38, 0x008A9D38);
CSF_GLOBAL(const char*, g_pszKey_ANIMACIONES_PLAYER, 0x008A9D3C);
CSF_GLOBAL(const char*, g_pszKey_ANIMACIONES_PLAYER_TERCERA, 0x008A9D40);
CSF_GLOBAL(const char*, g_pszKey_ANIMACIONES_MULTIPLAYER, 0x008A9D44);
CSF_GLOBAL(const char*, g_pszKey_ANIMACIONES_BOT, 0x008A9D48);
CSF_GLOBAL(const char*, g_pszKey_TIEMPO_DETONACION, 0x008A9D4C);
CSF_GLOBAL(const char*, g_pszKey_VELOCIDAD, 0x008A9D50);
CSF_GLOBAL(const char*, g_pszKey_ANGULO_LANZAMIENTO, 0x008A9D54);
CSF_GLOBAL(const char*, g_pszKey_VELOCIDAD_CORTO, 0x008A9D58);
CSF_GLOBAL(const char*, g_pszKey_ANGULO_LANZAMIENTO_CORTO, 0x008A9D5C);
CSF_GLOBAL(const char*, g_pszKey_ATENUACION_BOTE, 0x008A9D60);
CSF_GLOBAL(const char*, g_pszKey_APUNTAR_FOV, 0x008A9D64);
CSF_GLOBAL(const char*, g_pszKey_APUNTAR_VELOCIDAD, 0x008A9D68);
CSF_GLOBAL(const char*, g_pszKey_ANGULO_PUNTO_MIRA, 0x008A9D6C);
CSF_GLOBAL(const char*, g_pszKey_PULSO_ANGULO, 0x008A9D70);
CSF_GLOBAL(const char*, g_pszKey_PULSO_TIEMPO, 0x008A9D74);
CSF_GLOBAL(const char*, g_pszKey_PULSO_FACTOR, 0x008A9D78);
CSF_GLOBAL(const char*, g_pszKey_PULSO_ESTABLE_DURACION, 0x008A9D7C);
CSF_GLOBAL(const char*, g_pszKey_PULSO_ESTABLE_COOLDOWN, 0x008A9D80);
CSF_GLOBAL(const char*, g_pszKey_PHYSIC_8A9D84, 0x008A9D84);
CSF_GLOBAL(const char*, g_pszKey_MODEL_FILE_8A9D88, 0x008A9D88);
CSF_GLOBAL(const char*, g_pszKey_MASS_8A9D8C, 0x008A9D8C);
CSF_GLOBAL(const char*, g_pszKey_BOUNCE_8A9D90, 0x008A9D90);
CSF_GLOBAL(const char*, g_pszKey_SLIDE_8A9D94, 0x008A9D94);
CSF_GLOBAL(const char*, g_pszKey_BBOX_8A9D98, 0x008A9D98);
CSF_GLOBAL(const char*, g_pszKey_INF_8A9D9C, 0x008A9D9C);
CSF_GLOBAL(const char*, g_pszKey_SUP_8A9DA0, 0x008A9DA0);

#define KEY(name) const_cast<char*>(g_pszKey_##name)

static int CWeaponDefTable_FindOwnerAnimation(CWeaponDefTable* self, int id)
{
    void* owner = reinterpret_cast<void*>(self->m_owner);
    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, owner, 0)[8 / 4])(owner, nullptr, id);
}

static void CWeaponDefTable_ReadAnimationList(
    CWeaponDefTable* self, std::uint8_t* list, void* destination,
    char* buffer, int (__cdecl* parseType)(char*))
{
    const std::uint32_t count = MenuExtras_GetChildCount(list, nullptr);
    for (std::uint32_t i = 0; i < count; ++i) {
        auto* entry = reinterpret_cast<std::uint8_t*>(
            GetComponent(list, nullptr, i));
        std::uint32_t reference[0x28 / 4] = {};
        reference[0] = reference[1] = reference[2] = static_cast<std::uint32_t>(-1);
        reference[0] = ReadIntegerAttribute(entry, nullptr, KEY(ID_8A9C78));
        reference[2] = CWeaponDefTable_FindOwnerAnimation(
            self, static_cast<int>(reference[0]));
        MenuExtras_ReadText(entry, nullptr, KEY(TIPO_8A9C8C), buffer, 256);
        reference[1] = parseType(buffer);
        if (reference[0] != static_cast<std::uint32_t>(-1) &&
            reference[1] != static_cast<std::uint32_t>(-1) &&
            reference[2] != static_cast<std::uint32_t>(-1)) {
            AnimRefList_Add(destination, nullptr, reference);
        }
    }
}

static void CWeaponDefTable_ReadVariantAnimationList(
    CWeaponDefTable* self, void* records, std::uint32_t recordOffset,
    std::uint8_t* list, std::uint32_t listOffset, char* variant,
    char* buffer, int (__cdecl* parseType)(char*))
{
    const std::uint32_t count = MenuExtras_GetChildCount(list, nullptr);
    for (std::uint32_t i = 0; i < count; ++i) {
        auto* entry = reinterpret_cast<std::uint8_t*>(
            GetComponent(list, nullptr, i));
        auto* record = CSF_AT(std::uint8_t*, records, 4) + recordOffset;
        std::uint32_t reference[0x28 / 4] = {};
        reference[0] = reference[1] = reference[2] = static_cast<std::uint32_t>(-1);
        auto* idProperty = FindProperty(entry, nullptr, KEY(ID_8A9C78));
        std::uint8_t* selected = nullptr;
        if (idProperty)
            selected = FindProperty(idProperty, nullptr, variant);
        if (!selected)
            selected = entry;
        const int id = ReadIntegerAttribute(selected, nullptr, KEY(ID_8A9C78));
        reference[0] = id;
        if (id != -1) {
            reference[2] = CWeaponDefTable_FindOwnerAnimation(self, id);
            MenuExtras_ReadText(entry, nullptr, KEY(TIPO_8A9C8C), buffer, 256);
            reference[1] = parseType(buffer);
            if (reference[0] != static_cast<std::uint32_t>(-1) &&
                reference[1] != static_cast<std::uint32_t>(-1) &&
                reference[2] != static_cast<std::uint32_t>(-1)) {
                AnimRefList_Add(record + listOffset, nullptr, reference);
            }
        }
    }
}

static char __fastcall CWeaponDefTable_ParseVersion13(
    CWeaponDefTable* self, void* /*edx*/, int recordsAddress,
    S_00692b80* root, char* variant)
{
    char buffer[0x108];
    std::memset(buffer, 0, 256);
    auto* list = LookupNamedElement(
        reinterpret_cast<std::uint8_t*>(root), nullptr,
        g_pszKey_LISTADATOS_8A9C74);
    const std::uint32_t count = MenuExtras_GetChildCount(list, nullptr);

    // Resize consumes and destroys the stack copy of this record.
    // Zero initialization supplies zero for unspecified padding.
    WeaponDefRecord defaults = {};
    WeaponDefs_InitializeDefaults(reinterpret_cast<int>(&defaults), 0);
    void* records = reinterpret_cast<void*>(recordsAddress);
    WeaponDefs_Resize(records, nullptr, static_cast<int>(count),
        defaults.data[0], defaults.data[1],
        defaults.data[2], defaults.data[3],
        defaults.data[4], defaults.data[5],
        defaults.data[6], defaults.data[7],
        defaults.data[8], defaults.data[9],
        defaults.data[10], defaults.data[11],
        defaults.data[12], defaults.data[13],
        defaults.data[14], defaults.data[15],
        defaults.data[16], defaults.data[17],
        defaults.data[18], defaults.data[19],
        defaults.data[20], defaults.data[21],
        defaults.data[22], defaults.data[23],
        defaults.data[24], defaults.data[25],
        defaults.data[26], defaults.data[27],
        defaults.data[28], defaults.data[29],
        defaults.data[30], defaults.data[31],
        defaults.data[32], defaults.data[33],
        defaults.data[34], defaults.data[35],
        defaults.data[36], defaults.data[37],
        defaults.data[38], defaults.data[39],
        defaults.data[40], defaults.data[41],
        defaults.data[42], defaults.data[43],
        defaults.data[44], defaults.data[45],
        defaults.data[46], defaults.data[47],
        defaults.data[48], defaults.data[49],
        defaults.data[50], defaults.data[51],
        defaults.data[52], defaults.data[53],
        defaults.data[54], defaults.data[55],
        defaults.data[56], defaults.data[57],
        defaults.data[58], defaults.data[59],
        defaults.data[60], defaults.data[61],
        defaults.data[62], defaults.data[63],
        defaults.data[64], defaults.data[65],
        defaults.data[66], defaults.data[67],
        defaults.data[68], defaults.data[69],
        defaults.data[70], defaults.data[71],
        defaults.data[72], defaults.data[73],
        defaults.data[74], defaults.data[75],
        defaults.data[76], defaults.data[77],
        defaults.data[78], defaults.data[79],
        defaults.data[80], defaults.data[81],
        defaults.data[82], defaults.data[83],
        defaults.data[84], defaults.data[85],
        defaults.data[86], defaults.data[87],
        defaults.data[88], defaults.data[89],
        defaults.data[90], defaults.data[91],
        defaults.data[92], defaults.data[93],
        defaults.data[94], defaults.data[95],
        defaults.data[96], defaults.data[97],
        defaults.data[98], defaults.data[99],
        defaults.data[100], defaults.data[101],
        defaults.data[102], defaults.data[103],
        defaults.data[104], defaults.data[105],
        defaults.data[106], defaults.data[107],
        defaults.data[108], defaults.data[109],
        defaults.data[110], defaults.data[111],
        defaults.data[112], defaults.data[113],
        defaults.data[114], defaults.data[115],
        defaults.data[116], defaults.data[117],
        defaults.data[118], defaults.data[119],
        defaults.data[120], defaults.data[121],
        defaults.data[122], defaults.data[123],
        defaults.data[124], defaults.data[125],
        defaults.data[126], defaults.data[127],
        defaults.data[128], defaults.data[129],
        defaults.data[130], defaults.data[131],
        defaults.data[132], defaults.data[133],
        defaults.data[134], defaults.data[135],
        defaults.data[136], defaults.data[137]);

    for (std::uint32_t index = 0; index < count; ++index) {
        auto* component = reinterpret_cast<std::uint8_t*>(
            GetComponent(list, nullptr, index));
        const std::uint32_t recordOffset = 552 * index;
        auto* record = CSF_AT(std::uint8_t*, records, 4) + recordOffset;

#define READ_TEXT(name) \
        MenuExtras_ReadText(component, nullptr, KEY(name), buffer, 256)
#define READ_STRING(name, offset) \
        do { \
            READ_TEXT(name); \
            AssignStringRange_429B60(record + offset, nullptr, buffer, \
                static_cast<unsigned int>(std::strlen(buffer))); \
        } while (false)
#define READ_ENUM(name, offset, parser) \
        do { \
            READ_TEXT(name); \
            CSF_AT(int, record, offset) = parser(buffer); \
        } while (false)
#define READ_INT(name, offset) \
        CSF_AT(int, record, offset) = \
            ReadIntegerAttribute(component, nullptr, KEY(name))
#define READ_FLOAT(name, offset) \
        CSF_AT(float, record, offset) = \
            static_cast<float>(ReadFloatAttribute(component, nullptr, KEY(name)))
#define READ_OPTIONAL_FLOAT(name, offset) \
        do { \
            auto* property = FindProperty(component, nullptr, KEY(name)); \
            if (property) \
                CSF_AT(float, record, offset) = static_cast<float>( \
                    ReadComponentFloat(reinterpret_cast<std::uint32_t>(property))); \
        } while (false)
#define READ_OPTIONAL_INT(name, offset) \
        do { \
            auto* property = FindProperty(component, nullptr, KEY(name)); \
            if (property) \
                CSF_AT(int, record, offset) = ParseIntegerComponent( \
                    reinterpret_cast<std::uint32_t>(property)); \
        } while (false)

        READ_INT(ID_8A9C78, 0x000);
        READ_STRING(NOMBRE_8A9C7C, 0x004);
        READ_STRING(FLI_NOMBRE_8A9C80, 0x020);
        READ_STRING(FILE_8A9C84, 0x03C);
        READ_STRING(FILE2, 0x058);
        READ_ENUM(TIPO_8A9C8C, 0x074, WeaponDefs_ParseWeaponType);

        if (auto* property = FindProperty(component, nullptr, KEY(PUNTERIA_8A9C90))) {
            CopyAttributeText(property, nullptr, buffer, 256);
            CSF_AT(int, record, 0x078) = WeaponDefs_ParseAimType(buffer);
        }
        READ_ENUM(AUTOMATISMO, 0x07C, WeaponDefs_ParseAutomaticMode);
        if (FindProperty(component, nullptr, KEY(GRADO_8A9C98)))
            READ_ENUM(GRADO_8A9C98, 0x080, WeaponDefs_ParseGrade);
        if (FindProperty(component, nullptr, KEY(ORDEN)))
            READ_INT(ORDEN, 0x084);
        if (FindProperty(component, nullptr, KEY(PESO)))
            READ_ENUM(PESO, 0x0DC, WeaponDefs_ParseWeightType);
        if (FindProperty(component, nullptr, KEY(PERSONAJE)))
            READ_ENUM(PERSONAJE, 0x0E0, WeaponDefs_ParseCharacterType);
        if (FindProperty(component, nullptr, KEY(NODROP))) {
            CSF_AT(std::uint8_t, record, 0x0E4) =
                ReadIntegerAttribute(component, nullptr, KEY(NODROP)) != 0;
        }
        READ_OPTIONAL_INT(FORCE, 0x11C);

        READ_STRING(FOGONAZO_DFF, 0x088);
        READ_STRING(FOGONAZO_TEXTURE, 0x0A4);
        READ_INT(FOGONAZO_COLOR, 0x0C0);
        READ_FLOAT(FOGONAZO_DURACION, 0x0C4);
        READ_FLOAT(FOGONAZO_RADIO, 0x0C8);
        const double flashWidth =
            ReadFloatProperty(component, nullptr, KEY(FOGONAZO_ANCHO));
        CSF_AT(float, record, 0x0CC) = static_cast<float>(flashWidth);
        if (flashWidth == g_fZero)
            CSF_AT(float, record, 0x0CC) = 15.0f;
        const double flashHeight =
            ReadFloatProperty(component, nullptr, KEY(FOGONAZO_ALTO));
        CSF_AT(float, record, 0x0D0) = static_cast<float>(flashHeight);
        if (flashHeight == g_fZero)
            CSF_AT(float, record, 0x0D0) = 15.0f;

        READ_ENUM(CARGA, 0x0D4, WeaponDefs_ParseLoadingType);
        READ_ENUM(ALCANCE, 0x0D8, WeaponDefs_ParseRangeType);
        READ_FLOAT(ALCANCE_RANGO_MIN, 0x0E8);
        READ_FLOAT(ALCANCE_RANGO_MAX, 0x0EC);
        READ_FLOAT(LIMITE_ANGULAR_HORIZONTAL, 0x0F0);
        READ_FLOAT(LIMITE_ANGULAR_VERTICAL, 0x0F4);
        READ_FLOAT(DESVIACION_H, 0x0F8);
        READ_FLOAT(DESVIACION_V, 0x0FC);
        READ_FLOAT(DESVIACION_FA, 0x100);
        READ_INT(BALAS, 0x104);
        READ_INT(CARGADORES, 0x108);
        READ_INT(MAX_CARGADORES, 0x10C);
        READ_INT(BALAS_DISPONIBLES, 0x110);
        READ_INT(MAX_BALAS_DISPONIBLES, 0x114);
        READ_INT(DAMAGE_8A9D00, 0x118);
        READ_FLOAT(DISPERSION, 0x120);
        READ_FLOAT(RADIO_8A9D0C, 0x124);
        READ_FLOAT(CADENCIA, 0x128);
        READ_FLOAT(TIEMPO_DETONACION, 0x1A4);
        READ_FLOAT(VELOCIDAD, 0x1A8);
        READ_FLOAT(ANGULO_LANZAMIENTO, 0x1AC);
        READ_FLOAT(ATENUACION_BOTE, 0x1B8);
        READ_FLOAT(APUNTAR_FOV, 0x1BC);
        READ_FLOAT(APUNTAR_VELOCIDAD, 0x1C0);
        READ_OPTIONAL_FLOAT(VELOCIDAD_CORTO, 0x1B0);
        READ_OPTIONAL_FLOAT(ANGULO_LANZAMIENTO_CORTO, 0x1B4);
        READ_OPTIONAL_FLOAT(ANGULO_PUNTO_MIRA, 0x12C);
        READ_INT(WAVE_DISPARO, 0x130);
        READ_INT(WAVE_RECARGA, 0x13C);
        READ_OPTIONAL_INT(WAVE_DISPARO_SIN_BALAS, 0x134);
        READ_OPTIONAL_INT(WAVE_DISPARO_PLAYER, 0x138);
        Hud_ReadFloatPair(component, nullptr, KEY(DECALUV1),
                          reinterpret_cast<int>(record + 320));
        Hud_ReadFloatPair(component, nullptr, KEY(DECALUV2),
                          reinterpret_cast<int>(record + 328));
        READ_ENUM(MANO, 0x150, WeaponDefs_ParseHand);
        READ_OPTIONAL_FLOAT(PULSO_ANGULO, 0x1C4);
        READ_OPTIONAL_FLOAT(PULSO_TIEMPO, 0x1C8);
        READ_OPTIONAL_FLOAT(PULSO_FACTOR, 0x1CC);
        READ_OPTIONAL_FLOAT(PULSO_ESTABLE_DURACION, 0x1D0);
        READ_OPTIONAL_FLOAT(PULSO_ESTABLE_COOLDOWN, 0x1D4);

        if (auto* animations = FindProperty(component, nullptr, KEY(ANIMACIONES_8A9D38))) {
            CWeaponDefTable_ReadAnimationList(
                self, animations, record + 340, buffer,
                WeaponDefs_ParseAnimationType);
        }
        if (auto* animations = FindProperty(component, nullptr, KEY(ANIMACIONES_PLAYER))) {
            CWeaponDefTable_ReadAnimationList(
                self, animations, record + 356, buffer,
                ParsePlayerAnimTypeName);
        }
        if (auto* animations = FindProperty(component, nullptr, KEY(ANIMACIONES_PLAYER_TERCERA))) {
            CWeaponDefTable_ReadVariantAnimationList(
                self, records, recordOffset, animations, 372,
                variant, buffer, ParsePlayerAnimTypeName);
        }
        if (auto* animations = FindProperty(component, nullptr, KEY(ANIMACIONES_MULTIPLAYER))) {
            CWeaponDefTable_ReadVariantAnimationList(
                self, records, recordOffset, animations, 388,
                variant, buffer, WeaponDefs_ParseMultiplayerAnimationType);
        }
        if (auto* animations = FindProperty(component, nullptr, KEY(ANIMACIONES_BOT))) {
            CWeaponDefTable_ReadVariantAnimationList(
                self, records, recordOffset, animations, 404,
                variant, buffer, WeaponDefs_ParseBotAnimationType);
        }

        if (auto* bounds = FindProperty(component, nullptr, KEY(BBOX_8A9D98))) {
            std::uint32_t lower[3] = {};
            std::uint32_t upper[3] = {};
            DataNode_ReadVector3Property(
                bounds, nullptr, KEY(INF_8A9D9C), reinterpret_cast<int>(lower));
            DataNode_ReadVector3Property(
                bounds, nullptr, KEY(SUP_8A9DA0), reinterpret_cast<int>(upper));
            CSF_AT(std::uint32_t, record, 0x1D8) = upper[0];
            CSF_AT(std::uint32_t, record, 0x1DC) = upper[1];
            CSF_AT(std::uint32_t, record, 0x1E0) = upper[2];
            CSF_AT(std::uint32_t, record, 0x1E4) = lower[0];
            CSF_AT(std::uint32_t, record, 0x1E8) = lower[1];
            CSF_AT(std::uint32_t, record, 0x1EC) = lower[2];
        }
        if (auto* physics = FindProperty(component, nullptr, KEY(PHYSIC_8A9D84))) {
            MenuExtras_ReadText(physics, nullptr, KEY(TIPO_8A9C8C), buffer, 256);
            CSF_AT(int, record, 0x1F0) = WeaponDefs_ParsePhysicsType(buffer);
            MenuExtras_ReadText(physics, nullptr, KEY(MODEL_FILE_8A9D88), buffer, 256);
            AssignStringRange_429B60(
                record + 500, nullptr, buffer,
                static_cast<unsigned int>(std::strlen(buffer)));
            CSF_AT(float, record, 0x21C) = static_cast<float>(
                ReadFloatProperty(physics, nullptr, KEY(MASS_8A9D8C)));
            CSF_AT(float, record, 0x220) = static_cast<float>(
                ReadFloatProperty(physics, nullptr, KEY(BOUNCE_8A9D90)));
            CSF_AT(float, record, 0x224) = static_cast<float>(
                ReadFloatProperty(physics, nullptr, KEY(SLIDE_8A9D94)));
        }
        WeaponDefs_FinalizeRecord(reinterpret_cast<int>(record));
    }
    return 1;
}

#undef READ_OPTIONAL_INT
#undef READ_OPTIONAL_FLOAT
#undef READ_FLOAT
#undef READ_INT
#undef READ_ENUM
#undef READ_STRING
#undef READ_TEXT
#undef KEY

HOOK(0x0078A4F0, CWeaponDefTable_ParseVersion13, "auto");
