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
CSF_FUNC(int, __cdecl, CharacterDefs_ParseCharacterType, 0x007619E0, (char* String1));
CSF_FUNC(int, __cdecl, CharacterDefs_ParseCharacterSubtype, 0x00761A30, (char* String1));
CSF_FUNC(int, __cdecl, CharacterDefs_ParseBehavior, 0x00761A80, (char* String1));
CSF_FUNC(int, __cdecl, CharacterDefs_ParseCollisionModelType, 0x00761AD0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseGrade, 0x00761B70, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParsePhysicsType, 0x00761D50, (char* String1));
CSF_FUNC(char, __fastcall, CharacterDefs_ReadAnimations, 0x007621F0, (void* self, void* edx, void* a2, int a3, void* a4, char* String1));
CSF_FUNC(int, __fastcall, CharacterDefs_InitializeCharacterData, 0x00762650, (int a1, int a2));

// The by-value record occupies 556 bytes. Expose its 139 stack words
// explicitly so the binding describes the complete ret 560 ABI.
CSF_FUNC(int, __fastcall, CharacterDefs_ResizeRecords, 0x007632E0,
    (void* self, void* edx, unsigned int requestedCount,
     std::uint32_t word0, std::uint32_t word1,
     std::uint32_t word2, std::uint32_t word3,
     std::uint32_t word4, std::uint32_t word5,
     std::uint32_t word6, std::uint32_t word7,
     std::uint32_t word8, std::uint32_t word9,
     std::uint32_t word10, std::uint32_t word11,
     std::uint32_t word12, std::uint32_t word13,
     std::uint32_t word14, std::uint32_t word15,
     std::uint32_t word16, std::uint32_t word17,
     std::uint32_t word18, std::uint32_t word19,
     std::uint32_t word20, std::uint32_t word21,
     std::uint32_t word22, std::uint32_t word23,
     std::uint32_t word24, std::uint32_t word25,
     std::uint32_t word26, std::uint32_t word27,
     std::uint32_t word28, std::uint32_t word29,
     std::uint32_t word30, std::uint32_t word31,
     std::uint32_t word32, std::uint32_t word33,
     std::uint32_t word34, std::uint32_t word35,
     std::uint32_t word36, std::uint32_t word37,
     std::uint32_t word38, std::uint32_t word39,
     std::uint32_t word40, std::uint32_t word41,
     std::uint32_t word42, std::uint32_t word43,
     std::uint32_t word44, std::uint32_t word45,
     std::uint32_t word46, std::uint32_t word47,
     std::uint32_t word48, std::uint32_t word49,
     std::uint32_t word50, std::uint32_t word51,
     std::uint32_t word52, std::uint32_t word53,
     std::uint32_t word54, std::uint32_t word55,
     std::uint32_t word56, std::uint32_t word57,
     std::uint32_t word58, std::uint32_t word59,
     std::uint32_t word60, std::uint32_t word61,
     std::uint32_t word62, std::uint32_t word63,
     std::uint32_t word64, std::uint32_t word65,
     std::uint32_t word66, std::uint32_t word67,
     std::uint32_t word68, std::uint32_t word69,
     std::uint32_t word70, std::uint32_t word71,
     std::uint32_t word72, std::uint32_t word73,
     std::uint32_t word74, std::uint32_t word75,
     std::uint32_t word76, std::uint32_t word77,
     std::uint32_t word78, std::uint32_t word79,
     std::uint32_t word80, std::uint32_t word81,
     std::uint32_t word82, std::uint32_t word83,
     std::uint32_t word84, std::uint32_t word85,
     std::uint32_t word86, std::uint32_t word87,
     std::uint32_t word88, std::uint32_t word89,
     std::uint32_t word90, std::uint32_t word91,
     std::uint32_t word92, std::uint32_t word93,
     std::uint32_t word94, std::uint32_t word95,
     std::uint32_t word96, std::uint32_t word97,
     std::uint32_t word98, std::uint32_t word99,
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
     std::uint32_t word136, std::uint32_t word137,
     std::uint32_t word138));

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
CSF_GLOBAL(const char*, g_pszKey_VEHICLE_INFO, 0x008A98AC);
CSF_GLOBAL(const char*, g_pszKey_VEH_MAX_VEL, 0x008A98B0);
CSF_GLOBAL(const char*, g_pszKey_VEH_MAX_BACK_VEL, 0x008A98B4);
CSF_GLOBAL(const char*, g_pszKey_VEH_MAX_ACCEL, 0x008A98B8);
CSF_GLOBAL(const char*, g_pszKey_VEH_MAX_DECEL, 0x008A98BC);
CSF_GLOBAL(const char*, g_pszKey_OCLUSOR, 0x008A98DC);
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

