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
CSF_FUNC(char, __fastcall, CharacterDefs_ReadAnimations, 0x007621F0, (void* self, void* edx, void* a2, int a3, void* a4, char* String1));
CSF_FUNC(int, __fastcall, CharacterDefs_InitializeRecord, 0x00762650, (int a1, int a2));

// One count followed by the complete 556-byte record value: 560 stack bytes.
// Explicit DWORD parameters preserve the entire record, including the first DWORD.
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

CSF_GLOBAL(const char*, g_KeyList, 0x008A9804);
CSF_GLOBAL(const char*, g_KeyId, 0x008A9808);
CSF_GLOBAL(const char*, g_KeyRefId, 0x008A980C);
CSF_GLOBAL(const char*, g_KeyRefId2, 0x008A9810);
CSF_GLOBAL(const char*, g_KeyDeathSound, 0x008A9814);
CSF_GLOBAL(const char*, g_KeyName, 0x008A9818);
CSF_GLOBAL(const char*, g_KeyFliName, 0x008A981C);
CSF_GLOBAL(const char*, g_KeyType, 0x008A9820);
CSF_GLOBAL(const char*, g_KeySubtype, 0x008A9824);
CSF_GLOBAL(const char*, g_KeyGrade, 0x008A9828);
CSF_GLOBAL(const char*, g_KeyMale, 0x008A982C);
CSF_GLOBAL(const char*, g_KeyModel, 0x008A9830);
CSF_GLOBAL(const char*, g_KeyThirdPersonModel, 0x008A9834);
CSF_GLOBAL(const char*, g_KeyLod1Name, 0x008A9838);
CSF_GLOBAL(const char*, g_KeyLod2Name, 0x008A983C);
CSF_GLOBAL(const char*, g_KeyLod1Distance, 0x008A9840);
CSF_GLOBAL(const char*, g_KeyLod2Distance, 0x008A9844);
CSF_GLOBAL(const char*, g_KeyDamage, 0x008A9848);
CSF_GLOBAL(const char*, g_KeyCollisionModel, 0x008A984C);
CSF_GLOBAL(const char*, g_KeyCollisionMode, 0x008A9850);
CSF_GLOBAL(const char*, g_KeyMaterial, 0x008A9854);
CSF_GLOBAL(const char*, g_KeySize, 0x008A9858);
CSF_GLOBAL(const char*, g_KeyBoundingBox, 0x008A985C);
CSF_GLOBAL(const char*, g_KeyLower, 0x008A9860);
CSF_GLOBAL(const char*, g_KeyUpper, 0x008A9864);
CSF_GLOBAL(const char*, g_KeyExternalShape, 0x008A9868);
CSF_GLOBAL(const char*, g_KeyExternalShapeOffset, 0x008A986C);
CSF_GLOBAL(const char*, g_KeyExtendedCollisionModel, 0x008A9870);
CSF_GLOBAL(const char*, g_KeyBehavior, 0x008A9874);
CSF_GLOBAL(const char*, g_KeyHealth, 0x008A9878);
CSF_GLOBAL(const char*, g_KeyEnergy, 0x008A987C);
CSF_GLOBAL(const char*, g_KeyAnimations, 0x008A9880);
CSF_GLOBAL(const char*, g_KeyHeadHeight, 0x008A9884);
CSF_GLOBAL(const char*, g_KeyWeapons, 0x008A9888);
CSF_GLOBAL(const char*, g_KeyItems, 0x008A988C);
CSF_GLOBAL(const char*, g_KeyViewAngle, 0x008A9890);
CSF_GLOBAL(const char*, g_KeyViewDistance, 0x008A9894);
CSF_GLOBAL(const char*, g_KeyLongViewDistance, 0x008A9898);
CSF_GLOBAL(const char*, g_KeyPercentage, 0x008A989C);
CSF_GLOBAL(const char*, g_KeyDistributorCallSound, 0x008A98A0);
CSF_GLOBAL(const char*, g_KeySpotlightInfo, 0x008A98A4);
CSF_GLOBAL(const char*, g_KeySpotlightTexture, 0x008A98A8);
CSF_GLOBAL(const char*, g_KeyVehicleInfo, 0x008A98AC);
CSF_GLOBAL(const char*, g_KeyMaxSpeed, 0x008A98B0);
CSF_GLOBAL(const char*, g_KeyMaxReverseSpeed, 0x008A98B4);
CSF_GLOBAL(const char*, g_KeyMaxAcceleration, 0x008A98B8);
CSF_GLOBAL(const char*, g_KeyMaxDeceleration, 0x008A98BC);
CSF_GLOBAL(const char*, g_KeyMinTurn, 0x008A98C0);
CSF_GLOBAL(const char*, g_KeyLowContact, 0x008A98C4);
CSF_GLOBAL(const char*, g_KeySwayMultiplier, 0x008A98C8);
CSF_GLOBAL(const char*, g_KeyPitchInertia, 0x008A98CC);
CSF_GLOBAL(const char*, g_KeyRollInertia, 0x008A98D0);
CSF_GLOBAL(const char*, g_KeyExternalPitch, 0x008A98D4);
CSF_GLOBAL(const char*, g_KeyExternalRoll, 0x008A98D8);
CSF_GLOBAL(const char*, g_KeyOccluder, 0x008A98DC);
CSF_GLOBAL(const char*, g_KeyIgnorePvs, 0x008A98E0);
CSF_GLOBAL(const char*, g_KeyIgnoreBoundingSphere, 0x008A98E4);
CSF_GLOBAL(const char*, g_KeyChassisHealth, 0x008A98E8);
CSF_GLOBAL(const char*, g_KeyEngineHealth, 0x008A98EC);
CSF_GLOBAL(const char*, g_KeyWheelHealth, 0x008A98F0);
CSF_GLOBAL(const char*, g_KeyUnderbodyHealth, 0x008A98F4);
CSF_GLOBAL(const char*, g_KeyPhysics, 0x008A98F8);
CSF_GLOBAL(const char*, g_KeyModelFile, 0x008A98FC);
CSF_GLOBAL(const char*, g_KeyDynamic, 0x008A9900);
CSF_GLOBAL(const char*, g_KeyMoving, 0x008A9904);
CSF_GLOBAL(const char*, g_KeyMass, 0x008A9908);
CSF_GLOBAL(const char*, g_KeyBounce, 0x008A990C);
CSF_GLOBAL(const char*, g_KeySlide, 0x008A9910);
CSF_GLOBAL(const char*, g_KeyShadow, 0x008A9914);
CSF_GLOBAL(const char*, g_KeyRespawnTime, 0x008A9918);
CSF_GLOBAL(const char*, g_KeyExplosionDamage, 0x008A991C);
CSF_GLOBAL(const char*, g_KeyExplosionRadius, 0x008A9920);

