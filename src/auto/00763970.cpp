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
CSF_FUNC(int, __cdecl, CharacterDefs_ParseBehaviorType, 0x00761A80, (char* String1));
CSF_FUNC(int, __cdecl, CharacterDefs_ParseCollisionType, 0x00761AD0, (char* String1));
CSF_FUNC(char, __fastcall, CharacterDefs_ReadAnimations, 0x00761FD0, (void* self, void* edx, void* a2, int a3, void* a4));
CSF_FUNC(int, __fastcall, CharacterDefs_InitializeCharacterData, 0x00762650, (int a1, int a2));

// The 556-byte record is passed inline after the count: 560 stack bytes.
// Spell out its words so the binding also exposes the exact cleanup size.
CSF_FUNC(int, __fastcall, CharacterDefs_ResizeRecords, 0x007632E0,
    (void* self, void* edx, unsigned int requestedCount,
     std::uint32_t word0, std::uint32_t word1, std::uint32_t word2,
     std::uint32_t word3, std::uint32_t word4, std::uint32_t word5,
     std::uint32_t word6, std::uint32_t word7, std::uint32_t word8,
     std::uint32_t word9, std::uint32_t word10, std::uint32_t word11,
     std::uint32_t word12, std::uint32_t word13, std::uint32_t word14,
     std::uint32_t word15, std::uint32_t word16, std::uint32_t word17,
     std::uint32_t word18, std::uint32_t word19, std::uint32_t word20,
     std::uint32_t word21, std::uint32_t word22, std::uint32_t word23,
     std::uint32_t word24, std::uint32_t word25, std::uint32_t word26,
     std::uint32_t word27, std::uint32_t word28, std::uint32_t word29,
     std::uint32_t word30, std::uint32_t word31, std::uint32_t word32,
     std::uint32_t word33, std::uint32_t word34, std::uint32_t word35,
     std::uint32_t word36, std::uint32_t word37, std::uint32_t word38,
     std::uint32_t word39, std::uint32_t word40, std::uint32_t word41,
     std::uint32_t word42, std::uint32_t word43, std::uint32_t word44,
     std::uint32_t word45, std::uint32_t word46, std::uint32_t word47,
     std::uint32_t word48, std::uint32_t word49, std::uint32_t word50,
     std::uint32_t word51, std::uint32_t word52, std::uint32_t word53,
     std::uint32_t word54, std::uint32_t word55, std::uint32_t word56,
     std::uint32_t word57, std::uint32_t word58, std::uint32_t word59,
     std::uint32_t word60, std::uint32_t word61, std::uint32_t word62,
     std::uint32_t word63, std::uint32_t word64, std::uint32_t word65,
     std::uint32_t word66, std::uint32_t word67, std::uint32_t word68,
     std::uint32_t word69, std::uint32_t word70, std::uint32_t word71,
     std::uint32_t word72, std::uint32_t word73, std::uint32_t word74,
     std::uint32_t word75, std::uint32_t word76, std::uint32_t word77,
     std::uint32_t word78, std::uint32_t word79, std::uint32_t word80,
     std::uint32_t word81, std::uint32_t word82, std::uint32_t word83,
     std::uint32_t word84, std::uint32_t word85, std::uint32_t word86,
     std::uint32_t word87, std::uint32_t word88, std::uint32_t word89,
     std::uint32_t word90, std::uint32_t word91, std::uint32_t word92,
     std::uint32_t word93, std::uint32_t word94, std::uint32_t word95,
     std::uint32_t word96, std::uint32_t word97, std::uint32_t word98,
     std::uint32_t word99, std::uint32_t word100, std::uint32_t word101,
     std::uint32_t word102, std::uint32_t word103, std::uint32_t word104,
     std::uint32_t word105, std::uint32_t word106, std::uint32_t word107,
     std::uint32_t word108, std::uint32_t word109, std::uint32_t word110,
     std::uint32_t word111, std::uint32_t word112, std::uint32_t word113,
     std::uint32_t word114, std::uint32_t word115, std::uint32_t word116,
     std::uint32_t word117, std::uint32_t word118, std::uint32_t word119,
     std::uint32_t word120, std::uint32_t word121, std::uint32_t word122,
     std::uint32_t word123, std::uint32_t word124, std::uint32_t word125,
     std::uint32_t word126, std::uint32_t word127, std::uint32_t word128,
     std::uint32_t word129, std::uint32_t word130, std::uint32_t word131,
     std::uint32_t word132, std::uint32_t word133, std::uint32_t word134,
     std::uint32_t word135, std::uint32_t word136, std::uint32_t word137,
     std::uint32_t word138));

