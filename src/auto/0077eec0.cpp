#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
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
CSF_FUNC(int, __fastcall, DataNode_ReadVector3, 0x00693020, (void* self, void* edx, float* a2));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __fastcall, DataNode_ReadVector3Property, 0x00693A60, (void* self, void* edx, char* String1, int a3));
CSF_FUNC(int, __fastcall, C_0086c288_GetGroupPoint, 0x0075CFB0, (C_0086c288* self, void* , int a2, int a3));
CSF_FUNC(char, __fastcall, CLevelData_ReadEntityAnimations, 0x00774C70, (void* self, void* edx, void* a2, int* a3, void* a4));
CSF_FUNC(unsigned int, __fastcall, EffectList_Reserve, 0x007760B0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, ControlPointVector_PushBack, 0x00776BC0, (void* self, void* edx, uint32_t* a3));
CSF_FUNC(uint32_t*, __fastcall, EffectDefVector_PushBack, 0x00776C50, (void* self, void* edx, int a3));
CSF_FUNC(uint32_t*, __fastcall, S_00776ff0_InitDefaults, 0x00776FF0, (S_00776ff0* self, void*));
CSF_FUNC(int, __fastcall, C_0082d410_ReleaseBuffersAndReset, 0x00778140, (C_0082d410* self, void*));
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
CSF_GLOBAL(const char*, g_pszKey_MODULATE2X, 0x008A9A44);
CSF_GLOBAL(const char*, g_pszKey_AMBIENTE, 0x008A9A48);
CSF_GLOBAL(const char*, g_pszKey_AMBESCENA, 0x008A9A4C);
CSF_GLOBAL(const char*, g_pszKey_CAMERANEAR, 0x008A9A54);
CSF_GLOBAL(const char*, g_pszKey_CAMERAFAR, 0x008A9A58);
CSF_GLOBAL(const char*, g_pszKey_FOGENABLED, 0x008A9A60);
CSF_GLOBAL(const char*, g_pszKey_FOGTYPE, 0x008A9A64);
CSF_GLOBAL(const char*, g_pszKey_FOGCOLOR, 0x008A9A68);
CSF_GLOBAL(const char*, g_pszKey_FOGDENSITY, 0x008A9A70);
CSF_GLOBAL(const char*, g_pszKey_FOGDISTANCE, 0x008A9A74);
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
CSF_GLOBAL(const char*, g_pszKey_MPDEATHMATCH, 0x008A9B40);
CSF_GLOBAL(const char*, g_pszKey_MPTDEATHMATCH, 0x008A9B44);
CSF_GLOBAL(const char*, g_pszKey_MPPOSTMEN, 0x008A9B48);
CSF_GLOBAL(const char*, g_pszKey_MPPOSTMEN_IDPOSTMAN, 0x008A9B4C);
CSF_GLOBAL(const char*, g_pszKey_MPFRANCOS, 0x008A9B50);

#define KEY(name) const_cast<char*>(g_pszKey_##name)

static void GameDataDefs_ReadString(
    std::uint8_t* node, char* key, void* destination, char* buffer)
{
    MenuExtras_ReadText(node, nullptr, key, buffer, 256);
    AssignStringRange_429B60(
        destination, nullptr, buffer,
        static_cast<unsigned int>(std::strlen(buffer)));
}