// These packet vtable labels coincide with directly called function addresses.
[[maybe_unused]] static const auto g_ChildCountVtableLabel = CSF_VTBL(0x00692B80);
[[maybe_unused]] static const auto g_ArrayGrowthVtableLabel = CSF_VTBL(0x0041E5F0);

#define KEY(name) const_cast<char*>(g_pszKey_##name)

static std::uint8_t* CharacterDefs_SelectLocalizedParent(
    std::uint8_t* node, char* key, char* locale)
{
    std::uint8_t* property = FindProperty(node, nullptr, key);
    if (property)
    {
        std::uint8_t* localized = FindProperty(property, nullptr, locale);
        if (localized)
            return localized;
    }
    return node;
}

static std::uint8_t* CharacterDefs_SelectLocalizedAttribute(
    std::uint8_t* node, char* key, char* locale)
{
    std::uint8_t* property = FindProperty(node, nullptr, key);
    if (!property)
        return nullptr;
    std::uint8_t* localized = FindProperty(property, nullptr, locale);
    return localized ? FindProperty(localized, nullptr, key) : property;
}

static int __fastcall C_0086f128_ReadCharacterRecords(
    C_0086f128* self, void* /*edx*/, int* records,
    S_00692b80* source, char* locale)
{
    char buffer[0x10C]{};
    std::uint8_t* list = LookupNamedElement(
        reinterpret_cast<std::uint8_t*>(source), nullptr, KEY(LISTADATOS_8A9804));
    const unsigned int recordCount = MenuExtras_GetChildCount(list, nullptr);

    std::uint32_t initialValue[556 / sizeof(std::uint32_t)]{};
    CharacterDefs_InitializeCharacterData(reinterpret_cast<int>(initialValue), 0);
    CharacterDefs_ResizeRecords(
        records, nullptr, recordCount,
        initialValue[0], initialValue[1], initialValue[2], initialValue[3],
        initialValue[4], initialValue[5], initialValue[6], initialValue[7],
        initialValue[8], initialValue[9], initialValue[10], initialValue[11],
        initialValue[12], initialValue[13], initialValue[14], initialValue[15],
        initialValue[16], initialValue[17], initialValue[18], initialValue[19],
        initialValue[20], initialValue[21], initialValue[22], initialValue[23],
        initialValue[24], initialValue[25], initialValue[26], initialValue[27],
        initialValue[28], initialValue[29], initialValue[30], initialValue[31],
        initialValue[32], initialValue[33], initialValue[34], initialValue[35],
        initialValue[36], initialValue[37], initialValue[38], initialValue[39],
        initialValue[40], initialValue[41], initialValue[42], initialValue[43],
        initialValue[44], initialValue[45], initialValue[46], initialValue[47],
        initialValue[48], initialValue[49], initialValue[50], initialValue[51],
        initialValue[52], initialValue[53], initialValue[54], initialValue[55],
        initialValue[56], initialValue[57], initialValue[58], initialValue[59],
        initialValue[60], initialValue[61], initialValue[62], initialValue[63],
        initialValue[64], initialValue[65], initialValue[66], initialValue[67],
        initialValue[68], initialValue[69], initialValue[70], initialValue[71],
        initialValue[72], initialValue[73], initialValue[74], initialValue[75],
        initialValue[76], initialValue[77], initialValue[78], initialValue[79],
        initialValue[80], initialValue[81], initialValue[82], initialValue[83],
        initialValue[84], initialValue[85], initialValue[86], initialValue[87],
        initialValue[88], initialValue[89], initialValue[90], initialValue[91],
        initialValue[92], initialValue[93], initialValue[94], initialValue[95],
        initialValue[96], initialValue[97], initialValue[98], initialValue[99],
        initialValue[100], initialValue[101], initialValue[102], initialValue[103],
        initialValue[104], initialValue[105], initialValue[106], initialValue[107],
        initialValue[108], initialValue[109], initialValue[110], initialValue[111],
        initialValue[112], initialValue[113], initialValue[114], initialValue[115],
        initialValue[116], initialValue[117], initialValue[118], initialValue[119],
        initialValue[120], initialValue[121], initialValue[122], initialValue[123],
        initialValue[124], initialValue[125], initialValue[126], initialValue[127],
        initialValue[128], initialValue[129], initialValue[130], initialValue[131],
        initialValue[132], initialValue[133], initialValue[134], initialValue[135],
        initialValue[136], initialValue[137], initialValue[138]);

    for (unsigned int index = 0; index < recordCount; ++index)
    {
        std::uint8_t* node = reinterpret_cast<std::uint8_t*>(
            GetComponent(list, nullptr, index));
        std::uint8_t* record =
            CSF_AT(std::uint8_t*, records, 0x4) + 556 * index;

        CSF_AT(int, record, 0x0) =
            ReadIntegerAttribute(node, nullptr, KEY(ID_8A9808));

        std::uint8_t* selected =
            CharacterDefs_SelectLocalizedParent(node, KEY(REF_ID), locale);
        CSF_AT(int, record, 0xC0) =
            ReadIntegerAttribute(selected, nullptr, KEY(REF_ID));

        selected = FindProperty(node, nullptr, KEY(REF_ID2));
        if (selected)
            CSF_AT(int, record, 0xC4) =
                ParseIntegerComponent(reinterpret_cast<std::uint32_t>(selected));

        selected = CharacterDefs_SelectLocalizedParent(node, KEY(NOMBRE_8A9818), locale);
        MenuExtras_ReadText(selected, nullptr, KEY(NOMBRE_8A9818), buffer, 256);
        AssignStringRange_429B60(
            record + 32, nullptr, buffer, static_cast<unsigned int>(std::strlen(buffer)));

        selected = CharacterDefs_SelectLocalizedParent(node, KEY(TIPO_8A9820), locale);
        MenuExtras_ReadText(selected, nullptr, KEY(TIPO_8A9820), buffer, 256);
        CSF_AT(int, record, 0xB4) = CharacterDefs_ParseCharacterType(buffer);

        selected = CharacterDefs_SelectLocalizedAttribute(node, KEY(SUBTIPO), locale);
        if (selected)
        {
            CopyAttributeText(selected, nullptr, buffer, 256);
            CSF_AT(int, record, 0xB8) = CharacterDefs_ParseCharacterSubtype(buffer);
        }

        selected = CharacterDefs_SelectLocalizedAttribute(node, KEY(GRADO), locale);
        if (selected)
        {
            CopyAttributeText(selected, nullptr, buffer, 256);
            CSF_AT(int, record, 0xBC) = WeaponDefs_ParseGrade(buffer);
        }

        selected = CharacterDefs_SelectLocalizedParent(node, KEY(MODELO), locale);
        MenuExtras_ReadText(selected, nullptr, KEY(MODELO), buffer, 256);
        AssignStringRange_429B60(
            record + 60, nullptr, buffer, static_cast<unsigned int>(std::strlen(buffer)));

        selected = CharacterDefs_SelectLocalizedParent(node, KEY(MODELO_TERCERA), locale);
        MenuExtras_ReadText(selected, nullptr, KEY(MODELO_TERCERA), buffer, 256);
        AssignStringRange_429B60(
            record + 88, nullptr, buffer, static_cast<unsigned int>(std::strlen(buffer)));

        selected = CharacterDefs_SelectLocalizedAttribute(node, KEY(OCLUSOR), locale);
        CSF_AT(std::uint8_t, record, 0x158) =
            selected && ParseIntegerComponent(reinterpret_cast<std::uint32_t>(selected)) != 0;

        selected = CharacterDefs_SelectLocalizedAttribute(node, KEY(ALTURA_CABEZA), locale);
        std::uint32_t component = GetComponent(selected, nullptr, 0);
        CSF_AT(float, record, 0xE4) = static_cast<float>(ReadComponentFloat(component));
        component = GetComponent(selected, nullptr, 1u);
        CSF_AT(float, record, 0xE8) = static_cast<float>(ReadComponentFloat(component));
        component = GetComponent(selected, nullptr, 2u);
        CSF_AT(float, record, 0xEC) = static_cast<float>(ReadComponentFloat(component));

        selected = CharacterDefs_SelectLocalizedAttribute(node, KEY(ANGULO_VISTA), locale);
        CSF_AT(float, record, 0x120) = selected
            ? static_cast<float>(ReadComponentFloat(reinterpret_cast<std::uint32_t>(selected)))
            : 120.0f;

        selected = CharacterDefs_SelectLocalizedAttribute(node, KEY(DISTANCIA_VISTA), locale);
        CSF_AT(float, record, 0x124) = selected
            ? static_cast<float>(ReadComponentFloat(reinterpret_cast<std::uint32_t>(selected)))
            : 2000.0f;

        selected = CharacterDefs_SelectLocalizedAttribute(node, KEY(DISTANCIA_LARGA_VISTA), locale);
        CSF_AT(float, record, 0x128) = selected
            ? static_cast<float>(ReadComponentFloat(reinterpret_cast<std::uint32_t>(selected)))
            : 5000.0f;

        selected = CharacterDefs_SelectLocalizedAttribute(node, KEY(PORCENTAJE), locale);
        if (selected)
            CSF_AT(int, record, 0x12C) =
                ParseIntegerComponent(reinterpret_cast<std::uint32_t>(selected));

        std::uint8_t* damage = LookupNamedElement(node, nullptr, KEY(DAMAGE));
        selected = CharacterDefs_SelectLocalizedParent(damage, KEY(VIDA_8A9878), locale);
        CSF_AT(int, record, 0xDC) =
            ReadIntegerAttribute(selected, nullptr, KEY(VIDA_8A9878));

        std::uint8_t* bounds = FindProperty(node, nullptr, KEY(BBOX_8A985C));
        if (bounds)
        {
            std::uint32_t lower[0x2C / sizeof(std::uint32_t)]{};
            std::uint32_t upper[3]{};
            selected = CharacterDefs_SelectLocalizedParent(bounds, KEY(INF), locale);
            DataNode_ReadVector3Property(
                selected, nullptr, KEY(INF), reinterpret_cast<int>(lower));
            selected = CharacterDefs_SelectLocalizedParent(bounds, KEY(SUP), locale);
            DataNode_ReadVector3Property(
                selected, nullptr, KEY(SUP), reinterpret_cast<int>(upper));
            CSF_AT(std::uint32_t, record, 0x130) = upper[0];
            CSF_AT(std::uint32_t, record, 0x134) = upper[1];
            CSF_AT(std::uint32_t, record, 0x138) = upper[2];
            CSF_AT(std::uint32_t, record, 0x13C) = lower[0];
            CSF_AT(std::uint32_t, record, 0x140) = lower[1];
            CSF_AT(std::uint32_t, record, 0x144) = lower[2];
        }

        std::uint8_t* collision = LookupNamedElement(node, nullptr, KEY(MODELO_COLISION));
        selected = CharacterDefs_SelectLocalizedParent(collision, KEY(TIPO_8A9820), locale);
        MenuExtras_ReadText(selected, nullptr, KEY(TIPO_8A9820), buffer, 256);
        CSF_AT(int, record, 0xCC) = CharacterDefs_ParseCollisionModelType(buffer);

        selected = CharacterDefs_SelectLocalizedParent(collision, KEY(TAM), locale);
        DataNode_ReadVector3Property(
            selected, nullptr, KEY(TAM), reinterpret_cast<int>(record + 352));

        if (FindProperty(collision, nullptr, KEY(EXTERNAL_SHAPE)))
        {
            selected = CharacterDefs_SelectLocalizedParent(collision, KEY(EXTERNAL_SHAPE), locale);
            CSF_AT(int, record, 0x188) =
                ReadIntegerAttribute(selected, nullptr, KEY(EXTERNAL_SHAPE));
        }
        else
        {
            CSF_AT(int, record, 0x188) = 1;
        }

        if (FindProperty(collision, nullptr, KEY(EXTERNAL_SHAPE_OFFSET)))
        {
            selected = CharacterDefs_SelectLocalizedParent(
                collision, KEY(EXTERNAL_SHAPE_OFFSET), locale);
            DataNode_ReadVector3Property(
                selected, nullptr, KEY(EXTERNAL_SHAPE_OFFSET),
                reinterpret_cast<int>(record + 396));
        }
        else
        {
            CSF_AT(std::uint32_t, record, 0x18C) = 0;
            CSF_AT(std::uint32_t, record, 0x190) = 0;
            CSF_AT(std::uint32_t, record, 0x194) = 0;
        }

        if (CSF_AT(int, record, 0xCC) == 1)
        {
            selected = CharacterDefs_SelectLocalizedParent(collision, KEY(CO_MODEL_EX), locale);
            MenuExtras_ReadText(selected, nullptr, KEY(CO_MODEL_EX), buffer, 256);
            AssignStringRange_429B60(
                record + 364, nullptr, buffer, static_cast<unsigned int>(std::strlen(buffer)));
        }

        std::uint8_t* vehicle = FindProperty(node, nullptr, KEY(VEHICLE_INFO));
        if (vehicle)
        {
            selected = CharacterDefs_SelectLocalizedParent(vehicle, KEY(VEH_MAX_VEL), locale);
            CSF_AT(float, record, 0x1B4) =
                static_cast<float>(ReadFloatProperty(selected, nullptr, KEY(VEH_MAX_VEL)));

            selected = CharacterDefs_SelectLocalizedParent(vehicle, KEY(VEH_MAX_BACK_VEL), locale);
            CSF_AT(float, record, 0x1B8) =
                static_cast<float>(ReadFloatProperty(selected, nullptr, KEY(VEH_MAX_BACK_VEL)));

            selected = CharacterDefs_SelectLocalizedParent(vehicle, KEY(VEH_MAX_ACCEL), locale);
            CSF_AT(float, record, 0x1BC) =
                static_cast<float>(ReadFloatProperty(selected, nullptr, KEY(VEH_MAX_ACCEL)));

            selected = CharacterDefs_SelectLocalizedParent(vehicle, KEY(VEH_MAX_DECEL), locale);
            CSF_AT(float, record, 0x1C0) =
                static_cast<float>(ReadFloatProperty(selected, nullptr, KEY(VEH_MAX_DECEL)));

            selected = CharacterDefs_SelectLocalizedParent(vehicle, KEY(VEHICLE_VIDA_CHASIS), locale);
            CSF_AT(int, record, 0x1C4) =
                ReadIntegerProperty(selected, nullptr, KEY(VEHICLE_VIDA_CHASIS));

            selected = CharacterDefs_SelectLocalizedParent(vehicle, KEY(VEHICLE_VIDA_MOTOR), locale);
            CSF_AT(int, record, 0x1C8) =
                ReadIntegerProperty(selected, nullptr, KEY(VEHICLE_VIDA_MOTOR));

            selected = CharacterDefs_SelectLocalizedParent(vehicle, KEY(VEHICLE_VIDA_RUEDAS), locale);
            CSF_AT(int, record, 0x1CC) =
                ReadIntegerProperty(selected, nullptr, KEY(VEHICLE_VIDA_RUEDAS));

            selected = CharacterDefs_SelectLocalizedParent(vehicle, KEY(VEHICLE_VIDA_BAJOS), locale);
            CSF_AT(int, record, 0x1D0) =
                ReadIntegerProperty(selected, nullptr, KEY(VEHICLE_VIDA_BAJOS));
        }

        selected = CharacterDefs_SelectLocalizedParent(node, KEY(MODO_COLISION), locale);
        const int collisionMode = ReadIntegerProperty(selected, nullptr, KEY(MODO_COLISION));
        CSF_AT(int, record, 0xD0) = collisionMode;
        if (collisionMode == 3)
            CSF_AT(int, record, 0xD0) = 0;

        std::uint8_t* physics = FindProperty(node, nullptr, KEY(PHYSIC));
        if (physics)
        {
            selected = CharacterDefs_SelectLocalizedParent(physics, KEY(TIPO_8A9820), locale);
            MenuExtras_ReadText(selected, nullptr, KEY(TIPO_8A9820), buffer, 256);
            CSF_AT(int, record, 0x1F0) = WeaponDefs_ParsePhysicsType(buffer);

            selected = CharacterDefs_SelectLocalizedParent(physics, KEY(MODEL_FILE), locale);
            MenuExtras_ReadText(selected, nullptr, KEY(MODEL_FILE), buffer, 256);
            AssignStringRange_429B60(
                record + 500, nullptr, buffer, static_cast<unsigned int>(std::strlen(buffer)));

            selected = CharacterDefs_SelectLocalizedParent(physics, KEY(DYNAMIC), locale);
            CSF_AT(std::uint8_t, record, 0x21C) =
                ReadIntegerProperty(selected, nullptr, KEY(DYNAMIC)) != 0;

            selected = CharacterDefs_SelectLocalizedParent(physics, KEY(MOVING), locale);
            CSF_AT(std::uint8_t, record, 0x21D) =
                ReadIntegerProperty(selected, nullptr, KEY(MOVING)) != 0;

            selected = CharacterDefs_SelectLocalizedParent(physics, KEY(MASS), locale);
            CSF_AT(float, record, 0x220) =
                static_cast<float>(ReadFloatProperty(selected, nullptr, KEY(MASS)));

            selected = CharacterDefs_SelectLocalizedParent(physics, KEY(BOUNCE), locale);
            CSF_AT(float, record, 0x224) =
                static_cast<float>(ReadFloatProperty(selected, nullptr, KEY(BOUNCE)));

            selected = CharacterDefs_SelectLocalizedParent(physics, KEY(SLIDE), locale);
            CSF_AT(float, record, 0x228) =
                static_cast<float>(ReadFloatProperty(selected, nullptr, KEY(SLIDE)));
        }

        selected = CharacterDefs_SelectLocalizedParent(node, KEY(COMPOR), locale);
        MenuExtras_ReadText(selected, nullptr, KEY(COMPOR), buffer, 256);
        CSF_AT(int, record, 0xD8) = CharacterDefs_ParseBehavior(buffer);

        selected = FindProperty(node, nullptr, KEY(ANIMACIONES));
        CharacterDefs_ReadAnimations(
            self, nullptr, selected, reinterpret_cast<int>(record), record + 240, locale);

        std::uint8_t* weapons =
            CharacterDefs_SelectLocalizedAttribute(node, KEY(ARMAS_8A9888), locale);
        if (weapons)
        {
            const unsigned int weaponCount = MenuExtras_GetChildCount(weapons, nullptr);
            const unsigned int capacity = CSF_AT(unsigned int, record, 0x104);
            if (weaponCount > capacity)
                GrowButtonArray(record + 256, nullptr, static_cast<int>(weaponCount - capacity));

            for (unsigned int weaponIndex = 0; weaponIndex < weaponCount; ++weaponIndex)
            {
                component = GetComponent(weapons, nullptr, weaponIndex);
                const int weaponId = ParseIntegerComponent(component);
                const unsigned int size = CSF_AT(unsigned int, record, 0x108);
                const unsigned int oldCapacity = CSF_AT(unsigned int, record, 0x104);
                if (size >= oldCapacity)
                {
                    const unsigned int growth = CSF_AT(unsigned int, record, 0x10C);
                    void* replacement = Mem_Alloc(4 * (growth + oldCapacity));
                    const unsigned int copyCount = CSF_AT(unsigned int, record, 0x108);
                    if (copyCount)
                        std::memcpy(replacement, CSF_AT(void*, record, 0x100), 4 * copyCount);
                    void* oldData = CSF_AT(void*, record, 0x100);
                    CSF_AT(unsigned int, record, 0x104) += growth;
                    if (oldData)
                    {
                        Mem_Free(oldData);
                        CSF_AT(void*, record, 0x100) = nullptr;
                    }
                    CSF_AT(void*, record, 0x100) = replacement;
                }
                const unsigned int appendIndex = CSF_AT(unsigned int, record, 0x108);
                int* destination = CSF_AT(int*, record, 0x100) + appendIndex;
                CSF_AT(unsigned int, record, 0x108) = appendIndex + 1;
                *destination = weaponId;
            }
        }

        std::uint8_t* items =
            CharacterDefs_SelectLocalizedAttribute(node, KEY(ITEMS_8A988C), locale);
        if (items)
        {
            const unsigned int itemCount = MenuExtras_GetChildCount(items, nullptr);
            const unsigned int capacity = CSF_AT(unsigned int, record, 0x114);
            if (itemCount > capacity)
            {
                const unsigned int growth = itemCount - capacity;
                void* replacement = Mem_Alloc(4 * itemCount);
                const unsigned int copyCount = CSF_AT(unsigned int, record, 0x118);
                if (copyCount)
                    std::memcpy(replacement, CSF_AT(void*, record, 0x110), 4 * copyCount);
                void* oldData = CSF_AT(void*, record, 0x110);
                CSF_AT(unsigned int, record, 0x114) += growth;
                if (oldData)
                {
                    Mem_Free(oldData);
                    CSF_AT(void*, record, 0x110) = nullptr;
                }
                CSF_AT(void*, record, 0x110) = replacement;
            }

            for (unsigned int itemIndex = 0; itemIndex < itemCount; ++itemIndex)
            {
                component = GetComponent(items, nullptr, itemIndex);
                const int itemId = ParseIntegerComponent(component);
                const unsigned int size = CSF_AT(unsigned int, record, 0x118);
                const unsigned int oldCapacity = CSF_AT(unsigned int, record, 0x114);
                if (size >= oldCapacity)
                {
                    const unsigned int growth = CSF_AT(unsigned int, record, 0x11C);
                    void* replacement = Mem_Alloc(4 * (growth + oldCapacity));
                    const unsigned int copyCount = CSF_AT(unsigned int, record, 0x118);
                    if (copyCount)
                        std::memcpy(replacement, CSF_AT(void*, record, 0x110), 4 * copyCount);
                    void* oldData = CSF_AT(void*, record, 0x110);
                    CSF_AT(unsigned int, record, 0x114) += growth;
                    if (oldData)
                    {
                        Mem_Free(oldData);
                        CSF_AT(void*, record, 0x110) = nullptr;
                    }
                    CSF_AT(void*, record, 0x110) = replacement;
                }
                const unsigned int appendIndex = CSF_AT(unsigned int, record, 0x118);
                int* destination = CSF_AT(int*, record, 0x110) + appendIndex;
                CSF_AT(unsigned int, record, 0x118) = appendIndex + 1;
                *destination = itemId;
            }
        }
    }
    return 1;
}

#undef KEY

HOOK(0x00763FA0, C_0086f128_ReadCharacterRecords, "auto");
