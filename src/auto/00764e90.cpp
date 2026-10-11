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
CSF_FUNC(int, __cdecl, CharacterDefs_ParseCollisionType, 0x00761AD0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseGrade, 0x00761B70, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParsePhysicsType, 0x00761D50, (char* String1));
CSF_FUNC(char, __fastcall, CharacterDefs_ReadAnimations, 0x007621F0, (void* self, void* edx, void* a2, int a3, void* a4, char* String1));
CSF_FUNC(int, __fastcall, CharacterDefs_InitializeCharacterData, 0x00762650, (int a1, int a2));

// The value argument occupies 556 bytes on the stack. Expose all 139
// dwords explicitly so the binding describes the callee's full ret 560 ABI.
CSF_FUNC(int, __fastcall, CharacterDefs_ResizeRecords, 0x007632E0,
    (void* self, void* edx, unsigned int requestedCount,
     std::uint32_t value0, std::uint32_t value1, std::uint32_t value2,
     std::uint32_t value3, std::uint32_t value4, std::uint32_t value5,
     std::uint32_t value6, std::uint32_t value7, std::uint32_t value8,
     std::uint32_t value9, std::uint32_t value10, std::uint32_t value11,
     std::uint32_t value12, std::uint32_t value13, std::uint32_t value14,
     std::uint32_t value15, std::uint32_t value16, std::uint32_t value17,
     std::uint32_t value18, std::uint32_t value19, std::uint32_t value20,
     std::uint32_t value21, std::uint32_t value22, std::uint32_t value23,
     std::uint32_t value24, std::uint32_t value25, std::uint32_t value26,
     std::uint32_t value27, std::uint32_t value28, std::uint32_t value29,
     std::uint32_t value30, std::uint32_t value31, std::uint32_t value32,
     std::uint32_t value33, std::uint32_t value34, std::uint32_t value35,
     std::uint32_t value36, std::uint32_t value37, std::uint32_t value38,
     std::uint32_t value39, std::uint32_t value40, std::uint32_t value41,
     std::uint32_t value42, std::uint32_t value43, std::uint32_t value44,
     std::uint32_t value45, std::uint32_t value46, std::uint32_t value47,
     std::uint32_t value48, std::uint32_t value49, std::uint32_t value50,
     std::uint32_t value51, std::uint32_t value52, std::uint32_t value53,
     std::uint32_t value54, std::uint32_t value55, std::uint32_t value56,
     std::uint32_t value57, std::uint32_t value58, std::uint32_t value59,
     std::uint32_t value60, std::uint32_t value61, std::uint32_t value62,
     std::uint32_t value63, std::uint32_t value64, std::uint32_t value65,
     std::uint32_t value66, std::uint32_t value67, std::uint32_t value68,
     std::uint32_t value69, std::uint32_t value70, std::uint32_t value71,
     std::uint32_t value72, std::uint32_t value73, std::uint32_t value74,
     std::uint32_t value75, std::uint32_t value76, std::uint32_t value77,
     std::uint32_t value78, std::uint32_t value79, std::uint32_t value80,
     std::uint32_t value81, std::uint32_t value82, std::uint32_t value83,
     std::uint32_t value84, std::uint32_t value85, std::uint32_t value86,
     std::uint32_t value87, std::uint32_t value88, std::uint32_t value89,
     std::uint32_t value90, std::uint32_t value91, std::uint32_t value92,
     std::uint32_t value93, std::uint32_t value94, std::uint32_t value95,
     std::uint32_t value96, std::uint32_t value97, std::uint32_t value98,
     std::uint32_t value99, std::uint32_t value100, std::uint32_t value101,
     std::uint32_t value102, std::uint32_t value103, std::uint32_t value104,
     std::uint32_t value105, std::uint32_t value106, std::uint32_t value107,
     std::uint32_t value108, std::uint32_t value109, std::uint32_t value110,
     std::uint32_t value111, std::uint32_t value112, std::uint32_t value113,
     std::uint32_t value114, std::uint32_t value115, std::uint32_t value116,
     std::uint32_t value117, std::uint32_t value118, std::uint32_t value119,
     std::uint32_t value120, std::uint32_t value121, std::uint32_t value122,
     std::uint32_t value123, std::uint32_t value124, std::uint32_t value125,
     std::uint32_t value126, std::uint32_t value127, std::uint32_t value128,
     std::uint32_t value129, std::uint32_t value130, std::uint32_t value131,
     std::uint32_t value132, std::uint32_t value133, std::uint32_t value134,
     std::uint32_t value135, std::uint32_t value136, std::uint32_t value137,
     std::uint32_t value138));

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
CSF_GLOBAL(const char*, g_pszKey_WAV_CALL_REPARTIDOR, 0x008A98A0);
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