CSF_GLOBAL(const char*, g_pszKey_LISTADATOS_8A9804, 0x008A9804);
CSF_GLOBAL(const char*, g_pszKey_ID_8A9808, 0x008A9808);
CSF_GLOBAL(const char*, g_pszKey_NOMBRE_8A9818, 0x008A9818);
CSF_GLOBAL(const char*, g_pszKey_TIPO_8A9820, 0x008A9820);
CSF_GLOBAL(const char*, g_pszKey_SUBTIPO, 0x008A9824);
CSF_GLOBAL(const char*, g_pszKey_MODELO, 0x008A9830);
CSF_GLOBAL(const char*, g_pszKey_MODELO_TERCERA, 0x008A9834);
CSF_GLOBAL(const char*, g_pszKey_DAMAGE, 0x008A9848);
CSF_GLOBAL(const char*, g_pszKey_MODELO_COLISION, 0x008A984C);
CSF_GLOBAL(const char*, g_pszKey_MODO_COLISION, 0x008A9850);
CSF_GLOBAL(const char*, g_pszKey_TAM, 0x008A9858);
CSF_GLOBAL(const char*, g_pszKey_EXTERNAL_SHAPE, 0x008A9868);
CSF_GLOBAL(const char*, g_pszKey_EXTERNAL_SHAPE_OFFSET, 0x008A986C);
CSF_GLOBAL(const char*, g_pszKey_CO_MODEL_EX, 0x008A9870);
CSF_GLOBAL(const char*, g_pszKey_COMPOR, 0x008A9874);
CSF_GLOBAL(const char*, g_pszKey_VIDA_8A9878, 0x008A9878);
CSF_GLOBAL(const char*, g_pszKey_ANIMACIONES, 0x008A9880);
CSF_GLOBAL(const char*, g_pszKey_ALTURA_CABEZA, 0x008A9884);
CSF_GLOBAL(const char*, g_pszKey_ARMAS_8A9888, 0x008A9888);
CSF_GLOBAL(const char*, g_pszKey_ANGULO_VISTA, 0x008A9890);
CSF_GLOBAL(const char*, g_pszKey_DISTANCIA_VISTA, 0x008A9894);
CSF_GLOBAL(const char*, g_pszKey_DISTANCIA_LARGA_VISTA, 0x008A9898);
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

[[maybe_unused]] static const auto kDataNodeVtable = CSF_VTBL(0x00692B80);
[[maybe_unused]] static const auto kWeaponArrayVtable = CSF_VTBL(0x0041E5F0);

