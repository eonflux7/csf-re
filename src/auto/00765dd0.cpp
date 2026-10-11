#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0, (std::uint32_t component));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40, (std::uint8_t* attribute, void* edx, char* buffer, int capacity));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __fastcall, DataNode_ReadVector3Property, 0x00693A60, (void* self, void* edx, char* String1, int a3));
CSF_FUNC(int, __cdecl, CharacterDefs_ParseType, 0x007619E0, (char* String1));
CSF_FUNC(int, __cdecl, CharacterDefs_ParseSubtype, 0x00761A30, (char* String1));
CSF_FUNC(int, __cdecl, CharacterDefs_ParseBehavior, 0x00761A80, (char* String1));
CSF_FUNC(int, __cdecl, CharacterDefs_ParseCollisionModelType, 0x00761AD0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseGrade, 0x00761B70, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParsePhysicsType, 0x00761D50, (char* String1));
CSF_FUNC(char, __fastcall, CharacterDefs_ParseAnimations, 0x007621F0, (void* self, void* edx, void* a2, int a3, void* a4, char* String1));
CSF_FUNC(char*, __fastcall, CharacterDefs_InitializeCharacterData, 0x00762650, (char* self, void* edx));

// The original consumes a count followed by a 556-byte record by value.
// Spell out its 139 stack words so the binding exposes all 560 popped bytes.
CSF_FUNC(int, __fastcall, CharacterDefs_ResizeRecords, 0x007632E0,
    (void* self, void* edx, unsigned int requestedCount,
     int word0, int word1, int word2, int word3, int word4,
     int word5, int word6, int word7, int word8, int word9,
     int word10, int word11, int word12, int word13, int word14,
     int word15, int word16, int word17, int word18, int word19,
     int word20, int word21, int word22, int word23, int word24,
     int word25, int word26, int word27, int word28, int word29,
     int word30, int word31, int word32, int word33, int word34,
     int word35, int word36, int word37, int word38, int word39,
     int word40, int word41, int word42, int word43, int word44,
     int word45, int word46, int word47, int word48, int word49,
     int word50, int word51, int word52, int word53, int word54,
     int word55, int word56, int word57, int word58, int word59,
     int word60, int word61, int word62, int word63, int word64,
     int word65, int word66, int word67, int word68, int word69,
     int word70, int word71, int word72, int word73, int word74,
     int word75, int word76, int word77, int word78, int word79,
     int word80, int word81, int word82, int word83, int word84,
     int word85, int word86, int word87, int word88, int word89,
     int word90, int word91, int word92, int word93, int word94,
     int word95, int word96, int word97, int word98, int word99,
     int word100, int word101, int word102, int word103, int word104,
     int word105, int word106, int word107, int word108, int word109,
     int word110, int word111, int word112, int word113, int word114,
     int word115, int word116, int word117, int word118, int word119,
     int word120, int word121, int word122, int word123, int word124,
     int word125, int word126, int word127, int word128, int word129,
     int word130, int word131, int word132, int word133, int word134,
     int word135, int word136, int word137, int word138));

