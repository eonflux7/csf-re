#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, IntVector_FillUninitialized, 0x0046BFB0, (std::uint32_t* destination, std::uint32_t count, const std::uint32_t* source, void* context));
CSF_FUNC(unsigned int, __fastcall, IntVector_InsertN, 0x0046C940, (void* self, void* edx, char* Src, unsigned int a3, uint32_t* a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0, (std::uint32_t component));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40, (std::uint8_t* attribute, void* edx, char* buffer, int capacity));
CSF_FUNC(int, __fastcall, DataNode_ReadVector3, 0x00693020, (void* self, void* edx, float* a2));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __fastcall, DataNode_ReadVector3Property, 0x00693A60, (void* self, void* edx, char* String1, int a3));
CSF_FUNC(float*, __fastcall, S_006abb80_InitializeBounds, 0x006ABB80, (S_006abb80* self, void* , float xFirst, float xSecond, float yFirst, float ySecond, float zFirst, float zSecond));
CSF_FUNC(int, __fastcall, C_0086c288_GetGroupPoint, 0x0075CFB0, (C_0086c288* self, void* , int a2, int a3));
CSF_FUNC(int, __cdecl, CharacterDefs_ParseAlliedFactionIndex, 0x00761B20, (char* String1));
CSF_FUNC(char, __fastcall, CLevelData_ReadEntityAnimations, 0x00774C70, (void* self, void* edx, void* a2, int* a3, void* a4));
CSF_FUNC(unsigned int, __fastcall, EffectList_Reserve, 0x007760B0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, ControlPointVector_PushBack, 0x00776BC0, (void* self, void* edx, uint32_t* a3));
CSF_FUNC(uint32_t*, __fastcall, EffectDefVector_PushBack, 0x00776C50, (void* self, void* edx, int a3));
CSF_FUNC(uint32_t*, __fastcall, S_00776ff0_InitDefaults, 0x00776FF0, (S_00776ff0* self, void*));
CSF_FUNC(int, __fastcall, C_0082d410_ReleaseBuffersAndReset, 0x00778140, (C_0082d410* self, void*));
CSF_FUNC(std::uint32_t, __fastcall, CLevelData_LoadWaters, 0x007781E0, (void* self, void* , std::uint8_t* rootNode));
CSF_FUNC(void, __fastcall, CLevelData_LoadBichoDefinitions, 0x00778BB0, (CLevelData* self, void* , S_00692b80* node));
CSF_FUNC(uint32_t*, __fastcall, GameDataDefs_AppendElement72, 0x0077D560, (S_0077d4b0* self, void* , int element));