static char* CharacterDefs_Key(const char* key)
{
    return const_cast<char*>(key);
}

static std::uint8_t* CharacterDefs_GetPropertySource(
    std::uint8_t* node, const char* key, char* variant)
{
    auto* property = FindProperty(node, nullptr, CharacterDefs_Key(key));
    if (property)
    {
        auto* selected = FindProperty(property, nullptr, variant);
        if (selected)
            return selected;
    }
    return node;
}

static std::uint8_t* CharacterDefs_GetOptionalAttribute(
    std::uint8_t* node, const char* key, char* variant)
{
    auto* property = FindProperty(node, nullptr, CharacterDefs_Key(key));
    if (!property)
        return nullptr;
    auto* selected = FindProperty(property, nullptr, variant);
    return selected ? FindProperty(selected, nullptr, CharacterDefs_Key(key)) : property;
}

static int CharacterDefs_ReadComponentInteger(std::uint8_t* component)
{
    return ParseIntegerComponent(reinterpret_cast<std::uint32_t>(component));
}

static float CharacterDefs_ReadComponentScalar(std::uint8_t* component)
{
    return static_cast<float>(ReadComponentFloat(reinterpret_cast<std::uint32_t>(component)));
}

static void CharacterDefs_AssignText(void* destination, char* buffer)
{
    AssignStringRange_429B60(destination, nullptr, buffer,
                           static_cast<unsigned int>(std::strlen(buffer)));
}

