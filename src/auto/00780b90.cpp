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
CSF_GLOBAL(const char*, g_pszKey_INICIO_COMMANDO, 0x008A9BCC);
CSF_GLOBAL(const char*, g_pszKey_INICIO_SNIPER, 0x008A9BD0);
CSF_GLOBAL(const char*, g_pszKey_INICIO_SPY, 0x008A9BD4);
CSF_GLOBAL(const char*, g_pszKey_ELIMINAR_MUERTOS, 0x008A9BD8);

static std::uint8_t* GameDataDefs_FindProperty(std::uint8_t* node, const char* key)
{
    return FindProperty(node, nullptr, const_cast<char*>(key));
}

static int GameDataDefs_ReadInteger(std::uint8_t* node, const char* key)
{
    return ReadIntegerProperty(node, nullptr, const_cast<char*>(key));
}

static int GameDataDefs_ReadAttributeInteger(std::uint8_t* node, const char* key)
{
    return ReadIntegerAttribute(node, nullptr, const_cast<char*>(key));
}

static float GameDataDefs_ReadFloat(std::uint8_t* node, const char* key)
{
    return static_cast<float>(ReadFloatProperty(node, nullptr, const_cast<char*>(key)));
}

static std::uint8_t* GameDataDefs_GetChild(std::uint8_t* node, std::uint32_t index)
{
    return reinterpret_cast<std::uint8_t*>(GetComponent(node, nullptr, index));
}

static void GameDataDefs_ReadString(
    std::uint8_t* node, const char* key, void* destination, char* buffer)
{
    MenuExtras_ReadText(node, nullptr, const_cast<char*>(key), buffer, 256);
    AssignStringRange_429B60(
        destination, nullptr, buffer, static_cast<unsigned int>(std::strlen(buffer)));
}

static void GameDataDefs_ReadVectorProperty(
    std::uint8_t* node, const char* key, void* destination)
{
    DataNode_ReadVector3Property(
        node, nullptr, const_cast<char*>(key), reinterpret_cast<int>(destination));
}