CSF_GLOBAL(const char*, g_pszKey_PLAYER, 0x008A99EC);
CSF_GLOBAL(const char*, g_pszKey_SCRIPT, 0x008A99F0);
CSF_GLOBAL(const char*, g_pszKey_ANIMACIONES_game_data_defs, 0x008A99F4);
CSF_GLOBAL(const char*, g_pszKey_MUNDOVIS, 0x008A9A14);
CSF_GLOBAL(const char*, g_pszKey_RWS, 0x008A9A18);
CSF_GLOBAL(const char*, g_pszKey_SIMPLIFICADO, 0x008A9A1C);
CSF_GLOBAL(const char*, g_pszKey_TEXTURAS, 0x008A9A20);
CSF_GLOBAL(const char*, g_pszKey_SONIDO_AMBIENTE, 0x008A9A2C);
CSF_GLOBAL(const char*, g_pszKey_CIELO, 0x008A9A30);
CSF_GLOBAL(const char*, g_pszKey_CIELOSPEED1, 0x008A9A34);
CSF_GLOBAL(const char*, g_pszKey_CIELOSPEED2, 0x008A9A38);
CSF_GLOBAL(const char*, g_pszKey_CIELOSPEED3, 0x008A9A3C);
CSF_GLOBAL(const char*, g_pszKey_CIELO_MODULATE2X, 0x008A9A40);
CSF_GLOBAL(const char*, g_pszKey_MODULATE2X, 0x008A9A44);
CSF_GLOBAL(const char*, g_pszKey_AMBIENTE, 0x008A9A48);
CSF_GLOBAL(const char*, g_pszKey_AMBESCENA, 0x008A9A4C);
CSF_GLOBAL(const char*, g_pszKey_VIEWPORT_AMB_ADD, 0x008A9A50);
CSF_GLOBAL(const char*, g_pszKey_CAMERANEAR, 0x008A9A54);
CSF_GLOBAL(const char*, g_pszKey_CAMERAFAR, 0x008A9A58);
CSF_GLOBAL(const char*, g_pszKey_CAMERAFOV, 0x008A9A5C);
CSF_GLOBAL(const char*, g_pszKey_FOGENABLED, 0x008A9A60);
CSF_GLOBAL(const char*, g_pszKey_FOGTYPE, 0x008A9A64);
CSF_GLOBAL(const char*, g_pszKey_FOGCOLOR, 0x008A9A68);
CSF_GLOBAL(const char*, g_pszKey_FOGDENSITY, 0x008A9A70);
CSF_GLOBAL(const char*, g_pszKey_FOGDISTANCE, 0x008A9A74);
CSF_GLOBAL(const char*, g_pszKey_ATMOSFERICFX, 0x008A9A78);
CSF_GLOBAL(const char*, g_pszKey_VAHOENABLED, 0x008A9A7C);
CSF_GLOBAL(const char*, g_pszKey_BICHOS, 0x008A9A80);
CSF_GLOBAL(const char*, g_pszKey_EFECTOS_game_data_defs, 0x008A9A84);
CSF_GLOBAL(const char*, g_pszKey_CLASSID, 0x008A9A8C);
CSF_GLOBAL(const char*, g_pszKey_ID_8A9A90, 0x008A9A90);
CSF_GLOBAL(const char*, g_pszKey_NOMBRE_game_data_defs, 0x008A9A94);
CSF_GLOBAL(const char*, g_pszKey_POS_game_data_defs, 0x008A9A98);
CSF_GLOBAL(const char*, g_pszKey_CELDA, 0x008A9A9C);
CSF_GLOBAL(const char*, g_pszKey_GRUPO, 0x008A9AA0);
CSF_GLOBAL(const char*, g_pszKey_PUNTO, 0x008A9AA4);
CSF_GLOBAL(const char*, g_pszKey_ANGULO, 0x008A9AA8);
CSF_GLOBAL(const char*, g_pszKey_ANGULO_X, 0x008A9AAC);
CSF_GLOBAL(const char*, g_pszKey_COLISION, 0x008A9AB0);
CSF_GLOBAL(const char*, g_pszKey_FLAGS_game_data_defs, 0x008A9AB4);
CSF_GLOBAL(const char*, g_pszKey_GRUPOS_BICHOS, 0x008A9ABC);
CSF_GLOBAL(const char*, g_pszKey_DUMMY, 0x008A9AC0);
CSF_GLOBAL(const char*, g_pszKey_PRIORITY, 0x008A9AC4);
CSF_GLOBAL(const char*, g_pszKey_SHARE_GROUP, 0x008A9AC8);
CSF_GLOBAL(const char*, g_pszKey_MINIMAPA_RTS_game_data_defs, 0x008A9ACC);
CSF_GLOBAL(const char*, g_pszKey_RADIO_RTS, 0x008A9AD0);
CSF_GLOBAL(const char*, g_pszKey_TEXTURA_RTS, 0x008A9AD4);
CSF_GLOBAL(const char*, g_pszKey_ORIGEN_RTS, 0x008A9AD8);
CSF_GLOBAL(const char*, g_pszKey_DESTINO_RTS, 0x008A9ADC);
CSF_GLOBAL(const char*, g_pszKey_MTACTICO_ORIGEN, 0x008A9AE0);
CSF_GLOBAL(const char*, g_pszKey_MTACTICO_DESTINO, 0x008A9AE4);
CSF_GLOBAL(const char*, g_pszKey_MTACTICO_TEXTURA, 0x008A9AE8);
CSF_GLOBAL(const char*, g_pszKey_FLIS_ESCENARIO, 0x008A9AEC);
CSF_GLOBAL(const char*, g_pszKey_BRIDGES, 0x008A9AF0);
CSF_GLOBAL(const char*, g_pszKey_VISUALRWS, 0x008A9AF4);
CSF_GLOBAL(const char*, g_pszKey_PHYSICRWS, 0x008A9AF8);
CSF_GLOBAL(const char*, g_pszKey_CONTROL_POINTS, 0x008A9AFC);
CSF_GLOBAL(const char*, g_pszKey_TYPE, 0x008A9B00);
CSF_GLOBAL(const char*, g_pszKey_P1, 0x008A9B04);
CSF_GLOBAL(const char*, g_pszKey_P2, 0x008A9B08);
CSF_GLOBAL(const char*, g_pszKey_HEIGHT_game_data_defs, 0x008A9B0C);
CSF_GLOBAL(const char*, g_pszKey_SCN, 0x008A9B10);
CSF_GLOBAL(const char*, g_pszKey_SAVEGAME_DESCR, 0x008A9B14);
CSF_GLOBAL(const char*, g_pszKey_SAVEGAME_SCENE, 0x008A9B18);
CSF_GLOBAL(const char*, g_pszKey_GLOW_ON, 0x008A9B1C);
CSF_GLOBAL(const char*, g_pszKey_GLOW_CUTCOLOR, 0x008A9B20);
CSF_GLOBAL(const char*, g_pszKey_GLOW_INTENSITY, 0x008A9B24);
CSF_GLOBAL(const char*, g_pszKey_GLOW_DISPLACEMENT, 0x008A9B28);
CSF_GLOBAL(const char*, g_pszKey_VIDEO_OPENING, 0x008A9B34);
CSF_GLOBAL(const char*, g_pszKey_PUNTERIA, 0x008A9B38);
CSF_GLOBAL(const char*, g_pszKey_MULTIPLAYER, 0x008A9B3C);
CSF_GLOBAL(const char*, g_pszKey_MPDEATHMATCH, 0x008A9B40);
CSF_GLOBAL(const char*, g_pszKey_MPTDEATHMATCH, 0x008A9B44);
CSF_GLOBAL(const char*, g_pszKey_MPPOSTMEN, 0x008A9B48);
CSF_GLOBAL(const char*, g_pszKey_MPPOSTMEN_IDPOSTMAN, 0x008A9B4C);
CSF_GLOBAL(const char*, g_pszKey_MPFRANCOS, 0x008A9B50);
CSF_GLOBAL(const char*, g_pszKey_INVERTIR_NORTE_SUR, 0x008A9B5C);
CSF_GLOBAL(const char*, g_pszKey_DOOR_BOX, 0x008A9B60);
CSF_GLOBAL(const char*, g_pszKey_PORTRAIT, 0x008A9B64);
CSF_GLOBAL(const char*, g_pszKey_SEGUNDA_EXPLOSION, 0x008A9B68);
CSF_GLOBAL(const char*, g_pszKey_BANDO, 0x008A9B6C);
CSF_GLOBAL(const char*, g_pszKey_GRUPO_VISUAL, 0x008A9B70);
CSF_GLOBAL(const char*, g_pszKey_AGUA_FOGTYPE, 0x008A9BB8);
CSF_GLOBAL(const char*, g_pszKey_AGUA_FOGCOLOR, 0x008A9BBC);
CSF_GLOBAL(const char*, g_pszKey_AGUA_FOGDENSITY, 0x008A9BC4);
CSF_GLOBAL(const char*, g_pszKey_AGUA_FOGDISTANCE, 0x008A9BC8);