CSF_GLOBAL(const char*, g_pszKey_LISTADATOS_8A9804, 0x008A9804);
CSF_GLOBAL(const char*, g_pszKey_ID_8A9808, 0x008A9808);
CSF_GLOBAL(const char*, g_pszKey_REF_ID, 0x008A980C);
CSF_GLOBAL(const char*, g_pszKey_REF_ID2, 0x008A9810);
CSF_GLOBAL(const char*, g_pszKey_NOMBRE_8A9818, 0x008A9818);
CSF_GLOBAL(const char*, g_pszKey_TIPO_8A9820, 0x008A9820);
CSF_GLOBAL(const char*, g_pszKey_SUBTIPO, 0x008A9824);
CSF_GLOBAL(const char*, g_pszKey_GRADO, 0x008A9828);
CSF_GLOBAL(const char*, g_pszKey_MODELO, 0x008A9830);
CSF_GLOBAL(const char*, g_pszKey_MODELO_TERCERA, 0x008A9834);
CSF_GLOBAL(const char*, g_pszKey_LOD1_NOMBRE, 0x008A9838);
CSF_GLOBAL(const char*, g_pszKey_LOD2_NOMBRE, 0x008A983C);
CSF_GLOBAL(const char*, g_pszKey_LOD1_DIST, 0x008A9840);
CSF_GLOBAL(const char*, g_pszKey_LOD2_DIST, 0x008A9844);
CSF_GLOBAL(const char*, g_pszKey_DAMAGE, 0x008A9848);
CSF_GLOBAL(const char*, g_pszKey_MODELO_COLISION, 0x008A984C);
CSF_GLOBAL(const char*, g_pszKey_MODO_COLISION, 0x008A9850);
CSF_GLOBAL(const char*, g_pszKey_TAM, 0x008A9858);
CSF_GLOBAL(const char*, g_pszKey_BBOX_8A985C, 0x008A985C);
CSF_GLOBAL(const char*, g_pszKey_INF, 0x008A9860);
CSF_GLOBAL(const char*, g_pszKey_SUP, 0x008A9864);
CSF_GLOBAL(const char*, g_pszKey_EXTERNAL_SHAPE, 0x008A9868);
CSF_GLOBAL(const char*, g_pszKey_EXTERNAL_SHAPE_OFFSET, 0x008A986C);
CSF_GLOBAL(const char*, g_pszKey_CO_MODEL_EX, 0x008A9870);
CSF_GLOBAL(const char*, g_pszKey_COMPOR, 0x008A9874);
CSF_GLOBAL(const char*, g_pszKey_VIDA_8A9878, 0x008A9878);
CSF_GLOBAL(const char*, g_pszKey_ANIMACIONES, 0x008A9880);
CSF_GLOBAL(const char*, g_pszKey_ALTURA_CABEZA, 0x008A9884);
CSF_GLOBAL(const char*, g_pszKey_ARMAS_8A9888, 0x008A9888);
CSF_GLOBAL(const char*, g_pszKey_ITEMS_8A988C, 0x008A988C);
CSF_GLOBAL(const char*, g_pszKey_ANGULO_VISTA, 0x008A9890);
CSF_GLOBAL(const char*, g_pszKey_DISTANCIA_VISTA, 0x008A9894);
CSF_GLOBAL(const char*, g_pszKey_DISTANCIA_LARGA_VISTA, 0x008A9898);
CSF_GLOBAL(const char*, g_pszKey_PORCENTAJE, 0x008A989C);
CSF_GLOBAL(const char*, g_pszKey_WAV_CALL_REPARTIDOR, 0x008A98A0);
CSF_GLOBAL(const char*, g_pszKey_VEHICLE_INFO, 0x008A98AC);
CSF_GLOBAL(const char*, g_pszKey_VEH_MAX_VEL, 0x008A98B0);
CSF_GLOBAL(const char*, g_pszKey_VEH_MAX_BACK_VEL, 0x008A98B4);
CSF_GLOBAL(const char*, g_pszKey_VEH_MAX_ACCEL, 0x008A98B8);
CSF_GLOBAL(const char*, g_pszKey_VEH_MAX_DECEL, 0x008A98BC);
CSF_GLOBAL(const char*, g_pszKey_OCLUSOR, 0x008A98DC);
CSF_GLOBAL(const char*, g_pszKey_IGNORE_PVS, 0x008A98E0);
CSF_GLOBAL(const char*, g_pszKey_IGNORE_BSPHERE, 0x008A98E4);
CSF_GLOBAL(const char*, g_pszKey_VEHICLE_VIDA_CHASIS, 0x008A98E8);
CSF_GLOBAL(const char*, g_pszKey_VEHICLE_VIDA_MOTOR, 0x008A98EC);
CSF_GLOBAL(const char*, g_pszKey_VEHICLE_VIDA_RUEDAS, 0x008A98F0);
CSF_GLOBAL(const char*, g_pszKey_VEHICLE_VIDA_BAJOS, 0x008A98F4);
CSF_GLOBAL(const char*, g_pszKey_PHYSIC, 0x008A98F8);
CSF_GLOBAL(const char*, g_pszKey_MODEL_FILE, 0x008A98FC);
CSF_GLOBAL(const char*, g_pszKey_DYNAMIC, 0x008A9900);
CSF_GLOBAL(const char*, g_pszKey_MOVING, 0x008A9904);
CSF_GLOBAL(const char*, g_pszKey_MASS, 0x008A9908);
CSF_GLOBAL(const char*, g_pszKey_BOUNCE, 0x008A990C);
CSF_GLOBAL(const char*, g_pszKey_SLIDE, 0x008A9910);