static int __fastcall CLevelData_LoadV14(
    CLevelData* self, void* /*edx*/, S_00692b80* rootNode)
{
    // The packet also labels these direct-call addresses as vtable references.
    (void)CSF_VTBL(0x00692B80);
    (void)CSF_VTBL(0x0041E5F0);
    (void)CSF_VTBL(0x00776FF0);

    alignas(8) char buffer[0x100] = {};
    auto* root = reinterpret_cast<std::uint8_t*>(rootNode);
    auto* level = reinterpret_cast<std::uint8_t*>(self);

    CSF_AT(int, self, 0x64) =
        ReadIntegerProperty(root, nullptr, KEY(PLAYER));
    auto* groups = FindProperty(root, nullptr, KEY(GRUPOS_BICHOS));
    CLevelData_LoadBichoDefinitions(
        self, nullptr, reinterpret_cast<S_00692b80*>(groups));

    auto* world = LookupNamedElement(root, nullptr, g_pszKey_MUNDOVIS);
    GameDataDefs_ReadString(world, KEY(RWS), level + 0x1F54, buffer);
    if (FindProperty(world, nullptr, KEY(SIMPLIFICADO)))
        GameDataDefs_ReadString(world, KEY(SIMPLIFICADO), level + 0x8D4, buffer);
    GameDataDefs_ReadString(world, KEY(TEXTURAS), level + 0x1F70, buffer);
    CSF_AT(int, self, 0x68) =
        ReadIntegerProperty(world, nullptr, KEY(SONIDO_AMBIENTE));
    GameDataDefs_ReadString(world, KEY(CIELO), level + 0x488, buffer);

    if (FindProperty(world, nullptr, KEY(CIELOSPEED1)))
    {
        CSF_AT(float, self, 0x4A8) =
            static_cast<float>(ReadFloatProperty(world, nullptr, KEY(CIELOSPEED1)));
        CSF_AT(float, self, 0x4AC) =
            static_cast<float>(ReadFloatProperty(world, nullptr, KEY(CIELOSPEED2)));
        CSF_AT(float, self, 0x4B0) =
            static_cast<float>(ReadFloatProperty(world, nullptr, KEY(CIELOSPEED3)));
    }
    CSF_AT(std::uint8_t, self, 0x8F4) =
        ReadIntegerProperty(world, nullptr, KEY(MODULATE2X)) == 1;
    CSF_AT(int, self, 0x8F8) =
        ReadIntegerProperty(world, nullptr, KEY(AMBESCENA));
    CSF_AT(int, self, 0x8FC) =
        ReadIntegerProperty(world, nullptr, KEY(AMBIENTE));
    CSF_AT(float, self, 0x904) =
        static_cast<float>(ReadFloatProperty(world, nullptr, KEY(CAMERANEAR)));
    CSF_AT(float, self, 0x908) =
        static_cast<float>(ReadFloatProperty(world, nullptr, KEY(CAMERAFAR)));
    CSF_AT(std::uint8_t, self, 0x910) =
        ReadIntegerProperty(world, nullptr, KEY(FOGENABLED)) == 1;
    CSF_AT(int, self, 0x914) =
        ReadIntegerProperty(world, nullptr, KEY(FOGTYPE));
    CSF_AT(int, self, 0x928) =
        ReadIntegerProperty(world, nullptr, KEY(FOGCOLOR));
    CSF_AT(float, self, 0x930) =
        static_cast<float>(ReadFloatProperty(world, nullptr, KEY(FOGDENSITY)));
    CSF_AT(float, self, 0x934) =
        static_cast<float>(ReadFloatProperty(world, nullptr, KEY(FOGDISTANCE)));
    CSF_AT(std::uint8_t, self, 0x972) =
        ReadIntegerProperty(world, nullptr, KEY(MINIMAPA_RTS_game_data_defs)) == 1;
    CSF_AT(float, self, 0x974) =
        static_cast<float>(ReadFloatProperty(world, nullptr, KEY(RADIO_RTS)));

    GameDataDefs_ReadString(world, KEY(TEXTURA_RTS), level + 0xD94, buffer);
    DataNode_ReadVector3Property(
        world, nullptr, KEY(ORIGEN_RTS), reinterpret_cast<int>(level + 0xDB4));
    DataNode_ReadVector3Property(
        world, nullptr, KEY(DESTINO_RTS), reinterpret_cast<int>(level + 0xDC0));
    DataNode_ReadVector3Property(
        world, nullptr, KEY(MTACTICO_ORIGEN), reinterpret_cast<int>(level + 0xDD0));
    DataNode_ReadVector3Property(
        world, nullptr, KEY(MTACTICO_DESTINO), reinterpret_cast<int>(level + 0xDDC));
    GameDataDefs_ReadString(world, KEY(MTACTICO_TEXTURA), level + 0x1204, buffer);

    if (FindProperty(world, nullptr, KEY(GLOW_ON)))
    {
        CSF_AT(std::uint8_t, self, 0x1F40) =
            ReadIntegerProperty(world, nullptr, KEY(GLOW_ON)) == 1;
        CSF_AT(int, self, 0x1F44) =
            ReadIntegerProperty(world, nullptr, KEY(GLOW_CUTCOLOR));
        CSF_AT(int, self, 0x1F48) =
            ReadIntegerProperty(world, nullptr, KEY(GLOW_INTENSITY));
        CSF_AT(int, self, 0x1F4C) =
            ReadIntegerProperty(world, nullptr, KEY(GLOW_DISPLACEMENT));
    }
    GameDataDefs_ReadString(world, KEY(FLIS_ESCENARIO), level + 0x1640, buffer);
    GameDataDefs_ReadString(world, KEY(SAVEGAME_DESCR), level + 0x1A9C, buffer);
    GameDataDefs_ReadString(world, KEY(SAVEGAME_SCENE), level + 0x1AB8, buffer);
    CSF_AT(std::uint8_t, self, 0x1F2C) =
        ReadIntegerProperty(world, nullptr, KEY(MPDEATHMATCH)) != 0;
    CSF_AT(std::uint8_t, self, 0x1F2D) =
        ReadIntegerProperty(world, nullptr, KEY(MPTDEATHMATCH)) != 0;
    CSF_AT(std::uint8_t, self, 0x1F2E) =
        ReadIntegerProperty(world, nullptr, KEY(MPPOSTMEN)) != 0;
    CSF_AT(int, self, 0x1F30) =
        ReadIntegerProperty(world, nullptr, KEY(MPPOSTMEN_IDPOSTMAN));
    CSF_AT(std::uint8_t, self, 0x1F34) =
        ReadIntegerProperty(world, nullptr, KEY(MPFRANCOS)) != 0;

    auto* entities = FindProperty(root, nullptr, KEY(BICHOS));
    const std::uint32_t entityCount = MenuExtras_GetChildCount(entities, nullptr);
    if (entityCount > self->field_0008)
    {
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(&self->field_0004), nullptr,
            static_cast<int>(entityCount - self->field_0008));
    }

    for (std::uint32_t index = 0; index < entityCount; ++index)
    {
        auto* node = reinterpret_cast<std::uint8_t*>(
            GetComponent(entities, nullptr, index));
        const int classId = ReadIntegerAttribute(node, nullptr, KEY(CLASSID));
        void* table = reinterpret_cast<void*>(self->m_entityClassTable);
        const int classIndex =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, table, 0)[0x10 / 4])(table, nullptr, classId);
        if (classIndex == -1)
            continue;

        table = reinterpret_cast<void*>(self->m_entityClassTable);
        auto* definition = reinterpret_cast<S_00692b80*>(
            reinterpret_cast<std::uint32_t(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, table, 0)[0x08 / 4])(table, nullptr, classIndex));

        auto* storage = static_cast<S_00776ff0*>(Mem_Alloc(232));
        std::uint32_t* entity =
            storage ? S_00776ff0_InitDefaults(storage, nullptr) : nullptr;
        CSF_AT(int, entity, 0x00) = classId;
        CSF_AT(int, entity, 0x04) =
            ReadIntegerAttribute(node, nullptr, KEY(ID_8A9A90));
        MenuExtras_ReadText(node, nullptr, KEY(NOMBRE_game_data_defs), buffer, 256);
        if (buffer[0])
        {
            AssignStringRange_429B60(
                reinterpret_cast<std::uint8_t*>(entity) + 0x08, nullptr,
                buffer, static_cast<unsigned int>(std::strlen(buffer)));
        }
        else
        {
            AssignStringSubstring(
                reinterpret_cast<std::uint8_t*>(entity) + 0x08, nullptr,
                reinterpret_cast<std::uint8_t*>(definition + 2) + 0x08,
                0, static_cast<std::uint32_t>(-1));
        }

        alignas(8) std::uint8_t positionBytes[0x30];
        CSF_AT(float, positionBytes, 0x00) = 0.0f;
        CSF_AT(float, positionBytes, 0x04) = 0.0f;
        CSF_AT(float, positionBytes, 0x08) = 0.0f;
        auto* positionNode =
            LookupNamedElement(node, nullptr, g_pszKey_POS_game_data_defs);
        if (positionNode)
            DataNode_ReadVector3(
                positionNode, nullptr, reinterpret_cast<float*>(positionBytes));

        auto* cell = FindProperty(node, nullptr, KEY(CELDA));
        if (definition[15].field_0000 != 26 && cell)
        {
            CSF_AT(int, entity, 0xE0) =
                ReadIntegerAttribute(cell, nullptr, KEY(GRUPO));
            const int pointId = ReadIntegerAttribute(cell, nullptr, KEY(PUNTO));
            CSF_AT(int, entity, 0xE4) = pointId;
            if (pointId != -1)
            {
                auto* point = reinterpret_cast<std::uint8_t*>(
                    C_0086c288_GetGroupPoint(
                        reinterpret_cast<C_0086c288*>(self->m_navMesh),
                        nullptr, CSF_AT(int, entity, 0xE0), pointId));
                CSF_AT(std::uint32_t, positionBytes, 0x00) =
                    CSF_AT(std::uint32_t, point, 0x04);
                CSF_AT(std::uint32_t, positionBytes, 0x04) =
                    CSF_AT(std::uint32_t, point, 0x08);
                CSF_AT(std::uint32_t, positionBytes, 0x08) =
                    CSF_AT(std::uint32_t, point, 0x0C);
            }
        }
        CSF_AT(std::uint32_t, entity, 0x2C) =
            CSF_AT(std::uint32_t, positionBytes, 0x00);
        CSF_AT(std::uint32_t, entity, 0x30) =
            CSF_AT(std::uint32_t, positionBytes, 0x04);
        CSF_AT(std::uint32_t, entity, 0x34) =
            CSF_AT(std::uint32_t, positionBytes, 0x08);
        CSF_AT(float, entity, 0x78) =
            static_cast<float>(ReadFloatAttribute(node, nullptr, KEY(ANGULO)));
        CSF_AT(std::uint8_t, entity, 0x7C) =
            ReadIntegerAttribute(node, nullptr, KEY(COLISION)) == 1;
        CSF_AT(int, entity, 0xDC) =
            ReadIntegerProperty(node, nullptr, KEY(FLAGS_game_data_defs));

        auto* angleX = FindProperty(node, nullptr, KEY(ANGULO_X));
        if (angleX)
            CSF_AT(float, entity, 0x74) =
                static_cast<float>(ReadComponentFloat(
                    reinterpret_cast<std::uint32_t>(angleX)));

        auto* script = FindProperty(node, nullptr, KEY(SCRIPT));
        if (script)
        {
            const std::uint32_t scriptCount =
                MenuExtras_GetChildCount(script, nullptr);
            for (std::uint32_t scriptIndex = 0;
                 scriptIndex < scriptCount; ++scriptIndex)
            {
                const std::uint32_t component =
                    GetComponent(script, nullptr, scriptIndex);
                alignas(8) std::uint8_t valueBytes[0x04];
                CSF_AT(std::uint32_t, valueBytes, 0) =
                    static_cast<std::uint32_t>(ParseIntegerComponent(component));

                const std::uint32_t begin = CSF_AT(std::uint32_t, entity, 0x84);
                const std::uint32_t size = begin
                    ? (CSF_AT(std::uint32_t, entity, 0x88) - begin) >> 2
                    : 0;
                if (begin &&
                    size < (CSF_AT(std::uint32_t, entity, 0x8C) - begin) >> 2)
                {
                    const std::uint32_t end =
                        CSF_AT(std::uint32_t, entity, 0x88);
                    IntVector_FillUninitialized(
                        reinterpret_cast<std::uint32_t*>(end), 1,
                        reinterpret_cast<const std::uint32_t*>(valueBytes),
                        reinterpret_cast<std::uint8_t*>(entity) + 0x80);
                    CSF_AT(std::uint32_t, entity, 0x88) = end + 4;
                }
                else
                {
                    IntVector_InsertN(
                        reinterpret_cast<std::uint8_t*>(entity) + 0x80,
                        nullptr,
                        reinterpret_cast<char*>(
                            CSF_AT(std::uint32_t, entity, 0x88)),
                        1u, reinterpret_cast<std::uint32_t*>(valueBytes));
                }
            }
        }

        auto* animations =
            FindProperty(node, nullptr, KEY(ANIMACIONES_game_data_defs));
        CLevelData_ReadEntityAnimations(
            self, nullptr, animations, reinterpret_cast<int*>(entity),
            reinterpret_cast<S_00771b80*>(entity) + 9);

        const std::uint32_t capacity = self->field_0008;
        if (self->field_000c >= capacity)
        {
            const std::uint32_t growth = self->field_0010;
            void* newData = Mem_Alloc(4 * (growth + capacity));
            const std::uint32_t count = self->field_000c;
            if (count)
                std::memcpy(
                    newData, reinterpret_cast<void*>(self->field_0004),
                    4 * count);
            const std::uint32_t oldData = self->field_0004;
            self->field_0008 += growth;
            if (oldData)
            {
                Mem_Free(reinterpret_cast<void*>(oldData));
                self->field_0004 = 0;
            }
            self->field_0004 = reinterpret_cast<std::uint32_t>(newData);
        }
        const std::uint32_t count = self->field_000c;
        auto* slot = reinterpret_cast<std::uint32_t*>(
            self->field_0004 + 4 * count);
        self->field_000c = count + 1;
        *slot = reinterpret_cast<std::uint32_t>(entity);
    }

    auto* effects = FindProperty(root, nullptr, KEY(EFECTOS_game_data_defs));
    if (effects)
    {
        const std::uint32_t effectCount =
            MenuExtras_GetChildCount(effects, nullptr);
        EffectList_Reserve(level + 0x14, nullptr, effectCount);
        for (std::uint32_t index = 0; index < effectCount; ++index)
        {
            auto* node = reinterpret_cast<std::uint8_t*>(
                GetComponent(effects, nullptr, index));
            // String plus the scalar fields following it, not just a string.
            alignas(8) std::uint8_t effectBytes[0x34];
            CSF_AT(std::uint32_t, effectBytes, 0x18) = 15;
            CSF_AT(std::uint32_t, effectBytes, 0x14) = 0;
            CSF_AT(std::uint8_t, effectBytes, 0x04) = 0;
            CSF_AT(int, effectBytes, 0x1C) = -1;
            CSF_AT(int, effectBytes, 0x20) = -1;
            CSF_AT(int, effectBytes, 0x24) = -1;
            CSF_AT(int, effectBytes, 0x28) = -1;
            CSF_AT(std::uint8_t, effectBytes, 0x32) = 0;
            CSF_AT(std::uint16_t, effectBytes, 0x30) = 0;
            CSF_AT(int, effectBytes, 0x2C) = -1;

            GameDataDefs_ReadString(
                node, KEY(NOMBRE_game_data_defs), effectBytes, buffer);
            CSF_AT(int, effectBytes, 0x1C) =
                ReadIntegerAttribute(node, nullptr, KEY(ID_8A9A90));
            CSF_AT(int, effectBytes, 0x20) =
                ReadIntegerAttribute(node, nullptr, KEY(CLASSID));
            CSF_AT(int, effectBytes, 0x24) =
                ReadIntegerAttribute(node, nullptr, KEY(DUMMY));
            if (FindProperty(node, nullptr, KEY(PRIORITY)))
                CSF_AT(int, effectBytes, 0x28) =
                    ReadIntegerAttribute(node, nullptr, KEY(PRIORITY));
            else
                CSF_AT(int, effectBytes, 0x28) = -1;

            void* table = reinterpret_cast<void*>(self->m_effectClassTable);
            if (reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, table, 0)[0x10 / 4])(
                    table, nullptr, CSF_AT(int, effectBytes, 0x20)) != -1)
            {
                EffectDefVector_PushBack(
                    level + 0x14, nullptr, reinterpret_cast<int>(effectBytes));
            }
            if (CSF_AT(std::uint32_t, effectBytes, 0x18) >= 0x10 &&
                CSF_AT(std::uint32_t, effectBytes, 0x04))
            {
                Mem_FreeDebug(reinterpret_cast<void*>(
                    CSF_AT(std::uint32_t, effectBytes, 0x04)));
            }
            CSF_AT(std::uint32_t, effectBytes, 0x18) = 15;
            CSF_AT(std::uint32_t, effectBytes, 0x14) = 0;
            CSF_AT(std::uint8_t, effectBytes, 0x04) = 0;
        }
    }

    auto* bridges = FindProperty(root, nullptr, KEY(BRIDGES));
    if (bridges)
    {
        const std::uint32_t bridgeCount =
            MenuExtras_GetChildCount(bridges, nullptr);
        for (std::uint32_t index = 0; index < bridgeCount; ++index)
        {
            // Two 0x1C-byte strings followed by the 0x10-byte point vector.
            alignas(8) std::uint8_t bridgeBytes[0x48];
            auto* visual = bridgeBytes;
            auto* physics = bridgeBytes + 0x1C;
            auto* controlPoints = bridgeBytes + 0x38;
            CSF_AT(std::uint32_t, visual, 0x18) = 15;
            CSF_AT(std::uint32_t, physics, 0x18) = 15;
            CSF_AT(std::uint32_t, visual, 0x14) = 0;
            CSF_AT(std::uint8_t, visual, 0x04) = 0;
            CSF_AT(std::uint32_t, physics, 0x14) = 0;
            CSF_AT(std::uint8_t, physics, 0x04) = 0;
            std::memset(controlPoints + 0x04, 0, 12);

            auto* node = reinterpret_cast<std::uint8_t*>(
                GetComponent(bridges, nullptr, index));
            GameDataDefs_ReadString(node, KEY(VISUALRWS), visual, buffer);
            GameDataDefs_ReadString(node, KEY(PHYSICRWS), physics, buffer);
            auto* points = FindProperty(node, nullptr, KEY(CONTROL_POINTS));
            if (points)
            {
                const std::uint32_t pointCount =
                    MenuExtras_GetChildCount(points, nullptr);
                for (std::uint32_t pointIndex = 0;
                     pointIndex < pointCount; ++pointIndex)
                {
                    // Type, two vectors, height, and a 0x1C-byte string.
                    alignas(8) std::uint8_t pointBytes[0x3C];
                    CSF_AT(std::uint32_t, pointBytes, 0x04) = 0;
                    std::memset(pointBytes + 0x08, 0, sizeof(C_0082d410));
                    CSF_AT(std::uint32_t, pointBytes, 0x14) = 0;
                    CSF_AT(std::uint32_t, pointBytes, 0x18) = 0;
                    auto* sceneName = pointBytes + 0x20;
                    CSF_AT(std::uint32_t, sceneName, 0x18) = 15;
                    CSF_AT(std::uint32_t, sceneName, 0x14) = 0;
                    CSF_AT(std::uint8_t, sceneName, 0x04) = 0;

                    auto* pointNode = reinterpret_cast<std::uint8_t*>(
                        GetComponent(points, nullptr, pointIndex));
                    CSF_AT(int, pointBytes, 0x00) =
                        ReadIntegerProperty(pointNode, nullptr, KEY(TYPE));
                    DataNode_ReadVector3Property(
                        pointNode, nullptr, KEY(P1),
                        reinterpret_cast<int>(pointBytes + 0x04));
                    DataNode_ReadVector3Property(
                        pointNode, nullptr, KEY(P2),
                        reinterpret_cast<int>(pointBytes + 0x10));
                    CSF_AT(float, pointBytes, 0x1C) =
                        static_cast<float>(ReadFloatProperty(
                            pointNode, nullptr, KEY(HEIGHT_game_data_defs)));
                    GameDataDefs_ReadString(
                        pointNode, KEY(SCN), sceneName, buffer);
                    ControlPointVector_PushBack(
                        controlPoints, nullptr,
                        reinterpret_cast<std::uint32_t*>(pointBytes));
                    if (CSF_AT(std::uint32_t, sceneName, 0x18) >= 0x10 &&
                        CSF_AT(std::uint32_t, sceneName, 0x04))
                    {
                        Mem_FreeDebug(reinterpret_cast<void*>(
                            CSF_AT(std::uint32_t, sceneName, 0x04)));
                    }
                    CSF_AT(std::uint32_t, sceneName, 0x18) = 15;
                    CSF_AT(std::uint32_t, sceneName, 0x14) = 0;
                    CSF_AT(std::uint8_t, sceneName, 0x04) = 0;
                }
                GameDataDefs_AppendElement72(
                    reinterpret_cast<S_0077d4b0*>(level + 0x24),
                    nullptr, reinterpret_cast<int>(bridgeBytes));
            }
            C_0082d410_ReleaseBuffersAndReset(
                reinterpret_cast<C_0082d410*>(bridgeBytes), nullptr);
        }
    }

    if (CSF_AT(int, self, 0x64) == -1 && self->field_000c)
    {
        CSF_AT(int, self, 0x64) = CSF_AT(
            int, reinterpret_cast<void*>(
                CSF_AT(std::uint32_t,
                    reinterpret_cast<void*>(self->field_0004), 0)),
            0x04);
    }
    return 1;
}

#undef KEY

HOOK(0x0077EEC0, CLevelData_LoadV14, "auto");