// Scalar readers expect the selected variant container, or the original parent.
static std::uint8_t* CharacterDefs_FindLocalizedContainer(
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

// Optional attributes instead select the same-named attribute inside a variant.
static std::uint8_t* CharacterDefs_FindLocalizedAttribute(
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

static int __fastcall C_0086f128_ReadCharacterDefinitions(
    C_0086f128* self, void* /*edx*/, int* records,
    S_00692b80* source, char* variant)
{
    // The supplied inventory labels these direct callee addresses as vtables.
    (void)CSF_VTBL(0x00692B80);
    (void)CSF_VTBL(0x0041E5F0);

    char buffer[0x10C] = {};
    auto* list = LookupNamedElement(
        reinterpret_cast<std::uint8_t*>(source), nullptr,
        g_pszKey_LISTADATOS_8A9804);
    const unsigned int recordCount = MenuExtras_GetChildCount(list, nullptr);

    std::uint32_t initialValue[556 / sizeof(std::uint32_t)] = {};
    CharacterDefs_InitializeCharacterData(
        reinterpret_cast<int>(initialValue), 0);
    CharacterDefs_ResizeRecords(records, nullptr, recordCount,
        initialValue[0], initialValue[1], initialValue[2],
        initialValue[3], initialValue[4], initialValue[5],
        initialValue[6], initialValue[7], initialValue[8],
        initialValue[9], initialValue[10], initialValue[11],
        initialValue[12], initialValue[13], initialValue[14],
        initialValue[15], initialValue[16], initialValue[17],
        initialValue[18], initialValue[19], initialValue[20],
        initialValue[21], initialValue[22], initialValue[23],
        initialValue[24], initialValue[25], initialValue[26],
        initialValue[27], initialValue[28], initialValue[29],
        initialValue[30], initialValue[31], initialValue[32],
        initialValue[33], initialValue[34], initialValue[35],
        initialValue[36], initialValue[37], initialValue[38],
        initialValue[39], initialValue[40], initialValue[41],
        initialValue[42], initialValue[43], initialValue[44],
        initialValue[45], initialValue[46], initialValue[47],
        initialValue[48], initialValue[49], initialValue[50],
        initialValue[51], initialValue[52], initialValue[53],
        initialValue[54], initialValue[55], initialValue[56],
        initialValue[57], initialValue[58], initialValue[59],
        initialValue[60], initialValue[61], initialValue[62],
        initialValue[63], initialValue[64], initialValue[65],
        initialValue[66], initialValue[67], initialValue[68],
        initialValue[69], initialValue[70], initialValue[71],
        initialValue[72], initialValue[73], initialValue[74],
        initialValue[75], initialValue[76], initialValue[77],
        initialValue[78], initialValue[79], initialValue[80],
        initialValue[81], initialValue[82], initialValue[83],
        initialValue[84], initialValue[85], initialValue[86],
        initialValue[87], initialValue[88], initialValue[89],
        initialValue[90], initialValue[91], initialValue[92],
        initialValue[93], initialValue[94], initialValue[95],
        initialValue[96], initialValue[97], initialValue[98],
        initialValue[99], initialValue[100], initialValue[101],
        initialValue[102], initialValue[103], initialValue[104],
        initialValue[105], initialValue[106], initialValue[107],
        initialValue[108], initialValue[109], initialValue[110],
        initialValue[111], initialValue[112], initialValue[113],
        initialValue[114], initialValue[115], initialValue[116],
        initialValue[117], initialValue[118], initialValue[119],
        initialValue[120], initialValue[121], initialValue[122],
        initialValue[123], initialValue[124], initialValue[125],
        initialValue[126], initialValue[127], initialValue[128],
        initialValue[129], initialValue[130], initialValue[131],
        initialValue[132], initialValue[133], initialValue[134],
        initialValue[135], initialValue[136], initialValue[137],
        initialValue[138]);

    for (unsigned int index = 0; index < recordCount; ++index)
    {
        auto* node = reinterpret_cast<std::uint8_t*>(
            GetComponent(list, nullptr, index));
        auto* record = reinterpret_cast<std::uint8_t*>(
            CSF_AT(std::uint32_t, records, 0x4) + 556 * index);

        CSF_AT(int, record, 0x0) = ReadIntegerAttribute(
            node, nullptr, const_cast<char*>(g_pszKey_ID_8A9808));

        auto* selected = CharacterDefs_FindLocalizedContainer(
            node, g_pszKey_REF_ID, variant);
        CSF_AT(int, record, 0xC0) = ReadIntegerAttribute(
            selected, nullptr, const_cast<char*>(g_pszKey_REF_ID));

        auto* property = FindProperty(
            node, nullptr, const_cast<char*>(g_pszKey_REF_ID2));
        if (property)
            CSF_AT(int, record, 0xC4) =
                ParseIntegerComponent(reinterpret_cast<std::uint32_t>(property));

        selected = CharacterDefs_FindLocalizedContainer(
            node, g_pszKey_NOMBRE_8A9818, variant);
        MenuExtras_ReadText(selected, nullptr,
            const_cast<char*>(g_pszKey_NOMBRE_8A9818), buffer, 256);
        AssignStringRange_429B60(record + 0x20, nullptr, buffer,
            static_cast<unsigned int>(std::strlen(buffer)));

        selected = CharacterDefs_FindLocalizedContainer(
            node, g_pszKey_TIPO_8A9820, variant);
        MenuExtras_ReadText(selected, nullptr,
            const_cast<char*>(g_pszKey_TIPO_8A9820), buffer, 256);
        CSF_AT(int, record, 0xB4) = CharacterDefs_ParseType(buffer);

        property = CharacterDefs_FindLocalizedAttribute(
            node, g_pszKey_SUBTIPO, variant);
        if (property)
        {
            CopyAttributeText(property, nullptr, buffer, 256);
            CSF_AT(int, record, 0xB8) = CharacterDefs_ParseSubtype(buffer);
        }
        property = CharacterDefs_FindLocalizedAttribute(
            node, g_pszKey_GRADO, variant);
        if (property)
        {
            CopyAttributeText(property, nullptr, buffer, 256);
            CSF_AT(int, record, 0xBC) = WeaponDefs_ParseGrade(buffer);
        }

        selected = CharacterDefs_FindLocalizedContainer(
            node, g_pszKey_MODELO, variant);
        MenuExtras_ReadText(selected, nullptr,
            const_cast<char*>(g_pszKey_MODELO), buffer, 256);
        AssignStringRange_429B60(record + 0x3C, nullptr, buffer,
            static_cast<unsigned int>(std::strlen(buffer)));

        selected = CharacterDefs_FindLocalizedContainer(
            node, g_pszKey_MODELO_TERCERA, variant);
        MenuExtras_ReadText(selected, nullptr,
            const_cast<char*>(g_pszKey_MODELO_TERCERA), buffer, 256);
        AssignStringRange_429B60(record + 0x58, nullptr, buffer,
            static_cast<unsigned int>(std::strlen(buffer)));

        property = CharacterDefs_FindLocalizedAttribute(
            node, g_pszKey_OCLUSOR, variant);
        CSF_AT(std::uint8_t, record, 0x158) =
            property && ParseIntegerComponent(
                reinterpret_cast<std::uint32_t>(property)) != 0;

        auto* headHeight = CharacterDefs_FindLocalizedAttribute(
            node, g_pszKey_ALTURA_CABEZA, variant);
        auto component = GetComponent(headHeight, nullptr, 0);
        CSF_AT(float, record, 0xE4) =
            static_cast<float>(ReadComponentFloat(component));
        component = GetComponent(headHeight, nullptr, 1u);
        CSF_AT(float, record, 0xE8) =
            static_cast<float>(ReadComponentFloat(component));
        component = GetComponent(headHeight, nullptr, 2u);
        CSF_AT(float, record, 0xEC) =
            static_cast<float>(ReadComponentFloat(component));

        property = CharacterDefs_FindLocalizedAttribute(
            node, g_pszKey_ANGULO_VISTA, variant);
        CSF_AT(float, record, 0x120) = property
            ? static_cast<float>(ReadComponentFloat(
                reinterpret_cast<std::uint32_t>(property)))
            : 120.0f;
        property = CharacterDefs_FindLocalizedAttribute(
            node, g_pszKey_DISTANCIA_VISTA, variant);
        CSF_AT(float, record, 0x124) = property
            ? static_cast<float>(ReadComponentFloat(
                reinterpret_cast<std::uint32_t>(property)))
            : 2000.0f;

        property = CharacterDefs_FindLocalizedAttribute(
            node, g_pszKey_WAV_CALL_REPARTIDOR, variant);
        if (property)
            CSF_AT(int, record, 0x148) =
                ParseIntegerComponent(reinterpret_cast<std::uint32_t>(property));

        property = CharacterDefs_FindLocalizedAttribute(
            node, g_pszKey_DISTANCIA_LARGA_VISTA, variant);
        CSF_AT(float, record, 0x128) = property
            ? static_cast<float>(ReadComponentFloat(
                reinterpret_cast<std::uint32_t>(property)))
            : 5000.0f;

        property = CharacterDefs_FindLocalizedAttribute(
            node, g_pszKey_PORCENTAJE, variant);
        if (property)
            CSF_AT(int, record, 0x12C) =
                ParseIntegerComponent(reinterpret_cast<std::uint32_t>(property));

        auto* damage = LookupNamedElement(node, nullptr, g_pszKey_DAMAGE);
        selected = CharacterDefs_FindLocalizedContainer(
            damage, g_pszKey_VIDA_8A9878, variant);
        CSF_AT(int, record, 0xDC) = ReadIntegerAttribute(
            selected, nullptr, const_cast<char*>(g_pszKey_VIDA_8A9878));

        auto* bounds = FindProperty(
            node, nullptr, const_cast<char*>(g_pszKey_BBOX_8A985C));
        if (bounds)
        {
            int lower[0x2C / sizeof(int)] = {};
            int upper[3] = {};
            selected = CharacterDefs_FindLocalizedContainer(
                bounds, g_pszKey_INF, variant);
            DataNode_ReadVector3Property(selected, nullptr,
                const_cast<char*>(g_pszKey_INF), reinterpret_cast<int>(lower));

            selected = CharacterDefs_FindLocalizedContainer(
                bounds, g_pszKey_SUP, variant);
            DataNode_ReadVector3Property(selected, nullptr,
                const_cast<char*>(g_pszKey_SUP), reinterpret_cast<int>(upper));

            CSF_AT(int, record, 0x130) = upper[0];
            CSF_AT(int, record, 0x134) = upper[1];
            CSF_AT(int, record, 0x138) = upper[2];
            CSF_AT(int, record, 0x13C) = lower[0];
            CSF_AT(int, record, 0x140) = lower[1];
            CSF_AT(int, record, 0x144) = lower[2];
        }

        auto* collision = LookupNamedElement(
            node, nullptr, g_pszKey_MODELO_COLISION);
        selected = CharacterDefs_FindLocalizedContainer(
            collision, g_pszKey_TIPO_8A9820, variant);
        MenuExtras_ReadText(selected, nullptr,
            const_cast<char*>(g_pszKey_TIPO_8A9820), buffer, 256);
        CSF_AT(int, record, 0xCC) = CharacterDefs_ParseCollisionType(buffer);

        selected = CharacterDefs_FindLocalizedContainer(
            collision, g_pszKey_TAM, variant);
        DataNode_ReadVector3Property(selected, nullptr,
            const_cast<char*>(g_pszKey_TAM), reinterpret_cast<int>(record + 0x160));

        if (FindProperty(collision, nullptr,
                const_cast<char*>(g_pszKey_EXTERNAL_SHAPE)))
        {
            selected = CharacterDefs_FindLocalizedContainer(
                collision, g_pszKey_EXTERNAL_SHAPE, variant);
            CSF_AT(int, record, 0x188) = ReadIntegerAttribute(
                selected, nullptr, const_cast<char*>(g_pszKey_EXTERNAL_SHAPE));
        }
        else
        {
            CSF_AT(int, record, 0x188) = 1;
        }

        if (FindProperty(collision, nullptr,
                const_cast<char*>(g_pszKey_EXTERNAL_SHAPE_OFFSET)))
        {
            selected = CharacterDefs_FindLocalizedContainer(
                collision, g_pszKey_EXTERNAL_SHAPE_OFFSET, variant);
            DataNode_ReadVector3Property(selected, nullptr,
                const_cast<char*>(g_pszKey_EXTERNAL_SHAPE_OFFSET),
                reinterpret_cast<int>(record + 0x18C));
        }
        else
        {
            CSF_AT(int, record, 0x18C) = 0;
            CSF_AT(int, record, 0x190) = 0;
            CSF_AT(int, record, 0x194) = 0;
        }

        if (CSF_AT(int, record, 0xCC) == 1)
        {
            selected = CharacterDefs_FindLocalizedContainer(
                collision, g_pszKey_CO_MODEL_EX, variant);
            MenuExtras_ReadText(selected, nullptr,
                const_cast<char*>(g_pszKey_CO_MODEL_EX), buffer, 256);
            AssignStringRange_429B60(record + 0x16C, nullptr, buffer,
                static_cast<unsigned int>(std::strlen(buffer)));
        }

        auto* vehicle = FindProperty(
            node, nullptr, const_cast<char*>(g_pszKey_VEHICLE_INFO));
        if (vehicle)
        {
            selected = CharacterDefs_FindLocalizedContainer(
                vehicle, g_pszKey_VEH_MAX_VEL, variant);
            CSF_AT(float, record, 0x1B4) = static_cast<float>(
                ReadFloatProperty(selected, nullptr,
                    const_cast<char*>(g_pszKey_VEH_MAX_VEL)));

            selected = CharacterDefs_FindLocalizedContainer(
                vehicle, g_pszKey_VEH_MAX_BACK_VEL, variant);
            CSF_AT(float, record, 0x1B8) = static_cast<float>(
                ReadFloatProperty(selected, nullptr,
                    const_cast<char*>(g_pszKey_VEH_MAX_BACK_VEL)));

            selected = CharacterDefs_FindLocalizedContainer(
                vehicle, g_pszKey_VEH_MAX_ACCEL, variant);
            CSF_AT(float, record, 0x1BC) = static_cast<float>(
                ReadFloatProperty(selected, nullptr,
                    const_cast<char*>(g_pszKey_VEH_MAX_ACCEL)));

            selected = CharacterDefs_FindLocalizedContainer(
                vehicle, g_pszKey_VEH_MAX_DECEL, variant);
            CSF_AT(float, record, 0x1C0) = static_cast<float>(
                ReadFloatProperty(selected, nullptr,
                    const_cast<char*>(g_pszKey_VEH_MAX_DECEL)));

            selected = CharacterDefs_FindLocalizedContainer(
                vehicle, g_pszKey_VEHICLE_VIDA_CHASIS, variant);
            CSF_AT(int, record, 0x1C4) = ReadIntegerProperty(
                selected, nullptr, const_cast<char*>(g_pszKey_VEHICLE_VIDA_CHASIS));

            selected = CharacterDefs_FindLocalizedContainer(
                vehicle, g_pszKey_VEHICLE_VIDA_MOTOR, variant);
            CSF_AT(int, record, 0x1C8) = ReadIntegerProperty(
                selected, nullptr, const_cast<char*>(g_pszKey_VEHICLE_VIDA_MOTOR));

            selected = CharacterDefs_FindLocalizedContainer(
                vehicle, g_pszKey_VEHICLE_VIDA_RUEDAS, variant);
            CSF_AT(int, record, 0x1CC) = ReadIntegerProperty(
                selected, nullptr, const_cast<char*>(g_pszKey_VEHICLE_VIDA_RUEDAS));

            selected = CharacterDefs_FindLocalizedContainer(
                vehicle, g_pszKey_VEHICLE_VIDA_BAJOS, variant);
            CSF_AT(int, record, 0x1D0) = ReadIntegerProperty(
                selected, nullptr, const_cast<char*>(g_pszKey_VEHICLE_VIDA_BAJOS));
        }

        selected = CharacterDefs_FindLocalizedContainer(
            node, g_pszKey_MODO_COLISION, variant);
        const int collisionMode = ReadIntegerProperty(
            selected, nullptr, const_cast<char*>(g_pszKey_MODO_COLISION));
        CSF_AT(int, record, 0xD0) = collisionMode;
        if (collisionMode == 3)
            CSF_AT(int, record, 0xD0) = 0;

        auto* physics = FindProperty(
            node, nullptr, const_cast<char*>(g_pszKey_PHYSIC));
        if (physics)
        {
            selected = CharacterDefs_FindLocalizedContainer(
                physics, g_pszKey_TIPO_8A9820, variant);
            MenuExtras_ReadText(selected, nullptr,
                const_cast<char*>(g_pszKey_TIPO_8A9820), buffer, 256);
            CSF_AT(int, record, 0x1F0) = WeaponDefs_ParsePhysicsType(buffer);

            selected = CharacterDefs_FindLocalizedContainer(
                physics, g_pszKey_MODEL_FILE, variant);
            MenuExtras_ReadText(selected, nullptr,
                const_cast<char*>(g_pszKey_MODEL_FILE), buffer, 256);
            AssignStringRange_429B60(record + 0x1F4, nullptr, buffer,
                static_cast<unsigned int>(std::strlen(buffer)));

            selected = CharacterDefs_FindLocalizedContainer(
                physics, g_pszKey_DYNAMIC, variant);
            CSF_AT(std::uint8_t, record, 0x21C) = ReadIntegerProperty(
                selected, nullptr, const_cast<char*>(g_pszKey_DYNAMIC)) != 0;

            selected = CharacterDefs_FindLocalizedContainer(
                physics, g_pszKey_MOVING, variant);
            CSF_AT(std::uint8_t, record, 0x21D) = ReadIntegerProperty(
                selected, nullptr, const_cast<char*>(g_pszKey_MOVING)) != 0;

            selected = CharacterDefs_FindLocalizedContainer(
                physics, g_pszKey_MASS, variant);
            CSF_AT(float, record, 0x220) = static_cast<float>(
                ReadFloatProperty(selected, nullptr, const_cast<char*>(g_pszKey_MASS)));

            selected = CharacterDefs_FindLocalizedContainer(
                physics, g_pszKey_BOUNCE, variant);
            CSF_AT(float, record, 0x224) = static_cast<float>(
                ReadFloatProperty(selected, nullptr, const_cast<char*>(g_pszKey_BOUNCE)));

            selected = CharacterDefs_FindLocalizedContainer(
                physics, g_pszKey_SLIDE, variant);
            CSF_AT(float, record, 0x228) = static_cast<float>(
                ReadFloatProperty(selected, nullptr, const_cast<char*>(g_pszKey_SLIDE)));
        }

        selected = CharacterDefs_FindLocalizedContainer(
            node, g_pszKey_COMPOR, variant);
        MenuExtras_ReadText(selected, nullptr,
            const_cast<char*>(g_pszKey_COMPOR), buffer, 256);
        CSF_AT(int, record, 0xD8) = CharacterDefs_ParseBehavior(buffer);

        property = FindProperty(
            node, nullptr, const_cast<char*>(g_pszKey_ANIMACIONES));
        CharacterDefs_ReadAnimations(self, nullptr, property,
            reinterpret_cast<int>(record), record + 0xF0, variant);

        auto* weapons = CharacterDefs_FindLocalizedAttribute(
            node, g_pszKey_ARMAS_8A9888, variant);
        if (weapons)
        {
            const unsigned int count = MenuExtras_GetChildCount(weapons, nullptr);
            const unsigned int capacity = CSF_AT(std::uint32_t, record, 0x104);
            if (count > capacity)
                GrowButtonArray(record + 0x100, nullptr,
                    static_cast<int>(count - capacity));

            for (unsigned int weaponIndex = 0; weaponIndex < count; ++weaponIndex)
            {
                component = GetComponent(weapons, nullptr, weaponIndex);
                const int weaponId = ParseIntegerComponent(component);
                const unsigned int used = CSF_AT(std::uint32_t, record, 0x108);
                const unsigned int oldCapacity = CSF_AT(std::uint32_t, record, 0x104);
                if (used >= oldCapacity)
                {
                    const unsigned int growth = CSF_AT(std::uint32_t, record, 0x10C);
                    void* allocation = Mem_Alloc(4 * (growth + oldCapacity));
                    const unsigned int copyCount = CSF_AT(std::uint32_t, record, 0x108);
                    if (copyCount)
                        std::memcpy(allocation,
                            reinterpret_cast<const void*>(
                                CSF_AT(std::uint32_t, record, 0x100)),
                            4 * copyCount);

                    void* oldAllocation = reinterpret_cast<void*>(
                        CSF_AT(std::uint32_t, record, 0x100));
                    CSF_AT(std::uint32_t, record, 0x104) += growth;
                    if (oldAllocation)
                    {
                        Mem_Free(oldAllocation);
                        CSF_AT(std::uint32_t, record, 0x100) = 0;
                    }
                    CSF_AT(std::uint32_t, record, 0x100) =
                        reinterpret_cast<std::uint32_t>(allocation);
                }
                const unsigned int position = CSF_AT(std::uint32_t, record, 0x108);
                auto* destination = reinterpret_cast<int*>(
                    CSF_AT(std::uint32_t, record, 0x100) + 4 * position);
                CSF_AT(std::uint32_t, record, 0x108) = position + 1;
                *destination = weaponId;
            }
        }

        auto* items = CharacterDefs_FindLocalizedAttribute(
            node, g_pszKey_ITEMS_8A988C, variant);
        if (items)
        {
            const unsigned int count = MenuExtras_GetChildCount(items, nullptr);
            const unsigned int capacity = CSF_AT(std::uint32_t, record, 0x114);
            if (count > capacity)
            {
                const unsigned int growth = count - capacity;
                void* allocation = Mem_Alloc(4 * count);
                const unsigned int copyCount = CSF_AT(std::uint32_t, record, 0x118);
                if (copyCount)
                    std::memcpy(allocation,
                        reinterpret_cast<const void*>(
                            CSF_AT(std::uint32_t, record, 0x110)),
                        4 * copyCount);

                void* oldAllocation = reinterpret_cast<void*>(
                    CSF_AT(std::uint32_t, record, 0x110));
                CSF_AT(std::uint32_t, record, 0x114) += growth;
                if (oldAllocation)
                {
                    Mem_Free(oldAllocation);
                    CSF_AT(std::uint32_t, record, 0x110) = 0;
                }
                CSF_AT(std::uint32_t, record, 0x110) =
                    reinterpret_cast<std::uint32_t>(allocation);
            }

            for (unsigned int itemIndex = 0; itemIndex < count; ++itemIndex)
            {
                component = GetComponent(items, nullptr, itemIndex);
                const int itemId = ParseIntegerComponent(component);
                const unsigned int used = CSF_AT(std::uint32_t, record, 0x118);
                const unsigned int oldCapacity = CSF_AT(std::uint32_t, record, 0x114);
                if (used >= oldCapacity)
                {
                    const unsigned int growth = CSF_AT(std::uint32_t, record, 0x11C);
                    void* allocation = Mem_Alloc(4 * (growth + oldCapacity));
                    const unsigned int copyCount = CSF_AT(std::uint32_t, record, 0x118);
                    if (copyCount)
                        std::memcpy(allocation,
                            reinterpret_cast<const void*>(
                                CSF_AT(std::uint32_t, record, 0x110)),
                            4 * copyCount);

                    void* oldAllocation = reinterpret_cast<void*>(
                        CSF_AT(std::uint32_t, record, 0x110));
                    CSF_AT(std::uint32_t, record, 0x114) += growth;
                    if (oldAllocation)
                    {
                        Mem_Free(oldAllocation);
                        CSF_AT(std::uint32_t, record, 0x110) = 0;
                    }
                    CSF_AT(std::uint32_t, record, 0x110) =
                        reinterpret_cast<std::uint32_t>(allocation);
                }
                const unsigned int position = CSF_AT(std::uint32_t, record, 0x118);
                auto* destination = reinterpret_cast<int*>(
                    CSF_AT(std::uint32_t, record, 0x110) + 4 * position);
                CSF_AT(std::uint32_t, record, 0x118) = position + 1;
                *destination = itemId;
            }
        }
    }
    return 1;
}

HOOK(0x00764E90, C_0086f128_ReadCharacterDefinitions, "auto");