// Select the variant's container, falling back to the original parent.
static std::uint8_t* CharacterDefs_SelectContainer(
    std::uint8_t* parent, const char* key, char* variant)
{
    auto* property = FindProperty(parent, nullptr, const_cast<char*>(key));
    if (property)
    {
        auto* selected = FindProperty(property, nullptr, variant);
        if (selected)
            return selected;
    }
    return parent;
}

// Optional values have a different nesting rule: variant -> key -> value.
static std::uint8_t* CharacterDefs_FindOptionalValue(
    std::uint8_t* parent, const char* key, char* variant)
{
    auto* property = FindProperty(parent, nullptr, const_cast<char*>(key));
    if (!property)
        return nullptr;
    auto* selected = FindProperty(property, nullptr, variant);
    return selected
        ? FindProperty(selected, nullptr, const_cast<char*>(key))
        : property;
}

static void CharacterDefs_AssignBufferedString(void* destination, char* buffer)
{
    AssignStringRange_429B60(
        destination, nullptr, buffer,
        static_cast<unsigned int>(std::strlen(buffer)));
}

static void CharacterDefs_GrowIntegerArray(
    std::uint8_t* array, std::uint32_t newCapacity, std::uint32_t growth)
{
    void* replacement = Mem_Alloc(4 * newCapacity);
    const std::uint32_t count = CSF_AT(std::uint32_t, array, 0x8);
    if (count)
        std::memcpy(replacement, CSF_AT(void*, array, 0x0), 4 * count);

    void* oldStorage = CSF_AT(void*, array, 0x0);
    CSF_AT(std::uint32_t, array, 0x4) += growth;
    if (oldStorage)
    {
        Mem_Free(oldStorage);
        CSF_AT(void*, array, 0x0) = nullptr;
    }
    CSF_AT(void*, array, 0x0) = replacement;
}

static void CharacterDefs_AppendInteger(std::uint8_t* array, int value)
{
    const std::uint32_t count = CSF_AT(std::uint32_t, array, 0x8);
    const std::uint32_t capacity = CSF_AT(std::uint32_t, array, 0x4);
    if (count >= capacity)
    {
        const std::uint32_t growth = CSF_AT(std::uint32_t, array, 0xC);
        CharacterDefs_GrowIntegerArray(array, growth + capacity, growth);
    }
    const std::uint32_t index = CSF_AT(std::uint32_t, array, 0x8);
    int* destination = CSF_AT(int*, array, 0x0) + index;
    CSF_AT(std::uint32_t, array, 0x8) = index + 1;
    *destination = value;
}