static void CharacterDefs_AppendInteger(void* array, int value)
{
    const auto capacity = CSF_AT(std::uint32_t, array, 0x4);
    if (CSF_AT(std::uint32_t, array, 0x8) >= capacity)
    {
        const auto growth = CSF_AT(std::uint32_t, array, 0xC);
        void* replacement = Mem_Alloc(4 * (growth + capacity));
        const auto size = CSF_AT(std::uint32_t, array, 0x8);
        if (size)
            std::memcpy(replacement, CSF_AT(void*, array, 0x0), 4 * size);
        void* old = CSF_AT(void*, array, 0x0);
        CSF_AT(std::uint32_t, array, 0x4) += growth;
        if (old)
        {
            Mem_Free(old);
            CSF_AT(void*, array, 0x0) = nullptr;
        }
        CSF_AT(void*, array, 0x0) = replacement;
    }
    const auto size = CSF_AT(std::uint32_t, array, 0x8);
    int* destination = CSF_AT(int*, array, 0x0) + size;
    CSF_AT(std::uint32_t, array, 0x8) = size + 1;
    *destination = value;
}

static char __fastcall C_0086f128_ReadCharacterDefinitions(
    C_0086f128* self, void* /*edx*/, int* records, S_00692b80* root, char* variant)
{
    // Packet annotations identify direct-call addresses, not virtual dispatch.
    (void)CSF_VTBL(0x00692B80);
    (void)CSF_VTBL(0x0041E5F0);

    char buffer[0x110] = {};
    auto* list = LookupNamedElement(reinterpret_cast<std::uint8_t*>(root), nullptr, g_KeyList);
    const auto count = MenuExtras_GetChildCount(list, nullptr);

    std::uint32_t initial[556 / sizeof(std::uint32_t)] = {};
    CharacterDefs_InitializeRecord(reinterpret_cast<int>(initial), 0);
    CharacterDefs_ResizeRecords(records, nullptr, count,
        initial[0], initial[1], initial[2], initial[3], initial[4],
        initial[5], initial[6], initial[7], initial[8], initial[9],
        initial[10], initial[11], initial[12], initial[13], initial[14],
        initial[15], initial[16], initial[17], initial[18], initial[19],
        initial[20], initial[21], initial[22], initial[23], initial[24],
        initial[25], initial[26], initial[27], initial[28], initial[29],
        initial[30], initial[31], initial[32], initial[33], initial[34],
        initial[35], initial[36], initial[37], initial[38], initial[39],
        initial[40], initial[41], initial[42], initial[43], initial[44],
        initial[45], initial[46], initial[47], initial[48], initial[49],
        initial[50], initial[51], initial[52], initial[53], initial[54],
        initial[55], initial[56], initial[57], initial[58], initial[59],
        initial[60], initial[61], initial[62], initial[63], initial[64],
        initial[65], initial[66], initial[67], initial[68], initial[69],
        initial[70], initial[71], initial[72], initial[73], initial[74],
        initial[75], initial[76], initial[77], initial[78], initial[79],
        initial[80], initial[81], initial[82], initial[83], initial[84],
        initial[85], initial[86], initial[87], initial[88], initial[89],
        initial[90], initial[91], initial[92], initial[93], initial[94],
        initial[95], initial[96], initial[97], initial[98], initial[99],
        initial[100], initial[101], initial[102], initial[103], initial[104],
        initial[105], initial[106], initial[107], initial[108], initial[109],
        initial[110], initial[111], initial[112], initial[113], initial[114],
        initial[115], initial[116], initial[117], initial[118], initial[119],
        initial[120], initial[121], initial[122], initial[123], initial[124],
        initial[125], initial[126], initial[127], initial[128], initial[129],
        initial[130], initial[131], initial[132], initial[133], initial[134],
        initial[135], initial[136], initial[137], initial[138]);

    for (unsigned int index = 0; index < count; ++index)
    {
        auto* component = reinterpret_cast<std::uint8_t*>(GetComponent(list, nullptr, index));
        auto* record = reinterpret_cast<std::uint8_t*>(records[1]) + 556 * index;
        std::uint8_t* property;
        std::uint8_t* source;

        CSF_AT(int, record, 0x0) =
            ReadIntegerAttribute(component, nullptr, CharacterDefs_Key(g_KeyId));
        source = CharacterDefs_GetPropertySource(component, g_KeyRefId, variant);
        CSF_AT(int, record, 0xC0) =
            ReadIntegerAttribute(source, nullptr, CharacterDefs_Key(g_KeyRefId));
        property = FindProperty(component, nullptr, CharacterDefs_Key(g_KeyRefId2));
        if (property)
            CSF_AT(int, record, 0xC4) = CharacterDefs_ReadComponentInteger(property);
        property = FindProperty(component, nullptr, CharacterDefs_Key(g_KeyDeathSound));
        if (property)
            CSF_AT(int, record, 0xC8) = CharacterDefs_ReadComponentInteger(property);

        source = CharacterDefs_GetPropertySource(component, g_KeyName, variant);
        MenuExtras_ReadText(source, nullptr, CharacterDefs_Key(g_KeyName), buffer, 256);
        CharacterDefs_AssignText(record + 32, buffer);
        source = CharacterDefs_GetPropertySource(component, g_KeyFliName, variant);
        MenuExtras_ReadText(source, nullptr, CharacterDefs_Key(g_KeyFliName), buffer, 256);
        CharacterDefs_AssignText(record + 4, buffer);
        source = CharacterDefs_GetPropertySource(component, g_KeyType, variant);
        MenuExtras_ReadText(source, nullptr, CharacterDefs_Key(g_KeyType), buffer, 256);
        CSF_AT(int, record, 0xB4) = CharacterDefs_ParseType(buffer);

        property = CharacterDefs_GetOptionalAttribute(component, g_KeySubtype, variant);
        if (property)
        {
            CopyAttributeText(property, nullptr, buffer, 256);
            CSF_AT(int, record, 0xB8) = CharacterDefs_ParseSubtype(buffer);
        }
        property = CharacterDefs_GetOptionalAttribute(component, g_KeyGrade, variant);
        if (property)
        {
            CopyAttributeText(property, nullptr, buffer, 256);
            CSF_AT(int, record, 0xBC) = WeaponDefs_ParseGrade(buffer);
        }
        property = CharacterDefs_GetOptionalAttribute(component, g_KeyMale, variant);
        if (property)
            CSF_AT(std::uint8_t, record, 0x15C) = CharacterDefs_ReadComponentInteger(property) != 0;

        source = CharacterDefs_GetPropertySource(component, g_KeyModel, variant);
        MenuExtras_ReadText(source, nullptr, CharacterDefs_Key(g_KeyModel), buffer, 256);
        CharacterDefs_AssignText(record + 60, buffer);
        source = CharacterDefs_GetPropertySource(component, g_KeyThirdPersonModel, variant);
        MenuExtras_ReadText(source, nullptr, CharacterDefs_Key(g_KeyThirdPersonModel), buffer, 256);
        CharacterDefs_AssignText(record + 88, buffer);
        property = CharacterDefs_GetOptionalAttribute(component, g_KeyLod1Name, variant);
        if (property)
        {
            CopyAttributeText(property, nullptr, buffer, 256);
            CharacterDefs_AssignText(record + 116, buffer);
        }
        property = CharacterDefs_GetOptionalAttribute(component, g_KeyLod2Name, variant);
        if (property)
        {
            CopyAttributeText(property, nullptr, buffer, 256);
            CharacterDefs_AssignText(record + 144, buffer);
        }
        property = CharacterDefs_GetOptionalAttribute(component, g_KeyLod1Distance, variant);
        if (property)
            CSF_AT(float, record, 0xAC) = CharacterDefs_ReadComponentScalar(property);
        property = CharacterDefs_GetOptionalAttribute(component, g_KeyLod2Distance, variant);
        if (property)
            CSF_AT(float, record, 0xB0) = CharacterDefs_ReadComponentScalar(property);

        property = CharacterDefs_GetOptionalAttribute(component, g_KeyOccluder, variant);
        CSF_AT(std::uint8_t, record, 0x158) =
            property && CharacterDefs_ReadComponentInteger(property) != 0;
        property = CharacterDefs_GetOptionalAttribute(component, g_KeyIgnorePvs, variant);
        CSF_AT(std::uint8_t, record, 0x159) =
            property && CharacterDefs_ReadComponentInteger(property) != 0;
        property = CharacterDefs_GetOptionalAttribute(component, g_KeyIgnoreBoundingSphere, variant);
        CSF_AT(std::uint8_t, record, 0x15A) =
            property && CharacterDefs_ReadComponentInteger(property) != 0;

        property = CharacterDefs_GetOptionalAttribute(component, g_KeyHeadHeight, variant);
        CSF_AT(float, record, 0xE4) = static_cast<float>(
            ReadComponentFloat(GetComponent(property, nullptr, 0)));
        CSF_AT(float, record, 0xE8) = static_cast<float>(
            ReadComponentFloat(GetComponent(property, nullptr, 1u)));
        CSF_AT(float, record, 0xEC) = static_cast<float>(
            ReadComponentFloat(GetComponent(property, nullptr, 2u)));

        property = CharacterDefs_GetOptionalAttribute(component, g_KeyViewAngle, variant);
        CSF_AT(float, record, 0x120) =
            property ? CharacterDefs_ReadComponentScalar(property) : 120.0f;
        property = CharacterDefs_GetOptionalAttribute(component, g_KeyViewDistance, variant);
        CSF_AT(float, record, 0x124) =
            property ? CharacterDefs_ReadComponentScalar(property) : 2000.0f;
        property = CharacterDefs_GetOptionalAttribute(component, g_KeyDistributorCallSound, variant);
        if (property)
            CSF_AT(int, record, 0x148) = CharacterDefs_ReadComponentInteger(property);
        property = CharacterDefs_GetOptionalAttribute(component, g_KeyLongViewDistance, variant);
        CSF_AT(float, record, 0x128) =
            property ? CharacterDefs_ReadComponentScalar(property) : 5000.0f;
        property = CharacterDefs_GetOptionalAttribute(component, g_KeyPercentage, variant);
        if (property)
            CSF_AT(int, record, 0x12C) = CharacterDefs_ReadComponentInteger(property);

        auto* damage = LookupNamedElement(component, nullptr, g_KeyDamage);
        source = CharacterDefs_GetPropertySource(damage, g_KeyHealth, variant);
        CSF_AT(int, record, 0xDC) =
            ReadIntegerAttribute(source, nullptr, CharacterDefs_Key(g_KeyHealth));
        property = CharacterDefs_GetOptionalAttribute(component, g_KeyEnergy, variant);
        CSF_AT(int, record, 0xE0) = property
            ? CharacterDefs_ReadComponentInteger(property) : CSF_AT(int, record, 0xDC);

        auto* boundingBox = FindProperty(component, nullptr, CharacterDefs_Key(g_KeyBoundingBox));
        if (boundingBox)
        {
            std::uint32_t lower[0x2C / 4] = {};
            std::uint32_t upper[3] = {};
            source = CharacterDefs_GetPropertySource(boundingBox, g_KeyLower, variant);
            DataNode_ReadVector3Property(source, nullptr, CharacterDefs_Key(g_KeyLower),
                                        reinterpret_cast<int>(lower));
            source = CharacterDefs_GetPropertySource(boundingBox, g_KeyUpper, variant);
            DataNode_ReadVector3Property(source, nullptr, CharacterDefs_Key(g_KeyUpper),
                                        reinterpret_cast<int>(upper));
            CSF_AT(std::uint32_t, record, 0x130) = upper[0];
            CSF_AT(std::uint32_t, record, 0x134) = upper[1];
            CSF_AT(std::uint32_t, record, 0x138) = upper[2];
            CSF_AT(std::uint32_t, record, 0x13C) = lower[0];
            CSF_AT(std::uint32_t, record, 0x140) = lower[1];
            CSF_AT(std::uint32_t, record, 0x144) = lower[2];
        }

        auto* collision = LookupNamedElement(component, nullptr, g_KeyCollisionModel);
        source = CharacterDefs_GetPropertySource(collision, g_KeyType, variant);
        MenuExtras_ReadText(source, nullptr, CharacterDefs_Key(g_KeyType), buffer, 256);
        CSF_AT(int, record, 0xCC) = CharacterDefs_ParseCollisionModelType(buffer);
        source = CharacterDefs_GetPropertySource(collision, g_KeySize, variant);
        DataNode_ReadVector3Property(source, nullptr, CharacterDefs_Key(g_KeySize),
                                    reinterpret_cast<int>(record + 352));
        if (FindProperty(collision, nullptr, CharacterDefs_Key(g_KeyExternalShape)))
        {
            source = CharacterDefs_GetPropertySource(collision, g_KeyExternalShape, variant);
            CSF_AT(int, record, 0x188) =
                ReadIntegerAttribute(source, nullptr, CharacterDefs_Key(g_KeyExternalShape));
        }
        else
            CSF_AT(int, record, 0x188) = 1;
        if (FindProperty(collision, nullptr, CharacterDefs_Key(g_KeyExternalShapeOffset)))
        {
            source = CharacterDefs_GetPropertySource(collision, g_KeyExternalShapeOffset, variant);
            DataNode_ReadVector3Property(source, nullptr, CharacterDefs_Key(g_KeyExternalShapeOffset),
                                        reinterpret_cast<int>(record + 396));
        }
        else
        {
            CSF_AT(int, record, 0x18C) = 0;
            CSF_AT(int, record, 0x190) = 0;
            CSF_AT(int, record, 0x194) = 0;
        }
        if (CSF_AT(int, record, 0xCC) == 1)
        {
            source = CharacterDefs_GetPropertySource(collision, g_KeyExtendedCollisionModel, variant);
            MenuExtras_ReadText(source, nullptr, CharacterDefs_Key(g_KeyExtendedCollisionModel), buffer, 256);
            CharacterDefs_AssignText(record + 364, buffer);
        }
        property = FindProperty(component, nullptr, CharacterDefs_Key(g_KeySpotlightInfo));
        if (property)
        {
            MenuExtras_ReadText(property, nullptr, CharacterDefs_Key(g_KeySpotlightTexture), buffer, 256);
            CharacterDefs_AssignText(record + 408, buffer);
        }

        auto* vehicle = FindProperty(component, nullptr, CharacterDefs_Key(g_KeyVehicleInfo));
        if (vehicle)
        {
            source = CharacterDefs_GetPropertySource(vehicle, g_KeyMaxSpeed, variant);
            CSF_AT(float, record, 0x1B4) = static_cast<float>(
                ReadFloatProperty(source, nullptr, CharacterDefs_Key(g_KeyMaxSpeed)));
            source = CharacterDefs_GetPropertySource(vehicle, g_KeyMaxReverseSpeed, variant);
            CSF_AT(float, record, 0x1B8) = static_cast<float>(
                ReadFloatProperty(source, nullptr, CharacterDefs_Key(g_KeyMaxReverseSpeed)));
            source = CharacterDefs_GetPropertySource(vehicle, g_KeyMaxAcceleration, variant);
            CSF_AT(float, record, 0x1BC) = static_cast<float>(
                ReadFloatProperty(source, nullptr, CharacterDefs_Key(g_KeyMaxAcceleration)));
            source = CharacterDefs_GetPropertySource(vehicle, g_KeyMaxDeceleration, variant);
            CSF_AT(float, record, 0x1C0) = static_cast<float>(
                ReadFloatProperty(source, nullptr, CharacterDefs_Key(g_KeyMaxDeceleration)));
            source = CharacterDefs_GetPropertySource(vehicle, g_KeyChassisHealth, variant);
            CSF_AT(int, record, 0x1C4) =
                ReadIntegerProperty(source, nullptr, CharacterDefs_Key(g_KeyChassisHealth));
            source = CharacterDefs_GetPropertySource(vehicle, g_KeyEngineHealth, variant);
            CSF_AT(int, record, 0x1C8) =
                ReadIntegerProperty(source, nullptr, CharacterDefs_Key(g_KeyEngineHealth));
            source = CharacterDefs_GetPropertySource(vehicle, g_KeyWheelHealth, variant);
            CSF_AT(int, record, 0x1CC) =
                ReadIntegerProperty(source, nullptr, CharacterDefs_Key(g_KeyWheelHealth));
            source = CharacterDefs_GetPropertySource(vehicle, g_KeyUnderbodyHealth, variant);
            CSF_AT(int, record, 0x1D0) =
                ReadIntegerProperty(source, nullptr, CharacterDefs_Key(g_KeyUnderbodyHealth));
            property = CharacterDefs_GetOptionalAttribute(vehicle, g_KeyMinTurn, variant);
            if (property)
                CSF_AT(float, record, 0x1D4) = CharacterDefs_ReadComponentScalar(property);
            property = CharacterDefs_GetOptionalAttribute(vehicle, g_KeyLowContact, variant);
            if (property)
                CSF_AT(float, record, 0x1D8) = CharacterDefs_ReadComponentScalar(property);
            property = CharacterDefs_GetOptionalAttribute(vehicle, g_KeySwayMultiplier, variant);
            if (property)
                CSF_AT(float, record, 0x1DC) = CharacterDefs_ReadComponentScalar(property);
            property = CharacterDefs_GetOptionalAttribute(vehicle, g_KeyPitchInertia, variant);
            if (property)
                CSF_AT(float, record, 0x1E0) = CharacterDefs_ReadComponentScalar(property);
            property = CharacterDefs_GetOptionalAttribute(vehicle, g_KeyRollInertia, variant);
            if (property)
                CSF_AT(float, record, 0x1E4) = CharacterDefs_ReadComponentScalar(property);
            property = CharacterDefs_GetOptionalAttribute(vehicle, g_KeyExternalPitch, variant);
            if (property)
                CSF_AT(float, record, 0x1E8) = CharacterDefs_ReadComponentScalar(property);
            property = CharacterDefs_GetOptionalAttribute(vehicle, g_KeyExternalRoll, variant);
            if (property)
                CSF_AT(float, record, 0x1EC) = CharacterDefs_ReadComponentScalar(property);
        }

        source = CharacterDefs_GetPropertySource(component, g_KeyCollisionMode, variant);
        CSF_AT(int, record, 0xD0) =
            ReadIntegerProperty(source, nullptr, CharacterDefs_Key(g_KeyCollisionMode));
        if (CSF_AT(int, record, 0xD0) == 3)
            CSF_AT(int, record, 0xD0) = 0;
        property = CharacterDefs_GetOptionalAttribute(component, g_KeyMaterial, variant);
        if (property)
            CSF_AT(int, record, 0xD4) = CharacterDefs_ReadComponentInteger(property);

        auto* physics = FindProperty(component, nullptr, CharacterDefs_Key(g_KeyPhysics));
        if (physics)
        {
            source = CharacterDefs_GetPropertySource(physics, g_KeyType, variant);
            MenuExtras_ReadText(source, nullptr, CharacterDefs_Key(g_KeyType), buffer, 256);
            CSF_AT(int, record, 0x1F0) = WeaponDefs_ParsePhysicsType(buffer);
            source = CharacterDefs_GetPropertySource(physics, g_KeyModelFile, variant);
            MenuExtras_ReadText(source, nullptr, CharacterDefs_Key(g_KeyModelFile), buffer, 256);
            CharacterDefs_AssignText(record + 500, buffer);
            source = CharacterDefs_GetPropertySource(physics, g_KeyDynamic, variant);
            CSF_AT(std::uint8_t, record, 0x21C) =
                ReadIntegerProperty(source, nullptr, CharacterDefs_Key(g_KeyDynamic)) != 0;
            source = CharacterDefs_GetPropertySource(physics, g_KeyMoving, variant);
            CSF_AT(std::uint8_t, record, 0x21D) =
                ReadIntegerProperty(source, nullptr, CharacterDefs_Key(g_KeyMoving)) != 0;
            source = CharacterDefs_GetPropertySource(physics, g_KeyMass, variant);
            CSF_AT(float, record, 0x220) = static_cast<float>(
                ReadFloatProperty(source, nullptr, CharacterDefs_Key(g_KeyMass)));
            source = CharacterDefs_GetPropertySource(physics, g_KeyBounce, variant);
            CSF_AT(float, record, 0x224) = static_cast<float>(
                ReadFloatProperty(source, nullptr, CharacterDefs_Key(g_KeyBounce)));
            source = CharacterDefs_GetPropertySource(physics, g_KeySlide, variant);
            CSF_AT(float, record, 0x228) = static_cast<float>(
                ReadFloatProperty(source, nullptr, CharacterDefs_Key(g_KeySlide)));
        }

        source = CharacterDefs_GetPropertySource(component, g_KeyBehavior, variant);
        MenuExtras_ReadText(source, nullptr, CharacterDefs_Key(g_KeyBehavior), buffer, 256);
        CSF_AT(int, record, 0xD8) = CharacterDefs_ParseBehavior(buffer);
        property = FindProperty(component, nullptr, CharacterDefs_Key(g_KeyAnimations));
        CharacterDefs_ReadAnimations(self, nullptr, property, reinterpret_cast<int>(record),
                                     record + 240, variant);

        property = CharacterDefs_GetOptionalAttribute(component, g_KeyWeapons, variant);
        if (property)
        {
            const auto weaponCount = MenuExtras_GetChildCount(property, nullptr);
            const auto capacity = CSF_AT(std::uint32_t, record, 0x104);
            if (weaponCount > capacity)
                GrowButtonArray(record + 256, nullptr, static_cast<int>(weaponCount - capacity));
            for (unsigned int weapon = 0; weapon < weaponCount; ++weapon)
            {
                const int value = ParseIntegerComponent(GetComponent(property, nullptr, weapon));
                CharacterDefs_AppendInteger(record + 256, value);
            }
        }
        property = CharacterDefs_GetOptionalAttribute(component, g_KeyItems, variant);
        if (property)
        {
            const auto itemCount = MenuExtras_GetChildCount(property, nullptr);
            const auto capacity = CSF_AT(std::uint32_t, record, 0x114);
            if (itemCount > capacity)
            {
                const auto growth = itemCount - capacity;
                void* replacement = Mem_Alloc(4 * itemCount);
                const auto size = CSF_AT(std::uint32_t, record, 0x118);
                if (size)
                    std::memcpy(replacement, CSF_AT(void*, record, 0x110), 4 * size);
                void* old = CSF_AT(void*, record, 0x110);
                CSF_AT(std::uint32_t, record, 0x114) += growth;
                if (old)
                {
                    Mem_Free(old);
                    CSF_AT(void*, record, 0x110) = nullptr;
                }
                CSF_AT(void*, record, 0x110) = replacement;
            }
            for (unsigned int item = 0; item < itemCount; ++item)
            {
                const int value = ParseIntegerComponent(GetComponent(property, nullptr, item));
                CharacterDefs_AppendInteger(record + 272, value);
            }
        }

        property = FindProperty(component, nullptr, CharacterDefs_Key(g_KeyShadow));
        if (property)
            CSF_AT(std::uint8_t, record, 0x15B) = CharacterDefs_ReadComponentInteger(property) != 0;
        property = FindProperty(component, nullptr, CharacterDefs_Key(g_KeyRespawnTime));
        if (property)
            CSF_AT(float, record, 0x14C) = CharacterDefs_ReadComponentScalar(property);
        property = FindProperty(component, nullptr, CharacterDefs_Key(g_KeyExplosionDamage));
        if (property)
            CSF_AT(int, record, 0x150) = CharacterDefs_ReadComponentInteger(property);
        property = FindProperty(component, nullptr, CharacterDefs_Key(g_KeyExplosionRadius));
        if (property)
            CSF_AT(float, record, 0x154) = CharacterDefs_ReadComponentScalar(property);
    }
    return 1;
}

HOOK(0x00766F10, C_0086f128_ReadCharacterDefinitions, "auto");