static char __fastcall C_0086f128_ReadCharacterDefinitions(
    C_0086f128* self, void* /*edx*/, S_00692b80* source)
{
    char buffer[0x128];
    std::memset(buffer, 0, 256);

    auto* entries = LookupNamedElement(
        reinterpret_cast<std::uint8_t*>(source), nullptr,
        g_pszKey_LISTADATOS_8A9804);
    const unsigned int entryCount = MenuExtras_GetChildCount(entries, nullptr);
    void* records = CSF_AT(void*, self, 0x8);

    std::uint32_t defaultRecord[556 / 4]{};
    CharacterDefs_InitializeCharacterData(
        reinterpret_cast<int>(defaultRecord), 0);
    CharacterDefs_ResizeRecords(
        records, nullptr, entryCount,
        defaultRecord[0], defaultRecord[1], defaultRecord[2],
        defaultRecord[3], defaultRecord[4], defaultRecord[5],
        defaultRecord[6], defaultRecord[7], defaultRecord[8],
        defaultRecord[9], defaultRecord[10], defaultRecord[11],
        defaultRecord[12], defaultRecord[13], defaultRecord[14],
        defaultRecord[15], defaultRecord[16], defaultRecord[17],
        defaultRecord[18], defaultRecord[19], defaultRecord[20],
        defaultRecord[21], defaultRecord[22], defaultRecord[23],
        defaultRecord[24], defaultRecord[25], defaultRecord[26],
        defaultRecord[27], defaultRecord[28], defaultRecord[29],
        defaultRecord[30], defaultRecord[31], defaultRecord[32],
        defaultRecord[33], defaultRecord[34], defaultRecord[35],
        defaultRecord[36], defaultRecord[37], defaultRecord[38],
        defaultRecord[39], defaultRecord[40], defaultRecord[41],
        defaultRecord[42], defaultRecord[43], defaultRecord[44],
        defaultRecord[45], defaultRecord[46], defaultRecord[47],
        defaultRecord[48], defaultRecord[49], defaultRecord[50],
        defaultRecord[51], defaultRecord[52], defaultRecord[53],
        defaultRecord[54], defaultRecord[55], defaultRecord[56],
        defaultRecord[57], defaultRecord[58], defaultRecord[59],
        defaultRecord[60], defaultRecord[61], defaultRecord[62],
        defaultRecord[63], defaultRecord[64], defaultRecord[65],
        defaultRecord[66], defaultRecord[67], defaultRecord[68],
        defaultRecord[69], defaultRecord[70], defaultRecord[71],
        defaultRecord[72], defaultRecord[73], defaultRecord[74],
        defaultRecord[75], defaultRecord[76], defaultRecord[77],
        defaultRecord[78], defaultRecord[79], defaultRecord[80],
        defaultRecord[81], defaultRecord[82], defaultRecord[83],
        defaultRecord[84], defaultRecord[85], defaultRecord[86],
        defaultRecord[87], defaultRecord[88], defaultRecord[89],
        defaultRecord[90], defaultRecord[91], defaultRecord[92],
        defaultRecord[93], defaultRecord[94], defaultRecord[95],
        defaultRecord[96], defaultRecord[97], defaultRecord[98],
        defaultRecord[99], defaultRecord[100], defaultRecord[101],
        defaultRecord[102], defaultRecord[103], defaultRecord[104],
        defaultRecord[105], defaultRecord[106], defaultRecord[107],
        defaultRecord[108], defaultRecord[109], defaultRecord[110],
        defaultRecord[111], defaultRecord[112], defaultRecord[113],
        defaultRecord[114], defaultRecord[115], defaultRecord[116],
        defaultRecord[117], defaultRecord[118], defaultRecord[119],
        defaultRecord[120], defaultRecord[121], defaultRecord[122],
        defaultRecord[123], defaultRecord[124], defaultRecord[125],
        defaultRecord[126], defaultRecord[127], defaultRecord[128],
        defaultRecord[129], defaultRecord[130], defaultRecord[131],
        defaultRecord[132], defaultRecord[133], defaultRecord[134],
        defaultRecord[135], defaultRecord[136], defaultRecord[137],
        defaultRecord[138]);

    unsigned int recordOffset = 0;
    for (unsigned int entryIndex = 0; entryIndex < entryCount; ++entryIndex)
    {
        auto* component = reinterpret_cast<std::uint8_t*>(
            GetComponent(entries, nullptr, entryIndex));
        auto* record = CSF_AT(std::uint8_t*, CSF_AT(void*, self, 0x8), 0x4)
                     + recordOffset;

        CSF_AT(int, record, 0x0) = ReadIntegerAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_ID_8A9808));

        MenuExtras_ReadText(component, nullptr,
            const_cast<char*>(g_pszKey_NOMBRE_8A9818), buffer, 256);
        AssignStringRange_429B60(record + 0x20, nullptr, buffer,
            static_cast<unsigned int>(std::strlen(buffer)));

        MenuExtras_ReadText(component, nullptr,
            const_cast<char*>(g_pszKey_TIPO_8A9820), buffer, 256);
        CSF_AT(int, record, 0xB4) = CharacterDefs_ParseCharacterType(buffer);

        auto* property = FindProperty(
            component, nullptr, const_cast<char*>(g_pszKey_SUBTIPO));
        if (property)
        {
            CopyAttributeText(property, nullptr, buffer, 256);
            CSF_AT(int, record, 0xB8) = CharacterDefs_ParseCharacterSubtype(buffer);
        }

        MenuExtras_ReadText(component, nullptr,
            const_cast<char*>(g_pszKey_MODELO), buffer, 256);
        AssignStringRange_429B60(record + 0x3C, nullptr, buffer,
            static_cast<unsigned int>(std::strlen(buffer)));

        MenuExtras_ReadText(component, nullptr,
            const_cast<char*>(g_pszKey_MODELO_TERCERA), buffer, 256);
        AssignStringRange_429B60(record + 0x58, nullptr, buffer,
            static_cast<unsigned int>(std::strlen(buffer)));

        property = FindProperty(component, nullptr,
            const_cast<char*>(g_pszKey_OCLUSOR));
        if (property)
            CSF_AT(std::uint8_t, record, 0x158) =
                ParseIntegerComponent(reinterpret_cast<std::uint32_t>(property)) != 0;
        else
            CSF_AT(std::uint8_t, record, 0x158) = 0;

        auto* headHeights = LookupNamedElement(
            component, nullptr, g_pszKey_ALTURA_CABEZA);
        CSF_AT(float, record, 0xE4) = static_cast<float>(
            ReadComponentFloat(GetComponent(headHeights, nullptr, 0)));
        CSF_AT(float, record, 0xE8) = static_cast<float>(
            ReadComponentFloat(GetComponent(headHeights, nullptr, 1u)));
        CSF_AT(float, record, 0xEC) = static_cast<float>(
            ReadComponentFloat(GetComponent(headHeights, nullptr, 2u)));

        property = FindProperty(component, nullptr,
            const_cast<char*>(g_pszKey_ANGULO_VISTA));
        if (property)
            CSF_AT(float, record, 0x120) = static_cast<float>(
                ReadComponentFloat(reinterpret_cast<std::uint32_t>(property)));
        else
            CSF_AT(float, record, 0x120) = 120.0f;

        property = FindProperty(component, nullptr,
            const_cast<char*>(g_pszKey_DISTANCIA_VISTA));
        if (property)
            CSF_AT(float, record, 0x124) = static_cast<float>(
                ReadComponentFloat(reinterpret_cast<std::uint32_t>(property)));
        else
            CSF_AT(float, record, 0x124) = 2000.0f;

        property = FindProperty(component, nullptr,
            const_cast<char*>(g_pszKey_DISTANCIA_LARGA_VISTA));
        if (property)
            CSF_AT(float, record, 0x128) = static_cast<float>(
                ReadComponentFloat(reinterpret_cast<std::uint32_t>(property)));
        else
            CSF_AT(float, record, 0x128) = 5000.0f;

        auto* damage = LookupNamedElement(component, nullptr, g_pszKey_DAMAGE);
        CSF_AT(int, record, 0xDC) = ReadIntegerAttribute(
            damage, nullptr, const_cast<char*>(g_pszKey_VIDA_8A9878));

        auto* collision = LookupNamedElement(
            component, nullptr, g_pszKey_MODELO_COLISION);
        MenuExtras_ReadText(collision, nullptr,
            const_cast<char*>(g_pszKey_TIPO_8A9820), buffer, 256);
        CSF_AT(int, record, 0xCC) = CharacterDefs_ParseCollisionType(buffer);

        DataNode_ReadVector3Property(collision, nullptr,
            const_cast<char*>(g_pszKey_TAM),
            reinterpret_cast<int>(record + 0x160));

        if (FindProperty(collision, nullptr,
                const_cast<char*>(g_pszKey_EXTERNAL_SHAPE)))
        {
            CSF_AT(int, record, 0x188) = ReadIntegerAttribute(
                collision, nullptr, const_cast<char*>(g_pszKey_EXTERNAL_SHAPE));
        }
        else
        {
            CSF_AT(int, record, 0x188) = 1;
        }

        if (FindProperty(collision, nullptr,
                const_cast<char*>(g_pszKey_EXTERNAL_SHAPE_OFFSET)))
        {
            DataNode_ReadVector3Property(collision, nullptr,
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
            MenuExtras_ReadText(collision, nullptr,
                const_cast<char*>(g_pszKey_CO_MODEL_EX), buffer, 256);
            AssignStringRange_429B60(record + 0x16C, nullptr, buffer,
                static_cast<unsigned int>(std::strlen(buffer)));
        }

        auto* vehicle = FindProperty(component, nullptr,
            const_cast<char*>(g_pszKey_VEHICLE_INFO));
        if (vehicle)
        {
            CSF_AT(float, record, 0x1B4) = static_cast<float>(
                ReadFloatProperty(vehicle, nullptr,
                    const_cast<char*>(g_pszKey_VEH_MAX_VEL)));
            CSF_AT(float, record, 0x1B8) = static_cast<float>(
                ReadFloatProperty(vehicle, nullptr,
                    const_cast<char*>(g_pszKey_VEH_MAX_BACK_VEL)));
            CSF_AT(float, record, 0x1BC) = static_cast<float>(
                ReadFloatProperty(vehicle, nullptr,
                    const_cast<char*>(g_pszKey_VEH_MAX_ACCEL)));
            CSF_AT(float, record, 0x1C0) = static_cast<float>(
                ReadFloatProperty(vehicle, nullptr,
                    const_cast<char*>(g_pszKey_VEH_MAX_DECEL)));
            CSF_AT(int, record, 0x1C4) = ReadIntegerProperty(
                vehicle, nullptr, const_cast<char*>(g_pszKey_VEHICLE_VIDA_CHASIS));
            CSF_AT(int, record, 0x1C8) = ReadIntegerProperty(
                vehicle, nullptr, const_cast<char*>(g_pszKey_VEHICLE_VIDA_MOTOR));
            CSF_AT(int, record, 0x1CC) = ReadIntegerProperty(
                vehicle, nullptr, const_cast<char*>(g_pszKey_VEHICLE_VIDA_RUEDAS));
            CSF_AT(int, record, 0x1D0) = ReadIntegerProperty(
                vehicle, nullptr, const_cast<char*>(g_pszKey_VEHICLE_VIDA_BAJOS));
        }

        const int collisionMode = ReadIntegerProperty(
            component, nullptr, const_cast<char*>(g_pszKey_MODO_COLISION));
        CSF_AT(int, record, 0xD0) = collisionMode;
        if (collisionMode == 3)
            CSF_AT(int, record, 0xD0) = 0;

        MenuExtras_ReadText(component, nullptr,
            const_cast<char*>(g_pszKey_COMPOR), buffer, 256);
        CSF_AT(int, record, 0xD8) = CharacterDefs_ParseBehaviorType(buffer);

        property = FindProperty(component, nullptr,
            const_cast<char*>(g_pszKey_ANIMACIONES));
        CharacterDefs_ReadAnimations(self, nullptr, property,
            reinterpret_cast<int>(record), record + 0xF0);

        auto* weapons = FindProperty(component, nullptr,
            const_cast<char*>(g_pszKey_ARMAS_8A9888));
        if (weapons)
        {
            const unsigned int weaponCount =
                MenuExtras_GetChildCount(weapons, nullptr);
            auto* weaponArray = record + 0x100;
            const unsigned int capacity =
                CSF_AT(std::uint32_t, weaponArray, 0x4);
            if (weaponCount > capacity)
                GrowButtonArray(weaponArray, nullptr,
                    static_cast<int>(weaponCount - capacity));

            for (unsigned int weaponIndex = 0;
                 weaponIndex < weaponCount; ++weaponIndex)
            {
                const std::uint32_t weapon =
                    GetComponent(weapons, nullptr, weaponIndex);
                const int weaponId = ParseIntegerComponent(weapon);

                const unsigned int count =
                    CSF_AT(std::uint32_t, weaponArray, 0x8);
                const unsigned int currentCapacity =
                    CSF_AT(std::uint32_t, weaponArray, 0x4);
                if (count >= currentCapacity)
                {
                    const std::uint32_t growth =
                        CSF_AT(std::uint32_t, weaponArray, 0xC);
                    void* newStorage = Mem_Alloc(4 * (growth + currentCapacity));
                    const std::uint32_t copyCount =
                        CSF_AT(std::uint32_t, weaponArray, 0x8);
                    if (copyCount)
                    {
                        std::memcpy(newStorage,
                            CSF_AT(void*, weaponArray, 0x0), 4 * copyCount);
                    }
                    CSF_AT(std::uint32_t, weaponArray, 0x4) += growth;
                    if (CSF_AT(void*, weaponArray, 0x0))
                    {
                        Mem_Free(CSF_AT(void*, weaponArray, 0x0));
                        CSF_AT(void*, weaponArray, 0x0) = nullptr;
                    }
                    CSF_AT(void*, weaponArray, 0x0) = newStorage;
                }

                const std::uint32_t appendIndex =
                    CSF_AT(std::uint32_t, weaponArray, 0x8);
                auto* destination = CSF_AT(std::uint8_t*, weaponArray, 0x0)
                                  + 4 * appendIndex;
                CSF_AT(std::uint32_t, weaponArray, 0x8) = appendIndex + 1;
                CSF_AT(int, destination, 0x0) = weaponId;
            }
        }
        recordOffset += 556;
    }
    return 1;
}

HOOK(0x00763970, C_0086f128_ReadCharacterDefinitions, "auto");