static int __fastcall C_0086f128_ParseCharacterDefinitions(
    C_0086f128* self, void* /*edx*/, int* records,
    S_00692b80* source, char* variant)
{
    // Packet annotations identify direct callees, not object vptr stores.
    (void)CSF_VTBL(0x00692B80);
    (void)CSF_VTBL(0x0041E5F0);

    char buffer[0x110] = {};
    auto* list = LookupNamedElement(
        reinterpret_cast<std::uint8_t*>(source), nullptr,
        g_pszKey_LISTADATOS_8A9804);
    const std::uint32_t recordCount = MenuExtras_GetChildCount(list, nullptr);

    int initialValue[556 / sizeof(int)] = {};
    CharacterDefs_InitializeCharacterData(
        reinterpret_cast<char*>(initialValue), nullptr);
    CharacterDefs_ResizeRecords(
        records, nullptr, recordCount,
        initialValue[0], initialValue[1], initialValue[2], initialValue[3], initialValue[4],
        initialValue[5], initialValue[6], initialValue[7], initialValue[8], initialValue[9],
        initialValue[10], initialValue[11], initialValue[12], initialValue[13], initialValue[14],
        initialValue[15], initialValue[16], initialValue[17], initialValue[18], initialValue[19],
        initialValue[20], initialValue[21], initialValue[22], initialValue[23], initialValue[24],
        initialValue[25], initialValue[26], initialValue[27], initialValue[28], initialValue[29],
        initialValue[30], initialValue[31], initialValue[32], initialValue[33], initialValue[34],
        initialValue[35], initialValue[36], initialValue[37], initialValue[38], initialValue[39],
        initialValue[40], initialValue[41], initialValue[42], initialValue[43], initialValue[44],
        initialValue[45], initialValue[46], initialValue[47], initialValue[48], initialValue[49],
        initialValue[50], initialValue[51], initialValue[52], initialValue[53], initialValue[54],
        initialValue[55], initialValue[56], initialValue[57], initialValue[58], initialValue[59],
        initialValue[60], initialValue[61], initialValue[62], initialValue[63], initialValue[64],
        initialValue[65], initialValue[66], initialValue[67], initialValue[68], initialValue[69],
        initialValue[70], initialValue[71], initialValue[72], initialValue[73], initialValue[74],
        initialValue[75], initialValue[76], initialValue[77], initialValue[78], initialValue[79],
        initialValue[80], initialValue[81], initialValue[82], initialValue[83], initialValue[84],
        initialValue[85], initialValue[86], initialValue[87], initialValue[88], initialValue[89],
        initialValue[90], initialValue[91], initialValue[92], initialValue[93], initialValue[94],
        initialValue[95], initialValue[96], initialValue[97], initialValue[98], initialValue[99],
        initialValue[100], initialValue[101], initialValue[102], initialValue[103], initialValue[104],
        initialValue[105], initialValue[106], initialValue[107], initialValue[108], initialValue[109],
        initialValue[110], initialValue[111], initialValue[112], initialValue[113], initialValue[114],
        initialValue[115], initialValue[116], initialValue[117], initialValue[118], initialValue[119],
        initialValue[120], initialValue[121], initialValue[122], initialValue[123], initialValue[124],
        initialValue[125], initialValue[126], initialValue[127], initialValue[128], initialValue[129],
        initialValue[130], initialValue[131], initialValue[132], initialValue[133], initialValue[134],
        initialValue[135], initialValue[136], initialValue[137], initialValue[138]);

    for (std::uint32_t index = 0; index < recordCount; ++index)
    {
        auto* component = reinterpret_cast<std::uint8_t*>(
            GetComponent(list, nullptr, index));
        auto* record = reinterpret_cast<std::uint8_t*>(
            CSF_AT(std::uint32_t, records, 0x4) + 556 * index);
        std::uint8_t* property;

#define SELECT(parent, key) CharacterDefs_SelectContainer(parent, key, variant)
#define OPTIONAL(parent, key) CharacterDefs_FindOptionalValue(parent, key, variant)
#define READ_TEXT(parent, key) \
        do { \
            auto* container = SELECT(parent, key); \
            MenuExtras_ReadText(container, nullptr, const_cast<char*>(key), buffer, 256); \
        } while (false)
#define READ_INT_ATTRIBUTE(parent, key, offset) \
        do { \
            auto* container = SELECT(parent, key); \
            CSF_AT(int, record, offset) = ReadIntegerAttribute( \
                container, nullptr, const_cast<char*>(key)); \
        } while (false)
#define READ_INT_PROPERTY(parent, key, offset) \
        do { \
            auto* container = SELECT(parent, key); \
            CSF_AT(int, record, offset) = ReadIntegerProperty( \
                container, nullptr, const_cast<char*>(key)); \
        } while (false)
#define READ_FLOAT_PROPERTY(parent, key, offset) \
        do { \
            auto* container = SELECT(parent, key); \
            CSF_AT(float, record, offset) = static_cast<float>(ReadFloatProperty( \
                container, nullptr, const_cast<char*>(key))); \
        } while (false)
#define READ_VECTOR(parent, key, destination) \
        do { \
            auto* container = SELECT(parent, key); \
            DataNode_ReadVector3Property(container, nullptr, const_cast<char*>(key), \
                reinterpret_cast<int>(destination)); \
        } while (false)

        CSF_AT(int, record, 0x0) = ReadIntegerAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_ID_8A9808));
        READ_INT_ATTRIBUTE(component, g_pszKey_REF_ID, 0xC0);
        property = FindProperty(
            component, nullptr, const_cast<char*>(g_pszKey_REF_ID2));
        if (property)
            CSF_AT(int, record, 0xC4) =
                ParseIntegerComponent(reinterpret_cast<std::uint32_t>(property));

        READ_TEXT(component, g_pszKey_NOMBRE_8A9818);
        CharacterDefs_AssignBufferedString(record + 32, buffer);
        READ_TEXT(component, g_pszKey_TIPO_8A9820);
        CSF_AT(int, record, 0xB4) = CharacterDefs_ParseType(buffer);

        property = OPTIONAL(component, g_pszKey_SUBTIPO);
        if (property)
        {
            CopyAttributeText(property, nullptr, buffer, 256);
            CSF_AT(int, record, 0xB8) = CharacterDefs_ParseSubtype(buffer);
        }
        property = OPTIONAL(component, g_pszKey_GRADO);
        if (property)
        {
            CopyAttributeText(property, nullptr, buffer, 256);
            CSF_AT(int, record, 0xBC) = WeaponDefs_ParseGrade(buffer);
        }

        READ_TEXT(component, g_pszKey_MODELO);
        CharacterDefs_AssignBufferedString(record + 60, buffer);
        READ_TEXT(component, g_pszKey_MODELO_TERCERA);
        CharacterDefs_AssignBufferedString(record + 88, buffer);
        property = OPTIONAL(component, g_pszKey_LOD1_NOMBRE);
        if (property)
        {
            CopyAttributeText(property, nullptr, buffer, 256);
            CharacterDefs_AssignBufferedString(record + 116, buffer);
        }
        property = OPTIONAL(component, g_pszKey_LOD2_NOMBRE);
        if (property)
        {
            CopyAttributeText(property, nullptr, buffer, 256);
            CharacterDefs_AssignBufferedString(record + 144, buffer);
        }
        property = OPTIONAL(component, g_pszKey_LOD1_DIST);
        if (property)
            CSF_AT(float, record, 0xAC) = static_cast<float>(
                ReadComponentFloat(reinterpret_cast<std::uint32_t>(property)));
        property = OPTIONAL(component, g_pszKey_LOD2_DIST);
        if (property)
            CSF_AT(float, record, 0xB0) = static_cast<float>(
                ReadComponentFloat(reinterpret_cast<std::uint32_t>(property)));

        property = OPTIONAL(component, g_pszKey_OCLUSOR);
        CSF_AT(std::uint8_t, record, 0x158) = property &&
            ParseIntegerComponent(reinterpret_cast<std::uint32_t>(property)) != 0;
        property = OPTIONAL(component, g_pszKey_IGNORE_PVS);
        CSF_AT(std::uint8_t, record, 0x159) = property &&
            ParseIntegerComponent(reinterpret_cast<std::uint32_t>(property)) != 0;
        property = OPTIONAL(component, g_pszKey_IGNORE_BSPHERE);
        CSF_AT(std::uint8_t, record, 0x15A) = property &&
            ParseIntegerComponent(reinterpret_cast<std::uint32_t>(property)) != 0;

        property = OPTIONAL(component, g_pszKey_ALTURA_CABEZA);
        auto coordinate = GetComponent(property, nullptr, 0);
        CSF_AT(float, record, 0xE4) =
            static_cast<float>(ReadComponentFloat(coordinate));
        coordinate = GetComponent(property, nullptr, 1u);
        CSF_AT(float, record, 0xE8) =
            static_cast<float>(ReadComponentFloat(coordinate));
        coordinate = GetComponent(property, nullptr, 2u);
        CSF_AT(float, record, 0xEC) =
            static_cast<float>(ReadComponentFloat(coordinate));

        property = OPTIONAL(component, g_pszKey_ANGULO_VISTA);
        CSF_AT(float, record, 0x120) = property
            ? static_cast<float>(ReadComponentFloat(
                reinterpret_cast<std::uint32_t>(property))) : 120.0f;
        property = OPTIONAL(component, g_pszKey_DISTANCIA_VISTA);
        CSF_AT(float, record, 0x124) = property
            ? static_cast<float>(ReadComponentFloat(
                reinterpret_cast<std::uint32_t>(property))) : 2000.0f;
        property = OPTIONAL(component, g_pszKey_WAV_CALL_REPARTIDOR);
        if (property)
            CSF_AT(int, record, 0x148) =
                ParseIntegerComponent(reinterpret_cast<std::uint32_t>(property));
        property = OPTIONAL(component, g_pszKey_DISTANCIA_LARGA_VISTA);
        CSF_AT(float, record, 0x128) = property
            ? static_cast<float>(ReadComponentFloat(
                reinterpret_cast<std::uint32_t>(property))) : 5000.0f;
        property = OPTIONAL(component, g_pszKey_PORCENTAJE);
        if (property)
            CSF_AT(int, record, 0x12C) =
                ParseIntegerComponent(reinterpret_cast<std::uint32_t>(property));

        auto* damage = LookupNamedElement(component, nullptr, g_pszKey_DAMAGE);
        READ_INT_ATTRIBUTE(damage, g_pszKey_VIDA_8A9878, 0xDC);

        auto* bounds = FindProperty(
            component, nullptr, const_cast<char*>(g_pszKey_BBOX_8A985C));
        if (bounds)
        {
            std::uint32_t lower[0x2C / 4] = {};
            std::uint32_t upper[3] = {};
            READ_VECTOR(bounds, g_pszKey_INF, lower);
            READ_VECTOR(bounds, g_pszKey_SUP, upper);
            CSF_AT(std::uint32_t, record, 0x130) = upper[0];
            CSF_AT(std::uint32_t, record, 0x134) = upper[1];
            CSF_AT(std::uint32_t, record, 0x138) = upper[2];
            CSF_AT(std::uint32_t, record, 0x13C) = lower[0];
            CSF_AT(std::uint32_t, record, 0x140) = lower[1];
            CSF_AT(std::uint32_t, record, 0x144) = lower[2];
        }

        auto* collision = LookupNamedElement(
            component, nullptr, g_pszKey_MODELO_COLISION);
        READ_TEXT(collision, g_pszKey_TIPO_8A9820);
        CSF_AT(int, record, 0xCC) = CharacterDefs_ParseCollisionModelType(buffer);
        READ_VECTOR(collision, g_pszKey_TAM, record + 352);
        if (FindProperty(collision, nullptr,
                         const_cast<char*>(g_pszKey_EXTERNAL_SHAPE)))
        {
            READ_INT_ATTRIBUTE(collision, g_pszKey_EXTERNAL_SHAPE, 0x188);
        }
        else
        {
            CSF_AT(int, record, 0x188) = 1;
        }
        if (FindProperty(collision, nullptr,
                         const_cast<char*>(g_pszKey_EXTERNAL_SHAPE_OFFSET)))
        {
            READ_VECTOR(collision, g_pszKey_EXTERNAL_SHAPE_OFFSET, record + 396);
        }
        else
        {
            CSF_AT(std::uint32_t, record, 0x18C) = 0;
            CSF_AT(std::uint32_t, record, 0x190) = 0;
            CSF_AT(std::uint32_t, record, 0x194) = 0;
        }
        if (CSF_AT(int, record, 0xCC) == 1)
        {
            READ_TEXT(collision, g_pszKey_CO_MODEL_EX);
            CharacterDefs_AssignBufferedString(record + 364, buffer);
        }

        auto* vehicle = FindProperty(
            component, nullptr, const_cast<char*>(g_pszKey_VEHICLE_INFO));
        if (vehicle)
        {
            READ_FLOAT_PROPERTY(vehicle, g_pszKey_VEH_MAX_VEL, 0x1B4);
            READ_FLOAT_PROPERTY(vehicle, g_pszKey_VEH_MAX_BACK_VEL, 0x1B8);
            READ_FLOAT_PROPERTY(vehicle, g_pszKey_VEH_MAX_ACCEL, 0x1BC);
            READ_FLOAT_PROPERTY(vehicle, g_pszKey_VEH_MAX_DECEL, 0x1C0);
            READ_INT_PROPERTY(vehicle, g_pszKey_VEHICLE_VIDA_CHASIS, 0x1C4);
            READ_INT_PROPERTY(vehicle, g_pszKey_VEHICLE_VIDA_MOTOR, 0x1C8);
            READ_INT_PROPERTY(vehicle, g_pszKey_VEHICLE_VIDA_RUEDAS, 0x1CC);
            READ_INT_PROPERTY(vehicle, g_pszKey_VEHICLE_VIDA_BAJOS, 0x1D0);
        }

        READ_INT_PROPERTY(component, g_pszKey_MODO_COLISION, 0xD0);
        if (CSF_AT(int, record, 0xD0) == 3)
            CSF_AT(int, record, 0xD0) = 0;

        auto* physics = FindProperty(
            component, nullptr, const_cast<char*>(g_pszKey_PHYSIC));
        if (physics)
        {
            READ_TEXT(physics, g_pszKey_TIPO_8A9820);
            CSF_AT(int, record, 0x1F0) = WeaponDefs_ParsePhysicsType(buffer);
            READ_TEXT(physics, g_pszKey_MODEL_FILE);
            CharacterDefs_AssignBufferedString(record + 500, buffer);
            property = SELECT(physics, g_pszKey_DYNAMIC);
            CSF_AT(std::uint8_t, record, 0x21C) = ReadIntegerProperty(
                property, nullptr, const_cast<char*>(g_pszKey_DYNAMIC)) != 0;
            property = SELECT(physics, g_pszKey_MOVING);
            CSF_AT(std::uint8_t, record, 0x21D) = ReadIntegerProperty(
                property, nullptr, const_cast<char*>(g_pszKey_MOVING)) != 0;
            READ_FLOAT_PROPERTY(physics, g_pszKey_MASS, 0x220);
            READ_FLOAT_PROPERTY(physics, g_pszKey_BOUNCE, 0x224);
            READ_FLOAT_PROPERTY(physics, g_pszKey_SLIDE, 0x228);
        }

        READ_TEXT(component, g_pszKey_COMPOR);
        CSF_AT(int, record, 0xD8) = CharacterDefs_ParseBehavior(buffer);
        property = FindProperty(
            component, nullptr, const_cast<char*>(g_pszKey_ANIMACIONES));
        CharacterDefs_ParseAnimations(
            self, nullptr, property, reinterpret_cast<int>(record),
            record + 240, variant);

        auto* weapons = OPTIONAL(component, g_pszKey_ARMAS_8A9888);
        if (weapons)
        {
            const std::uint32_t count = MenuExtras_GetChildCount(weapons, nullptr);
            const std::uint32_t capacity = CSF_AT(std::uint32_t, record, 0x104);
            if (count > capacity)
                GrowButtonArray(record + 256, nullptr, static_cast<int>(count - capacity));
            for (std::uint32_t weaponIndex = 0; weaponIndex < count; ++weaponIndex)
            {
                const auto child = GetComponent(weapons, nullptr, weaponIndex);
                const int value = ParseIntegerComponent(child);
                CharacterDefs_AppendInteger(record + 256, value);
            }
        }

        auto* items = OPTIONAL(component, g_pszKey_ITEMS_8A988C);
        if (items)
        {
            const std::uint32_t count = MenuExtras_GetChildCount(items, nullptr);
            const std::uint32_t capacity = CSF_AT(std::uint32_t, record, 0x114);
            if (count > capacity)
                CharacterDefs_GrowIntegerArray(record + 272, count, count - capacity);
            for (std::uint32_t itemIndex = 0; itemIndex < count; ++itemIndex)
            {
                const auto child = GetComponent(items, nullptr, itemIndex);
                const int value = ParseIntegerComponent(child);
                CharacterDefs_AppendInteger(record + 272, value);
            }
        }

#undef SELECT
#undef OPTIONAL
#undef READ_TEXT
#undef READ_INT_ATTRIBUTE
#undef READ_INT_PROPERTY
#undef READ_FLOAT_PROPERTY
#undef READ_VECTOR
    }
    return 1;
}

HOOK(0x00765DD0, C_0086f128_ParseCharacterDefinitions, "auto");