// The packet also labels these direct-call destinations as vtable references.
static void* const g_NodeCallTable = CSF_VTBL(0x00692B80);
static void* const g_ArrayGrowthCallTable = CSF_VTBL(0x0041E5F0);
static void* const g_EntityDefaultsCallTable = CSF_VTBL(0x00776FF0);
static void* const g_BoundsInitializationCallTable = CSF_VTBL(0x006ABB80);

static void GameDataDefs_ReadString(
    std::uint8_t* node, const char* key, void* destination, char* buffer)
{
    MenuExtras_ReadText(node, nullptr, const_cast<char*>(key), buffer, 256);
    AssignStringRange_429B60(
        destination, nullptr, buffer, static_cast<unsigned int>(std::strlen(buffer)));
}

#define KEY(name) const_cast<char*>(g_pszKey_##name)
#define LEVEL_INT(offset, key) \
    CSF_AT(int, self, offset) = ReadIntegerProperty(world, nullptr, KEY(key))
#define LEVEL_FLOAT(offset, key) \
    CSF_AT(float, self, offset) = static_cast<float>(ReadFloatProperty(world, nullptr, KEY(key)))
#define LEVEL_FLAG(offset, key) \
    CSF_AT(std::uint8_t, self, offset) = ReadIntegerProperty(world, nullptr, KEY(key)) == 1