static int __fastcall CLevelData_LoadVersion17(
    CLevelData* self, void* /*edx*/, S_00692b80* rootNode)
{
    // These entries in the packet identify direct callees, not vtable stores.
    (void)CSF_VTBL(0x00692B80);
    (void)CSF_VTBL(0x0041E5F0);
    (void)CSF_VTBL(0x00776FF0);
    (void)CSF_VTBL(0x006ABB80);

    auto* root = reinterpret_cast<std::uint8_t*>(rootNode);
    auto* level = reinterpret_cast<std::uint8_t*>(self);
    char buffer[256] = {};

    CSF_AT(int, self, 0x64) = GameDataDefs_ReadInteger(root, g_pszKey_PLAYER);
    auto* groups = GameDataDefs_FindProperty(root, g_pszKey_GRUPOS_BICHOS);
    CLevelData_LoadBichoDefinitions(
        self, nullptr, reinterpret_cast<S_00692b80*>(groups));

    auto* world = LookupNamedElement(root, nullptr, g_pszKey_MUNDOVIS);
    GameDataDefs_ReadString(world, g_pszKey_RWS, level + 0x1F54, buffer);
    if (GameDataDefs_FindProperty(world, g_pszKey_SIMPLIFICADO))
        GameDataDefs_ReadString(world, g_pszKey_SIMPLIFICADO, level + 0x8D4, buffer);
    GameDataDefs_ReadString(world, g_pszKey_TEXTURAS, level + 0x1F70, buffer);
    CSF_AT(int, self, 0x68) =
        GameDataDefs_ReadInteger(world, g_pszKey_SONIDO_AMBIENTE);
    GameDataDefs_ReadString(world, g_pszKey_CIELO, level + 0x488, buffer);

    if (GameDataDefs_FindProperty(world, g_pszKey_CIELOSPEED1))
    {
        CSF_AT(float, self, 0x4A8) = GameDataDefs_ReadFloat(world, g_pszKey_CIELOSPEED1);
        CSF_AT(float, self, 0x4AC) = GameDataDefs_ReadFloat(world, g_pszKey_CIELOSPEED2);
        CSF_AT(float, self, 0x4B0) = GameDataDefs_ReadFloat(world, g_pszKey_CIELOSPEED3);
    }
    if (GameDataDefs_FindProperty(world, g_pszKey_CIELO_MODULATE2X))
        CSF_AT(std::uint8_t, self, 0x4B4) =
            GameDataDefs_ReadInteger(world, g_pszKey_CIELO_MODULATE2X) == 1;

    CSF_AT(std::uint8_t, self, 0x8F4) =
        GameDataDefs_ReadInteger(world, g_pszKey_MODULATE2X) == 1;
    CSF_AT(int, self, 0x8F8) = GameDataDefs_ReadInteger(world, g_pszKey_AMBESCENA);
    CSF_AT(int, self, 0x8FC) = GameDataDefs_ReadInteger(world, g_pszKey_AMBIENTE);
    CSF_AT(float, self, 0x900) = GameDataDefs_ReadFloat(world, g_pszKey_VIEWPORT_AMB_ADD);
    CSF_AT(float, self, 0x904) = GameDataDefs_ReadFloat(world, g_pszKey_CAMERANEAR);
    CSF_AT(float, self, 0x908) = GameDataDefs_ReadFloat(world, g_pszKey_CAMERAFAR);
    if (GameDataDefs_FindProperty(world, g_pszKey_CAMERAFOV))
        CSF_AT(float, self, 0x90C) = GameDataDefs_ReadFloat(world, g_pszKey_CAMERAFOV);
    else
        CSF_AT(float, self, 0x90C) = -1.0f;

    CSF_AT(std::uint8_t, self, 0x910) =
        GameDataDefs_ReadInteger(world, g_pszKey_FOGENABLED) == 1;
    CSF_AT(int, self, 0x914) = GameDataDefs_ReadInteger(world, g_pszKey_FOGTYPE);
    CSF_AT(int, self, 0x928) = GameDataDefs_ReadInteger(world, g_pszKey_FOGCOLOR);
    CSF_AT(float, self, 0x930) = GameDataDefs_ReadFloat(world, g_pszKey_FOGDENSITY);
    CSF_AT(float, self, 0x934) = GameDataDefs_ReadFloat(world, g_pszKey_FOGDISTANCE);
    if (GameDataDefs_FindProperty(world, g_pszKey_AGUA_FOGTYPE))
    {
        CSF_AT(int, self, 0x938) = GameDataDefs_ReadInteger(world, g_pszKey_AGUA_FOGTYPE);
        CSF_AT(int, self, 0x94C) = GameDataDefs_ReadInteger(world, g_pszKey_AGUA_FOGCOLOR);
        CSF_AT(float, self, 0x954) = GameDataDefs_ReadFloat(world, g_pszKey_AGUA_FOGDENSITY);
        CSF_AT(float, self, 0x958) = GameDataDefs_ReadFloat(world, g_pszKey_AGUA_FOGDISTANCE);
    }
    CSF_AT(int, self, 0x95C) = GameDataDefs_ReadInteger(world, g_pszKey_ATMOSFERICFX);
    CSF_AT(std::uint8_t, self, 0x970) =
        GameDataDefs_ReadInteger(world, g_pszKey_VAHOENABLED) == 1;
    CSF_AT(std::uint8_t, self, 0x972) =
        GameDataDefs_ReadInteger(world, g_pszKey_MINIMAPA_RTS_game_data_defs) == 1;
    CSF_AT(float, self, 0x974) = GameDataDefs_ReadFloat(world, g_pszKey_RADIO_RTS);
    GameDataDefs_ReadString(world, g_pszKey_TEXTURA_RTS, level + 0xD94, buffer);
    GameDataDefs_ReadVectorProperty(world, g_pszKey_ORIGEN_RTS, level + 0xDB4);
    GameDataDefs_ReadVectorProperty(world, g_pszKey_DESTINO_RTS, level + 0xDC0);
    GameDataDefs_ReadVectorProperty(world, g_pszKey_MTACTICO_ORIGEN, level + 0xDD0);
    GameDataDefs_ReadVectorProperty(world, g_pszKey_MTACTICO_DESTINO, level + 0xDDC);
    GameDataDefs_ReadString(world, g_pszKey_MTACTICO_TEXTURA, level + 0x1204, buffer);

    if (GameDataDefs_FindProperty(world, g_pszKey_GLOW_ON))
    {
        CSF_AT(std::uint8_t, self, 0x1F40) =
            GameDataDefs_ReadInteger(world, g_pszKey_GLOW_ON) == 1;
        CSF_AT(int, self, 0x1F44) =
            GameDataDefs_ReadInteger(world, g_pszKey_GLOW_CUTCOLOR);
        CSF_AT(int, self, 0x1F48) =
            GameDataDefs_ReadInteger(world, g_pszKey_GLOW_INTENSITY);
        CSF_AT(int, self, 0x1F4C) =
            GameDataDefs_ReadInteger(world, g_pszKey_GLOW_DISPLACEMENT);
    }
    GameDataDefs_ReadString(world, g_pszKey_FLIS_ESCENARIO, level + 0x1640, buffer);
    GameDataDefs_ReadString(world, g_pszKey_SAVEGAME_DESCR, level + 0x1A9C, buffer);
    GameDataDefs_ReadString(world, g_pszKey_SAVEGAME_SCENE, level + 0x1AB8, buffer);
    CSF_AT(std::uint8_t, self, 0x1F35) =
        GameDataDefs_ReadInteger(world, g_pszKey_INVERTIR_NORTE_SUR) == 1;
    GameDataDefs_ReadString(world, g_pszKey_VIDEO_OPENING, level + 0x1EF0, buffer);
    if (auto* aiming = GameDataDefs_FindProperty(world, g_pszKey_PUNTERIA))
    {
        CopyAttributeText(aiming, nullptr, buffer, 256);
        AssignStringRange_429B60(
            level + 0x1F10, nullptr, buffer,
            static_cast<unsigned int>(std::strlen(buffer)));
    }

    auto* entities = GameDataDefs_FindProperty(root, g_pszKey_BICHOS);
    const std::uint32_t entityCount = MenuExtras_GetChildCount(entities, nullptr);
    if (entityCount > self->field_0008)
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(&self->field_0004), nullptr,
            static_cast<int>(entityCount - self->field_0008));

    for (std::uint32_t index = 0; index < entityCount; ++index)
    {
        auto* component = GameDataDefs_GetChild(entities, index);
        const int classId = GameDataDefs_ReadAttributeInteger(component, g_pszKey_CLASSID);
        auto* classTable = reinterpret_cast<void*>(self->m_entityClassTable);
        const int classIndex =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, classTable, 0)[0x10 / 4])(
                    classTable, nullptr, classId);
        if (classIndex == -1)
            continue;

        classTable = reinterpret_cast<void*>(self->m_entityClassTable);
        auto* classDefinition =
            reinterpret_cast<std::uint8_t*>(
                reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, classTable, 0)[0x8 / 4])(
                        classTable, nullptr, classIndex));

        auto* allocation = static_cast<S_00776ff0*>(Mem_Alloc(232));
        auto* entity = allocation
            ? reinterpret_cast<std::uint8_t*>(
                  S_00776ff0_InitDefaults(allocation, nullptr))
            : nullptr;

        CSF_AT(int, entity, 0x0) = classId;
        CSF_AT(int, entity, 0x4) =
            GameDataDefs_ReadAttributeInteger(component, g_pszKey_ID_8A9A90);
        MenuExtras_ReadText(
            component, nullptr, const_cast<char*>(g_pszKey_NOMBRE_game_data_defs),
            buffer, 256);
        if (buffer[0])
            AssignStringRange_429B60(
                entity + 0x8, nullptr, buffer,
                static_cast<unsigned int>(std::strlen(buffer)));
        else
            AssignStringSubstring(entity + 0x8, nullptr, classDefinition + 32, 0, -1);

        alignas(8) std::uint8_t positionBytes[0x1C] = {};
        auto* position = reinterpret_cast<float*>(positionBytes);
        if (auto* positionNode =
                LookupNamedElement(component, nullptr, g_pszKey_POS_game_data_defs))
            DataNode_ReadVector3(positionNode, nullptr, position);

        auto* cell = GameDataDefs_FindProperty(component, g_pszKey_CELDA);
        if (CSF_AT(int, classDefinition, 0xB4) != 26 && cell)
        {
            CSF_AT(int, entity, 0xE0) =
                GameDataDefs_ReadAttributeInteger(cell, g_pszKey_GRUPO);
            const int point = GameDataDefs_ReadAttributeInteger(cell, g_pszKey_PUNTO);
            CSF_AT(int, entity, 0xE4) = point;
            if (point != -1)
            {
                auto* groupPoint = reinterpret_cast<std::uint8_t*>(
                    C_0086c288_GetGroupPoint(
                        reinterpret_cast<C_0086c288*>(self->m_navMesh), nullptr,
                        CSF_AT(int, entity, 0xE0), point));
                CSF_AT(std::uint32_t, positionBytes, 0x0) =
                    CSF_AT(std::uint32_t, groupPoint, 0x4);
                CSF_AT(std::uint32_t, positionBytes, 0x4) =
                    CSF_AT(std::uint32_t, groupPoint, 0x8);
                CSF_AT(std::uint32_t, positionBytes, 0x8) =
                    CSF_AT(std::uint32_t, groupPoint, 0xC);
            }
        }
        CSF_AT(int, entity, 0xDC) =
            GameDataDefs_ReadInteger(component, g_pszKey_FLAGS_game_data_defs);
        CSF_AT(std::uint32_t, entity, 0x2C) =
            CSF_AT(std::uint32_t, positionBytes, 0x0);
        CSF_AT(std::uint32_t, entity, 0x30) =
            CSF_AT(std::uint32_t, positionBytes, 0x4);
        CSF_AT(std::uint32_t, entity, 0x34) =
            CSF_AT(std::uint32_t, positionBytes, 0x8);
        CSF_AT(float, entity, 0x78) = static_cast<float>(
            ReadFloatAttribute(component, nullptr, const_cast<char*>(g_pszKey_ANGULO)));
        CSF_AT(std::uint8_t, entity, 0x7C) =
            GameDataDefs_ReadAttributeInteger(component, g_pszKey_COLISION) == 1;

        if (auto* faction = GameDataDefs_FindProperty(component, g_pszKey_BANDO))
        {
            CopyAttributeText(faction, nullptr, buffer, 256);
            CSF_AT(int, entity, 0x24) = CharacterDefs_ParseAlliedFactionIndex(buffer);
        }
        else
            CSF_AT(int, entity, 0x24) = -1;

        GameDataDefs_ReadString(component, g_pszKey_PORTRAIT, entity + 0xA0, buffer);
        if (auto* explosion =
                GameDataDefs_FindProperty(component, g_pszKey_SEGUNDA_EXPLOSION))
            CSF_AT(std::uint8_t, entity, 0xBC) =
                ParseIntegerComponent(reinterpret_cast<std::uint32_t>(explosion)) != 0;
        GameDataDefs_ReadString(
            component, g_pszKey_GRUPO_VISUAL, entity + 0xC0, buffer);

        if (auto* doorBox = GameDataDefs_FindProperty(component, g_pszKey_DOOR_BOX))
        {
            alignas(8) std::uint8_t firstBytes[0xC] = {};
            alignas(8) std::uint8_t secondBytes[0x10] = {};
            auto* first = reinterpret_cast<float*>(firstBytes);
            auto* second = reinterpret_cast<float*>(secondBytes);
            auto* corner = GameDataDefs_GetChild(doorBox, 0);
            DataNode_ReadVector3(corner, nullptr, first);
            corner = GameDataDefs_GetChild(doorBox, 1u);
            DataNode_ReadVector3(corner, nullptr, second);
            S_006abb80_InitializeBounds(
                reinterpret_cast<S_006abb80*>(entity + 0x38), nullptr,
                first[0], second[0], first[1], second[1], first[2], second[2]);
        }
        if (auto* angle = GameDataDefs_FindProperty(component, g_pszKey_ANGULO_X))
            CSF_AT(float, entity, 0x74) = static_cast<float>(
                ReadComponentFloat(reinterpret_cast<std::uint32_t>(angle)));

        if (auto* script = GameDataDefs_FindProperty(component, g_pszKey_SCRIPT))
        {
            const std::uint32_t scriptCount = MenuExtras_GetChildCount(script, nullptr);
            for (std::uint32_t scriptIndex = 0; scriptIndex < scriptCount; ++scriptIndex)
            {
                auto* command = GameDataDefs_GetChild(script, scriptIndex);
                std::uint32_t commandId = static_cast<std::uint32_t>(
                    ParseIntegerComponent(reinterpret_cast<std::uint32_t>(command)));
                const std::uint32_t begin = CSF_AT(std::uint32_t, entity, 0x84);
                const std::uint32_t size = begin
                    ? (CSF_AT(std::uint32_t, entity, 0x88) - begin) >> 2
                    : 0;
                if (begin &&
                    size < ((CSF_AT(std::uint32_t, entity, 0x8C) - begin) >> 2))
                {
                    const std::uint32_t end = CSF_AT(std::uint32_t, entity, 0x88);
                    IntVector_FillUninitialized(
                        reinterpret_cast<std::uint32_t*>(end), 1,
                        &commandId, entity + 0x80);
                    CSF_AT(std::uint32_t, entity, 0x88) = end + 4;
                }
                else
                    IntVector_InsertN(
                        entity + 0x80, nullptr,
                        reinterpret_cast<char*>(CSF_AT(std::uint32_t, entity, 0x88)),
                        1u, &commandId);
            }
        }

        auto* animations =
            GameDataDefs_FindProperty(component, g_pszKey_ANIMACIONES_game_data_defs);
        CLevelData_ReadEntityAnimations(
            self, nullptr, animations, reinterpret_cast<int*>(entity), entity + 0x90);

        const std::uint32_t capacity = self->field_0008;
        if (self->field_000c >= capacity)
        {
            const std::uint32_t increment = self->field_0010;
            void* newArray = Mem_Alloc(4 * (increment + capacity));
            const std::uint32_t count = self->field_000c;
            if (count)
                std::memcpy(
                    newArray, reinterpret_cast<void*>(self->field_0004), 4 * count);
            const std::uint32_t oldArray = self->field_0004;
            self->field_0008 += increment;
            if (oldArray)
            {
                Mem_Free(reinterpret_cast<void*>(oldArray));
                self->field_0004 = 0;
            }
            self->field_0004 = reinterpret_cast<std::uint32_t>(newArray);
        }
        const std::uint32_t count = self->field_000c;
        auto* entry = reinterpret_cast<std::uint8_t*>(self->field_0004) + 4 * count;
        self->field_000c = count + 1;
        CSF_AT(std::uint32_t, entry, 0) = reinterpret_cast<std::uint32_t>(entity);
    }

    if (auto* effects = GameDataDefs_FindProperty(root, g_pszKey_EFECTOS_game_data_defs))
    {
        const std::uint32_t effectCount = MenuExtras_GetChildCount(effects, nullptr);
        void* effectList = level + 0x14;
        EffectList_Reserve(effectList, nullptr, effectCount);
        for (std::uint32_t index = 0; index < effectCount; ++index)
        {
            auto* component = GameDataDefs_GetChild(effects, index);
            // Name string (28 bytes), five integers, and three flag bytes.
            alignas(8) std::uint8_t effectBytes[0x34] = {};
            CSF_AT(std::uint32_t, effectBytes, 0x18) = 15;
            CSF_AT(int, effectBytes, 0x1C) = -1;
            CSF_AT(int, effectBytes, 0x20) = -1;
            CSF_AT(int, effectBytes, 0x24) = -1;
            CSF_AT(int, effectBytes, 0x28) = -1;
            CSF_AT(int, effectBytes, 0x2C) = -1;

            GameDataDefs_ReadString(
                component, g_pszKey_NOMBRE_game_data_defs, effectBytes, buffer);
            CSF_AT(int, effectBytes, 0x1C) =
                GameDataDefs_ReadAttributeInteger(component, g_pszKey_ID_8A9A90);
            CSF_AT(int, effectBytes, 0x20) =
                GameDataDefs_ReadAttributeInteger(component, g_pszKey_CLASSID);
            CSF_AT(int, effectBytes, 0x24) =
                GameDataDefs_ReadAttributeInteger(component, g_pszKey_DUMMY);
            if (GameDataDefs_FindProperty(component, g_pszKey_PRIORITY))
                CSF_AT(int, effectBytes, 0x28) =
                    GameDataDefs_ReadAttributeInteger(component, g_pszKey_PRIORITY);
            if (GameDataDefs_FindProperty(component, g_pszKey_SHARE_GROUP))
                CSF_AT(int, effectBytes, 0x2C) =
                    GameDataDefs_ReadAttributeInteger(component, g_pszKey_SHARE_GROUP);

            void* table = reinterpret_cast<void*>(self->m_effectClassTable);
            if (reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, table, 0)[0x10 / 4])(
                        table, nullptr, CSF_AT(int, effectBytes, 0x20)) != -1)
                EffectDefVector_PushBack(
                    effectList, nullptr, reinterpret_cast<int>(effectBytes));

            if (CSF_AT(std::uint32_t, effectBytes, 0x18) >= 0x10 &&
                CSF_AT(std::uint32_t, effectBytes, 0x4))
                Mem_FreeDebug(reinterpret_cast<void*>(
                    CSF_AT(std::uint32_t, effectBytes, 0x4)));
            CSF_AT(std::uint32_t, effectBytes, 0x18) = 15;
            CSF_AT(std::uint32_t, effectBytes, 0x14) = 0;
            CSF_AT(std::uint8_t, effectBytes, 0x4) = 0;
        }
    }

    CLevelData_LoadWaters(self, nullptr, root);
    if (auto* multiplayer = GameDataDefs_FindProperty(root, g_pszKey_MULTIPLAYER))
    {
        CSF_AT(std::uint8_t, self, 0x1F2C) =
            GameDataDefs_ReadInteger(multiplayer, g_pszKey_MPDEATHMATCH) != 0;
        CSF_AT(std::uint8_t, self, 0x1F2D) =
            GameDataDefs_ReadInteger(multiplayer, g_pszKey_MPTDEATHMATCH) != 0;
        CSF_AT(std::uint8_t, self, 0x1F2E) =
            GameDataDefs_ReadInteger(multiplayer, g_pszKey_MPPOSTMEN) != 0;
        CSF_AT(int, self, 0x1F30) =
            GameDataDefs_ReadInteger(multiplayer, g_pszKey_MPPOSTMEN_IDPOSTMAN);
        CSF_AT(std::uint8_t, self, 0x1F34) =
            GameDataDefs_ReadInteger(multiplayer, g_pszKey_MPFRANCOS) != 0;
    }

    if (auto* bridges = GameDataDefs_FindProperty(root, g_pszKey_BRIDGES))
    {
        const std::uint32_t bridgeCount = MenuExtras_GetChildCount(bridges, nullptr);
        for (std::uint32_t index = 0; index < bridgeCount; ++index)
        {
            // Two 28-byte strings followed by the 16-byte control-point vector.
            alignas(8) std::uint8_t bridgeBytes[0x48] = {};
            CSF_AT(std::uint32_t, bridgeBytes, 0x18) = 15;
            CSF_AT(std::uint32_t, bridgeBytes, 0x34) = 15;
            auto* component = GameDataDefs_GetChild(bridges, index);
            GameDataDefs_ReadString(
                component, g_pszKey_VISUALRWS, bridgeBytes, buffer);
            GameDataDefs_ReadString(
                component, g_pszKey_PHYSICRWS, bridgeBytes + 0x1C, buffer);

            if (auto* points = GameDataDefs_FindProperty(component, g_pszKey_CONTROL_POINTS))
            {
                const std::uint32_t pointCount = MenuExtras_GetChildCount(points, nullptr);
                for (std::uint32_t pointIndex = 0; pointIndex < pointCount; ++pointIndex)
                {
                    // Type, two vectors, height, and a 28-byte scene string.
                    alignas(8) std::uint8_t pointBytes[0x3C] = {};
                    CSF_AT(std::uint32_t, pointBytes, 0x38) = 15;
                    auto* point = GameDataDefs_GetChild(points, pointIndex);
                    CSF_AT(int, pointBytes, 0x0) =
                        GameDataDefs_ReadInteger(point, g_pszKey_TYPE);
                    GameDataDefs_ReadVectorProperty(point, g_pszKey_P1, pointBytes + 0x4);
                    GameDataDefs_ReadVectorProperty(point, g_pszKey_P2, pointBytes + 0x10);
                    CSF_AT(float, pointBytes, 0x1C) =
                        GameDataDefs_ReadFloat(point, g_pszKey_HEIGHT_game_data_defs);
                    GameDataDefs_ReadString(
                        point, g_pszKey_SCN, pointBytes + 0x20, buffer);
                    ControlPointVector_PushBack(
                        bridgeBytes + 0x38, nullptr,
                        reinterpret_cast<std::uint32_t*>(pointBytes));
                    if (CSF_AT(std::uint32_t, pointBytes, 0x38) >= 0x10 &&
                        CSF_AT(std::uint32_t, pointBytes, 0x24))
                        Mem_FreeDebug(reinterpret_cast<void*>(
                            CSF_AT(std::uint32_t, pointBytes, 0x24)));
                    CSF_AT(std::uint32_t, pointBytes, 0x38) = 15;
                    CSF_AT(std::uint32_t, pointBytes, 0x34) = 0;
                    CSF_AT(std::uint8_t, pointBytes, 0x24) = 0;
                }
                GameDataDefs_AppendElement72(
                    reinterpret_cast<S_0077d4b0*>(level + 0x24), nullptr,
                    reinterpret_cast<int>(bridgeBytes));
            }
            C_0082d410_ReleaseBuffersAndReset(
                reinterpret_cast<C_0082d410*>(bridgeBytes), nullptr);
        }
    }

    if (CSF_AT(int, self, 0x64) == -1 && self->field_000c)
    {
        auto* firstEntity = CSF_AT(std::uint8_t*, reinterpret_cast<void*>(self->field_0004), 0);
        CSF_AT(int, self, 0x64) = CSF_AT(int, firstEntity, 0x4);
    }
    CSF_AT(std::uint8_t, self, 0x1F50) =
        GameDataDefs_ReadInteger(world, g_pszKey_INICIO_COMMANDO) != 0;
    CSF_AT(std::uint8_t, self, 0x1F51) =
        GameDataDefs_ReadInteger(world, g_pszKey_INICIO_SNIPER) != 0;
    CSF_AT(std::uint8_t, self, 0x1F52) =
        GameDataDefs_ReadInteger(world, g_pszKey_INICIO_SPY) != 0;
    CSF_AT(std::uint8_t, self, 0x971) =
        GameDataDefs_ReadInteger(world, g_pszKey_ELIMINAR_MUERTOS) != 0;
    return 1;
}

HOOK(0x00780B90, CLevelData_LoadVersion17, "auto");