#define LEVEL_STRING(offset, key) \
    GameDataDefs_ReadString(world, g_pszKey_##key, bytes + offset, buffer)

static int __fastcall CLevelData_LoadV15to16(
    CLevelData* self, void* /*edx*/, S_00692b80* rootNode)
{
    auto* root = reinterpret_cast<std::uint8_t*>(rootNode);
    auto* bytes = reinterpret_cast<std::uint8_t*>(self);
    alignas(8) std::uint8_t bufferBytes[0x10C];
    char* buffer = reinterpret_cast<char*>(bufferBytes);
    std::memset(buffer, 0, 256);

    CSF_AT(int, self, 0x64) = ReadIntegerProperty(root, nullptr, KEY(PLAYER));
    auto* groups = FindProperty(root, nullptr, KEY(GRUPOS_BICHOS));
    CLevelData_LoadBichoDefinitions(
        self, nullptr, reinterpret_cast<S_00692b80*>(groups));

    auto* world = LookupNamedElement(root, nullptr, g_pszKey_MUNDOVIS);
    LEVEL_STRING(0x1F54, RWS);
    if (FindProperty(world, nullptr, KEY(SIMPLIFICADO)))
        LEVEL_STRING(0x8D4, SIMPLIFICADO);
    LEVEL_STRING(0x1F70, TEXTURAS);
    LEVEL_INT(0x68, SONIDO_AMBIENTE);
    LEVEL_STRING(0x488, CIELO);
    if (FindProperty(world, nullptr, KEY(CIELOSPEED1))) {
        LEVEL_FLOAT(0x4A8, CIELOSPEED1);
        LEVEL_FLOAT(0x4AC, CIELOSPEED2);
        LEVEL_FLOAT(0x4B0, CIELOSPEED3);
    }
    if (FindProperty(world, nullptr, KEY(CIELO_MODULATE2X)))
        LEVEL_FLAG(0x4B4, CIELO_MODULATE2X);

    LEVEL_FLAG(0x8F4, MODULATE2X);
    LEVEL_INT(0x8F8, AMBESCENA);
    LEVEL_INT(0x8FC, AMBIENTE);
    LEVEL_FLOAT(0x900, VIEWPORT_AMB_ADD);
    LEVEL_FLOAT(0x904, CAMERANEAR);
    LEVEL_FLOAT(0x908, CAMERAFAR);
    if (FindProperty(world, nullptr, KEY(CAMERAFOV)))
        LEVEL_FLOAT(0x90C, CAMERAFOV);
    else
        CSF_AT(float, self, 0x90C) = -1.0f;

    LEVEL_FLAG(0x910, FOGENABLED);
    LEVEL_INT(0x914, FOGTYPE);
    LEVEL_INT(0x928, FOGCOLOR);
    LEVEL_FLOAT(0x930, FOGDENSITY);
    LEVEL_FLOAT(0x934, FOGDISTANCE);
    if (FindProperty(world, nullptr, KEY(AGUA_FOGTYPE))) {
        LEVEL_INT(0x938, AGUA_FOGTYPE);
        LEVEL_INT(0x94C, AGUA_FOGCOLOR);
        LEVEL_FLOAT(0x954, AGUA_FOGDENSITY);
        LEVEL_FLOAT(0x958, AGUA_FOGDISTANCE);
    }
    LEVEL_INT(0x95C, ATMOSFERICFX);
    LEVEL_FLAG(0x970, VAHOENABLED);
    LEVEL_FLAG(0x972, MINIMAPA_RTS_game_data_defs);
    LEVEL_FLOAT(0x974, RADIO_RTS);
    LEVEL_STRING(0xD94, TEXTURA_RTS);
    DataNode_ReadVector3Property(
        world, nullptr, KEY(ORIGEN_RTS), reinterpret_cast<int>(bytes + 0xDB4));
    DataNode_ReadVector3Property(
        world, nullptr, KEY(DESTINO_RTS), reinterpret_cast<int>(bytes + 0xDC0));
    DataNode_ReadVector3Property(
        world, nullptr, KEY(MTACTICO_ORIGEN), reinterpret_cast<int>(bytes + 0xDD0));
    DataNode_ReadVector3Property(
        world, nullptr, KEY(MTACTICO_DESTINO), reinterpret_cast<int>(bytes + 0xDDC));
    LEVEL_STRING(0x1204, MTACTICO_TEXTURA);

    if (FindProperty(world, nullptr, KEY(GLOW_ON))) {
        LEVEL_FLAG(0x1F40, GLOW_ON);
        LEVEL_INT(0x1F44, GLOW_CUTCOLOR);
        LEVEL_INT(0x1F48, GLOW_INTENSITY);
        LEVEL_INT(0x1F4C, GLOW_DISPLACEMENT);
    }
    LEVEL_STRING(0x1640, FLIS_ESCENARIO);
    LEVEL_STRING(0x1A9C, SAVEGAME_DESCR);
    LEVEL_STRING(0x1AB8, SAVEGAME_SCENE);
    LEVEL_FLAG(0x1F35, INVERTIR_NORTE_SUR);
    LEVEL_STRING(0x1EF0, VIDEO_OPENING);
    if (auto* aiming = FindProperty(world, nullptr, KEY(PUNTERIA))) {
        CopyAttributeText(aiming, nullptr, buffer, 256);
        AssignStringRange_429B60(
            bytes + 0x1F10, nullptr, buffer,
            static_cast<unsigned int>(std::strlen(buffer)));
    }

    auto* entities = FindProperty(root, nullptr, KEY(BICHOS));
    const std::uint32_t entityCount = MenuExtras_GetChildCount(entities, nullptr);
    if (entityCount > self->field_0008) {
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(&self->field_0004), nullptr,
            static_cast<int>(entityCount - self->field_0008));
    }

    for (std::uint32_t entityIndex = 0; entityIndex < entityCount; ++entityIndex) {
        auto* component = reinterpret_cast<std::uint8_t*>(
            GetComponent(entities, nullptr, entityIndex));
        const int classId = ReadIntegerAttribute(component, nullptr, KEY(CLASSID));
        auto* classTable = reinterpret_cast<void*>(self->m_entityClassTable);
        const int definitionIndex =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, classTable, 0)[0x10 / 4])(
                    classTable, nullptr, classId);
        if (definitionIndex == -1)
            continue;

        classTable = reinterpret_cast<void*>(self->m_entityClassTable);
        auto* definition = reinterpret_cast<std::uint8_t*>(
            reinterpret_cast<std::uint32_t(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, classTable, 0)[0x8 / 4])(
                    classTable, nullptr, definitionIndex));

        auto* allocation = static_cast<S_00776ff0*>(Mem_Alloc(232));
        auto* entity = allocation ? S_00776ff0_InitDefaults(allocation, nullptr) : nullptr;
        auto* entityBytes = reinterpret_cast<std::uint8_t*>(entity);
        entity[0] = classId;
        entity[1] = ReadIntegerAttribute(component, nullptr, KEY(ID_8A9A90));
        MenuExtras_ReadText(
            component, nullptr, KEY(NOMBRE_game_data_defs), buffer, 256);
        if (buffer[0]) {
            AssignStringRange_429B60(
                entityBytes + 0x8, nullptr, buffer,
                static_cast<unsigned int>(std::strlen(buffer)));
        } else {
            AssignStringSubstring(
                entityBytes + 0x8, nullptr, definition + 32, 0, -1);
        }

        alignas(8) std::uint8_t positionBytes[0x24];
        std::memset(positionBytes, 0, 12);
        if (auto* position = LookupNamedElement(
                component, nullptr, g_pszKey_POS_game_data_defs)) {
            DataNode_ReadVector3(
                position, nullptr, reinterpret_cast<float*>(positionBytes));
        }

        auto* cell = FindProperty(component, nullptr, KEY(CELDA));
        if (CSF_AT(int, definition, 0xB4) != 26 && cell) {
            entity[56] = ReadIntegerAttribute(cell, nullptr, KEY(GRUPO));
            const int pointIndex = ReadIntegerAttribute(cell, nullptr, KEY(PUNTO));
            entity[57] = pointIndex;
            if (pointIndex != -1) {
                const int point = C_0086c288_GetGroupPoint(
                    reinterpret_cast<C_0086c288*>(self->m_navMesh), nullptr,
                    static_cast<int>(entity[56]), pointIndex);
                auto* pointBytes = reinterpret_cast<std::uint8_t*>(point);
                CSF_AT(std::uint32_t, positionBytes, 0x0) =
                    CSF_AT(std::uint32_t, pointBytes, 0x4);
                CSF_AT(std::uint32_t, positionBytes, 0x4) =
                    CSF_AT(std::uint32_t, pointBytes, 0x8);
                CSF_AT(std::uint32_t, positionBytes, 0x8) =
                    CSF_AT(std::uint32_t, pointBytes, 0xC);
            }
        }

        entity[55] = ReadIntegerProperty(component, nullptr, KEY(FLAGS_game_data_defs));
        entity[11] = CSF_AT(std::uint32_t, positionBytes, 0x0);
        entity[12] = CSF_AT(std::uint32_t, positionBytes, 0x4);
        entity[13] = CSF_AT(std::uint32_t, positionBytes, 0x8);
        CSF_AT(float, entityBytes, 0x78) =
            static_cast<float>(ReadFloatAttribute(component, nullptr, KEY(ANGULO)));
        CSF_AT(std::uint8_t, entityBytes, 0x7C) =
            ReadIntegerAttribute(component, nullptr, KEY(COLISION)) == 1;

        if (auto* faction = FindProperty(component, nullptr, KEY(BANDO))) {
            CopyAttributeText(faction, nullptr, buffer, 256);
            entity[9] = CharacterDefs_ParseAlliedFactionIndex(buffer);
        } else {
            entity[9] = -1;
        }
        GameDataDefs_ReadString(
            component, g_pszKey_PORTRAIT, entityBytes + 0xA0, buffer);
        if (auto* explosion = FindProperty(component, nullptr, KEY(SEGUNDA_EXPLOSION))) {
            CSF_AT(std::uint8_t, entityBytes, 0xBC) =
                ParseIntegerComponent(reinterpret_cast<std::uint32_t>(explosion)) != 0;
        }
        GameDataDefs_ReadString(
            component, g_pszKey_GRUPO_VISUAL, entityBytes + 0xC0, buffer);

        if (auto* doorBox = FindProperty(component, nullptr, KEY(DOOR_BOX))) {
            alignas(8) std::uint8_t firstCornerBytes[0xC] = {};
            alignas(8) std::uint8_t secondCornerBytes[0xC] = {};
            auto* firstCorner = reinterpret_cast<float*>(firstCornerBytes);
            auto* secondCorner = reinterpret_cast<float*>(secondCornerBytes);
            auto* firstNode = reinterpret_cast<void*>(GetComponent(doorBox, nullptr, 0));
            DataNode_ReadVector3(firstNode, nullptr, firstCorner);
            auto* secondNode = reinterpret_cast<void*>(GetComponent(doorBox, nullptr, 1u));
            DataNode_ReadVector3(secondNode, nullptr, secondCorner);
            S_006abb80_InitializeBounds(
                reinterpret_cast<S_006abb80*>(entityBytes + 0x38), nullptr,
                firstCorner[0], secondCorner[0],
                firstCorner[1], secondCorner[1],
                firstCorner[2], secondCorner[2]);
        }
        if (auto* angle = FindProperty(component, nullptr, KEY(ANGULO_X))) {
            CSF_AT(float, entityBytes, 0x74) = static_cast<float>(
                ReadComponentFloat(reinterpret_cast<std::uint32_t>(angle)));
        }

        if (auto* script = FindProperty(component, nullptr, KEY(SCRIPT))) {
            const std::uint32_t scriptCount = MenuExtras_GetChildCount(script, nullptr);
            for (std::uint32_t scriptIndex = 0; scriptIndex < scriptCount; ++scriptIndex) {
                const std::uint32_t scriptComponent =
                    GetComponent(script, nullptr, scriptIndex);
                std::uint32_t scriptId = ParseIntegerComponent(scriptComponent);
                const std::uint32_t begin = entity[33];
                const std::uint32_t size = begin ? (entity[34] - begin) >> 2 : 0;
                if (begin && size < (entity[35] - begin) >> 2) {
                    const std::uint32_t end = entity[34];
                    IntVector_FillUninitialized(
                        reinterpret_cast<std::uint32_t*>(end), 1, &scriptId,
                        entityBytes + 0x80);
                    entity[34] = end + 4;
                } else {
                    IntVector_InsertN(
                        entityBytes + 0x80, nullptr,
                        reinterpret_cast<char*>(entity[34]), 1u, &scriptId);
                }
            }
        }

        auto* animations =
            FindProperty(component, nullptr, KEY(ANIMACIONES_game_data_defs));
        CLevelData_ReadEntityAnimations(
            self, nullptr, animations, reinterpret_cast<int*>(entity),
            entityBytes + 0x90);

        if (self->field_000c >= self->field_0008) {
            const std::uint32_t growth = self->field_0010;
            void* newStorage = Mem_Alloc(4 * (growth + self->field_0008));
            const std::uint32_t count = self->field_000c;
            if (count) {
                std::memcpy(
                    newStorage, reinterpret_cast<void*>(self->field_0004), 4 * count);
            }
            const std::uint32_t oldStorage = self->field_0004;
            self->field_0008 += growth;
            if (oldStorage) {
                Mem_Free(reinterpret_cast<void*>(oldStorage));
                self->field_0004 = 0;
            }
            self->field_0004 = reinterpret_cast<std::uint32_t>(newStorage);
        }
        const std::uint32_t count = self->field_000c;
        auto* destination = reinterpret_cast<std::uint32_t*>(
            self->field_0004 + 4 * count);
        self->field_000c = count + 1;
        *destination = reinterpret_cast<std::uint32_t>(entity);
    }

    if (auto* effects = FindProperty(root, nullptr, KEY(EFECTOS_game_data_defs))) {
        const std::uint32_t effectCount = MenuExtras_GetChildCount(effects, nullptr);
        EffectList_Reserve(bytes + 0x14, nullptr, effectCount);
        for (std::uint32_t effectIndex = 0; effectIndex < effectCount; ++effectIndex) {
            auto* effectNode = reinterpret_cast<std::uint8_t*>(
                GetComponent(effects, nullptr, effectIndex));
            // String, id, class, dummy, priority, sharing group, and three flags.
            alignas(8) std::uint8_t effectBytes[0x34] = {};
            CSF_AT(std::uint32_t, effectBytes, 0x18) = 15;
            CSF_AT(int, effectBytes, 0x1C) = -1;
            CSF_AT(int, effectBytes, 0x20) = -1;
            CSF_AT(int, effectBytes, 0x24) = -1;
            CSF_AT(int, effectBytes, 0x28) = -1;
            CSF_AT(int, effectBytes, 0x2C) = -1;

            GameDataDefs_ReadString(
                effectNode, g_pszKey_NOMBRE_game_data_defs, effectBytes, buffer);
            CSF_AT(int, effectBytes, 0x1C) =
                ReadIntegerAttribute(effectNode, nullptr, KEY(ID_8A9A90));
            CSF_AT(int, effectBytes, 0x20) =
                ReadIntegerAttribute(effectNode, nullptr, KEY(CLASSID));
            CSF_AT(int, effectBytes, 0x24) =
                ReadIntegerAttribute(effectNode, nullptr, KEY(DUMMY));
            if (FindProperty(effectNode, nullptr, KEY(PRIORITY))) {
                CSF_AT(int, effectBytes, 0x28) =
                    ReadIntegerAttribute(effectNode, nullptr, KEY(PRIORITY));
            }
            if (FindProperty(effectNode, nullptr, KEY(SHARE_GROUP))) {
                CSF_AT(int, effectBytes, 0x2C) =
                    ReadIntegerAttribute(effectNode, nullptr, KEY(SHARE_GROUP));
            }

            auto* effectTable = reinterpret_cast<void*>(self->m_effectClassTable);
            if (reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, effectTable, 0)[0x10 / 4])(
                        effectTable, nullptr, CSF_AT(int, effectBytes, 0x20)) != -1) {
                EffectDefVector_PushBack(
                    bytes + 0x14, nullptr, reinterpret_cast<int>(effectBytes));
            }
            if (CSF_AT(std::uint32_t, effectBytes, 0x18) >= 0x10 &&
                CSF_AT(std::uint32_t, effectBytes, 0x4)) {
                Mem_FreeDebug(
                    reinterpret_cast<void*>(CSF_AT(std::uint32_t, effectBytes, 0x4)));
            }
            CSF_AT(std::uint32_t, effectBytes, 0x18) = 15;
            CSF_AT(std::uint32_t, effectBytes, 0x14) = 0;
            CSF_AT(std::uint8_t, effectBytes, 0x4) = 0;
        }
    }

    CLevelData_LoadWaters(self, nullptr, root);
    if (auto* multiplayer = FindProperty(root, nullptr, KEY(MULTIPLAYER))) {
        CSF_AT(std::uint8_t, self, 0x1F2C) =
            ReadIntegerProperty(multiplayer, nullptr, KEY(MPDEATHMATCH)) != 0;
        CSF_AT(std::uint8_t, self, 0x1F2D) =
            ReadIntegerProperty(multiplayer, nullptr, KEY(MPTDEATHMATCH)) != 0;
        CSF_AT(std::uint8_t, self, 0x1F2E) =
            ReadIntegerProperty(multiplayer, nullptr, KEY(MPPOSTMEN)) != 0;
        CSF_AT(int, self, 0x1F30) =
            ReadIntegerProperty(multiplayer, nullptr, KEY(MPPOSTMEN_IDPOSTMAN));
        CSF_AT(std::uint8_t, self, 0x1F34) =
            ReadIntegerProperty(multiplayer, nullptr, KEY(MPFRANCOS)) != 0;
    }

    if (auto* bridges = FindProperty(root, nullptr, KEY(BRIDGES))) {
        const std::uint32_t bridgeCount = MenuExtras_GetChildCount(bridges, nullptr);
        for (std::uint32_t bridgeIndex = 0; bridgeIndex < bridgeCount; ++bridgeIndex) {
            // Two contiguous 0x1C-byte strings and the 0x10-byte point vector.
            alignas(8) std::uint8_t bridgeBytes[0x48] = {};
            CSF_AT(std::uint32_t, bridgeBytes, 0x18) = 15;
            CSF_AT(std::uint32_t, bridgeBytes, 0x34) = 15;
            auto* bridgeNode = reinterpret_cast<std::uint8_t*>(
                GetComponent(bridges, nullptr, bridgeIndex));
            GameDataDefs_ReadString(
                bridgeNode, g_pszKey_VISUALRWS, bridgeBytes, buffer);
            GameDataDefs_ReadString(
                bridgeNode, g_pszKey_PHYSICRWS, bridgeBytes + 0x1C, buffer);

            if (auto* points = FindProperty(bridgeNode, nullptr, KEY(CONTROL_POINTS))) {
                const std::uint32_t pointCount = MenuExtras_GetChildCount(points, nullptr);
                for (std::uint32_t pointIndex = 0; pointIndex < pointCount; ++pointIndex) {
                    // Type, two vectors, height, and a 0x1C-byte scene string.
                    alignas(8) std::uint8_t pointBytes[0x3C] = {};
                    CSF_AT(std::uint32_t, pointBytes, 0x38) = 15;
                    auto* pointNode = reinterpret_cast<std::uint8_t*>(
                        GetComponent(points, nullptr, pointIndex));
                    CSF_AT(int, pointBytes, 0x0) =
                        ReadIntegerProperty(pointNode, nullptr, KEY(TYPE));
                    DataNode_ReadVector3Property(
                        pointNode, nullptr, KEY(P1),
                        reinterpret_cast<int>(pointBytes + 0x4));
                    DataNode_ReadVector3Property(
                        pointNode, nullptr, KEY(P2),
                        reinterpret_cast<int>(pointBytes + 0x10));
                    CSF_AT(float, pointBytes, 0x1C) = static_cast<float>(
                        ReadFloatProperty(pointNode, nullptr, KEY(HEIGHT_game_data_defs)));
                    GameDataDefs_ReadString(
                        pointNode, g_pszKey_SCN, pointBytes + 0x20, buffer);
                    ControlPointVector_PushBack(
                        bridgeBytes + 0x38, nullptr,
                        reinterpret_cast<std::uint32_t*>(pointBytes));
                    if (CSF_AT(std::uint32_t, pointBytes, 0x38) >= 0x10 &&
                        CSF_AT(std::uint32_t, pointBytes, 0x24)) {
                        Mem_FreeDebug(reinterpret_cast<void*>(
                            CSF_AT(std::uint32_t, pointBytes, 0x24)));
                    }
                    CSF_AT(std::uint32_t, pointBytes, 0x38) = 15;
                    CSF_AT(std::uint32_t, pointBytes, 0x34) = 0;
                    CSF_AT(std::uint8_t, pointBytes, 0x24) = 0;
                }
                GameDataDefs_AppendElement72(
                    reinterpret_cast<S_0077d4b0*>(bytes + 0x24), nullptr,
                    reinterpret_cast<int>(bridgeBytes));
            }
            C_0082d410_ReleaseBuffersAndReset(
                reinterpret_cast<C_0082d410*>(bridgeBytes), nullptr);
        }
    }

    if (CSF_AT(int, self, 0x64) == -1 && self->field_000c) {
        auto* firstEntity = reinterpret_cast<std::uint8_t*>(
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_0004), 0));
        CSF_AT(int, self, 0x64) = CSF_AT(int, firstEntity, 0x4);
    }
    return 1;
}

#undef LEVEL_STRING
#undef LEVEL_FLAG
#undef LEVEL_FLOAT
#undef LEVEL_INT
#undef KEY

HOOK(0x0077FB70, CLevelData_LoadV15to16, "auto");
