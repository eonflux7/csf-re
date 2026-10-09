#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// ---------------------------------------------------------------------------------------------------------------
// Callees (bound by address)
// ---------------------------------------------------------------------------------------------------------------
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, IntVector_FillUninitialized, 0x0046BFB0, (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(unsigned int, __fastcall, IntVector_InsertN, 0x0046C940, (void* self, void* edx, char* Src, unsigned int a3, uint32_t* a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(int, __fastcall, DataNode_ParseVector3, 0x00693020, (void* self, void* edx, float* a2));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __fastcall, DataNode_ReadVector3Property, 0x00693A60, (void* self, void* edx, char* String1, int a3));
CSF_FUNC(int, __fastcall, NavMesh_GetCellPoint, 0x0075CFB0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(char, __fastcall, CLevelData_ReadBichoAnimations, 0x00774C70, (void* self, void* edx, void* a2, int* a3, void* a4));
CSF_FUNC(unsigned int, __fastcall, EffectDefVector_Reserve, 0x007760B0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, ControlPointVector_PushBack, 0x00776BC0, (void* self, void* edx, uint32_t* a3));
CSF_FUNC(uint32_t*, __fastcall, EffectDefVector_PushBack, 0x00776C50, (void* self, void* edx, int a3));
CSF_FUNC(uint32_t*, __fastcall, BichoDef_InitDefaults, 0x00776FF0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, BridgeDef_Destroy, 0x00778140, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CLevelData_ReadBichoGroups, 0x00778BB0, (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t*, __fastcall, BridgeDefVector_PushBack, 0x0077D560, (void* self, void* edx, int a3));

// ---------------------------------------------------------------------------------------------------------------
// Keyword globals
// ---------------------------------------------------------------------------------------------------------------
CSF_GLOBAL(char*, g_pszKey_PLAYER, 0x008A99EC);
CSF_GLOBAL(char*, g_pszKey_SCRIPT, 0x008A99F0);
CSF_GLOBAL(char*, g_pszKey_ANIMACIONES_game_data_defs, 0x008A99F4);
CSF_GLOBAL(char*, g_pszKey_MUNDOVIS, 0x008A9A14);
CSF_GLOBAL(char*, g_pszKey_RWS, 0x008A9A18);
CSF_GLOBAL(char*, g_pszKey_SIMPLIFICADO, 0x008A9A1C);
CSF_GLOBAL(char*, g_pszKey_TEXTURAS, 0x008A9A20);
CSF_GLOBAL(char*, g_pszKey_SONIDO_AMBIENTE, 0x008A9A2C);
CSF_GLOBAL(char*, g_pszKey_CIELO, 0x008A9A30);
CSF_GLOBAL(char*, g_pszKey_CIELOSPEED1, 0x008A9A34);
CSF_GLOBAL(char*, g_pszKey_CIELOSPEED2, 0x008A9A38);
CSF_GLOBAL(char*, g_pszKey_CIELOSPEED3, 0x008A9A3C);
CSF_GLOBAL(char*, g_pszKey_MODULATE2X, 0x008A9A44);
CSF_GLOBAL(char*, g_pszKey_AMBIENTE, 0x008A9A48);
CSF_GLOBAL(char*, g_pszKey_AMBESCENA, 0x008A9A4C);
CSF_GLOBAL(char*, g_pszKey_CAMERANEAR, 0x008A9A54);
CSF_GLOBAL(char*, g_pszKey_CAMERAFAR, 0x008A9A58);
CSF_GLOBAL(char*, g_pszKey_FOGENABLED, 0x008A9A60);
CSF_GLOBAL(char*, g_pszKey_FOGTYPE, 0x008A9A64);
CSF_GLOBAL(char*, g_pszKey_FOGCOLOR, 0x008A9A68);
CSF_GLOBAL(char*, g_pszKey_FOGDENSITY, 0x008A9A70);
CSF_GLOBAL(char*, g_pszKey_FOGDISTANCE, 0x008A9A74);
CSF_GLOBAL(char*, g_pszKey_BICHOS, 0x008A9A80);
CSF_GLOBAL(char*, g_pszKey_EFECTOS_game_data_defs, 0x008A9A84);
CSF_GLOBAL(char*, g_pszKey_CLASSID, 0x008A9A8C);
CSF_GLOBAL(char*, g_pszKey_ID_8A9A90, 0x008A9A90);
CSF_GLOBAL(char*, g_pszKey_NOMBRE_game_data_defs, 0x008A9A94);
CSF_GLOBAL(char*, g_pszKey_POS_game_data_defs, 0x008A9A98);
CSF_GLOBAL(char*, g_pszKey_CELDA, 0x008A9A9C);
CSF_GLOBAL(char*, g_pszKey_GRUPO, 0x008A9AA0);
CSF_GLOBAL(char*, g_pszKey_PUNTO, 0x008A9AA4);
CSF_GLOBAL(char*, g_pszKey_ANGULO, 0x008A9AA8);
CSF_GLOBAL(char*, g_pszKey_COLISION, 0x008A9AB0);
CSF_GLOBAL(char*, g_pszKey_GRUPOS_BICHOS, 0x008A9ABC);
CSF_GLOBAL(char*, g_pszKey_DUMMY, 0x008A9AC0);
CSF_GLOBAL(char*, g_pszKey_PRIORITY, 0x008A9AC4);
CSF_GLOBAL(char*, g_pszKey_MINIMAPA_RTS_game_data_defs, 0x008A9ACC);
CSF_GLOBAL(char*, g_pszKey_RADIO_RTS, 0x008A9AD0);
CSF_GLOBAL(char*, g_pszKey_TEXTURA_RTS, 0x008A9AD4);
CSF_GLOBAL(char*, g_pszKey_MTACTICO_ORIGEN, 0x008A9AE0);
CSF_GLOBAL(char*, g_pszKey_MTACTICO_DESTINO, 0x008A9AE4);
CSF_GLOBAL(char*, g_pszKey_MTACTICO_TEXTURA, 0x008A9AE8);
CSF_GLOBAL(char*, g_pszKey_FLIS_ESCENARIO, 0x008A9AEC);
CSF_GLOBAL(char*, g_pszKey_BRIDGES, 0x008A9AF0);
CSF_GLOBAL(char*, g_pszKey_VISUALRWS, 0x008A9AF4);
CSF_GLOBAL(char*, g_pszKey_PHYSICRWS, 0x008A9AF8);
CSF_GLOBAL(char*, g_pszKey_CONTROL_POINTS, 0x008A9AFC);
CSF_GLOBAL(char*, g_pszKey_TYPE, 0x008A9B00);
CSF_GLOBAL(char*, g_pszKey_P1, 0x008A9B04);
CSF_GLOBAL(char*, g_pszKey_P2, 0x008A9B08);
CSF_GLOBAL(char*, g_pszKey_HEIGHT_game_data_defs, 0x008A9B0C);
CSF_GLOBAL(char*, g_pszKey_SCN, 0x008A9B10);

// ---------------------------------------------------------------------------------------------------------------
// Stack temporaries of the original (MSVC 7.1 std::string = 0x1C bytes, std::vector = 0x10 bytes)
// ---------------------------------------------------------------------------------------------------------------
struct LevelDataTempString
{
    uint32_t allocator;
    union
    {
        char  inlineBuffer[16];
        char* heapBuffer;
    };
    uint32_t size;
    uint32_t capacity;
};

struct LevelDataTempVector
{
    uint32_t allocator;
    uint32_t first;
    uint32_t last;
    uint32_t end;
};

struct LevelEffectDef
{
    LevelDataTempString name;
    int      id;
    int      classId;
    int      dummy;
    int      priority;
    int      unknown2C;
    uint16_t flags30;
    uint8_t  flag32;
};

struct BridgeControlPoint
{
    int   type;
    float p1[3];
    float p2[3];
    float height;
    LevelDataTempString scn;
};

struct BridgeDef
{
    LevelDataTempString visualRws;
    LevelDataTempString physicRws;
    LevelDataTempVector controlPoints;
};

static void LevelData_InitTempString(LevelDataTempString* str)
{
    str->capacity = 15;
    str->size = 0;
    str->inlineBuffer[0] = 0;
}

// Reads the BICHOS / EFECTOS / BRIDGES and MUNDOVIS sections of a level definition (one of the versioned loaders).
static char __fastcall CLevelData_LoadWorldSections(CLevelData* self, void* /*edx*/, uint8_t* root)
{
    char buffer[256];
    std::memset(buffer, 0, sizeof(buffer));

    CSF_AT(int, self, 0x64) = ReadIntegerProperty(root, nullptr, g_pszKey_PLAYER);
    CLevelData_ReadBichoGroups(self, nullptr, FindProperty(root, nullptr, g_pszKey_GRUPOS_BICHOS));

    // ----------------------------------------------------------------------------------------- MUNDOVIS
    uint8_t* world = LookupNamedElement(root, nullptr, g_pszKey_MUNDOVIS);

    MenuExtras_ReadText(world, nullptr, g_pszKey_RWS, buffer, 256);
    AssignStringRange_429B60((uint8_t*)self + 0x1F54, nullptr, buffer, (unsigned int)std::strlen(buffer));

    if (FindProperty(world, nullptr, g_pszKey_SIMPLIFICADO))
    {
        MenuExtras_ReadText(world, nullptr, g_pszKey_SIMPLIFICADO, buffer, 256);
        AssignStringRange_429B60((uint8_t*)self + 0x8D4, nullptr, buffer, (unsigned int)std::strlen(buffer));
    }

    MenuExtras_ReadText(world, nullptr, g_pszKey_TEXTURAS, buffer, 256);
    AssignStringRange_429B60((uint8_t*)self + 0x1F70, nullptr, buffer, (unsigned int)std::strlen(buffer));

    CSF_AT(int, self, 0x68) = ReadIntegerProperty(world, nullptr, g_pszKey_SONIDO_AMBIENTE);

    MenuExtras_ReadText(world, nullptr, g_pszKey_CIELO, buffer, 256);
    AssignStringRange_429B60((uint8_t*)self + 0x488, nullptr, buffer, (unsigned int)std::strlen(buffer));

    if (FindProperty(world, nullptr, g_pszKey_CIELOSPEED1))
    {
        CSF_AT(float, self, 0x4A8) = (float)ReadFloatProperty(world, nullptr, g_pszKey_CIELOSPEED1);
        CSF_AT(float, self, 0x4AC) = (float)ReadFloatProperty(world, nullptr, g_pszKey_CIELOSPEED2);
        CSF_AT(float, self, 0x4B0) = (float)ReadFloatProperty(world, nullptr, g_pszKey_CIELOSPEED3);
    }

    CSF_AT(uint8_t, self, 0x8F4) = ReadIntegerProperty(world, nullptr, g_pszKey_MODULATE2X) == 1;
    CSF_AT(int, self, 0x8F8) = ReadIntegerProperty(world, nullptr, g_pszKey_AMBESCENA);
    CSF_AT(int, self, 0x8FC) = ReadIntegerProperty(world, nullptr, g_pszKey_AMBIENTE);
    CSF_AT(float, self, 0x904) = (float)ReadFloatProperty(world, nullptr, g_pszKey_CAMERANEAR);
    CSF_AT(float, self, 0x908) = (float)ReadFloatProperty(world, nullptr, g_pszKey_CAMERAFAR);
    CSF_AT(uint8_t, self, 0x910) = ReadIntegerProperty(world, nullptr, g_pszKey_FOGENABLED) == 1;
    CSF_AT(int, self, 0x914) = ReadIntegerProperty(world, nullptr, g_pszKey_FOGTYPE);
    CSF_AT(int, self, 0x928) = ReadIntegerProperty(world, nullptr, g_pszKey_FOGCOLOR);
    CSF_AT(float, self, 0x930) = (float)ReadFloatProperty(world, nullptr, g_pszKey_FOGDENSITY);
    CSF_AT(float, self, 0x934) = (float)ReadFloatProperty(world, nullptr, g_pszKey_FOGDISTANCE);
    CSF_AT(uint8_t, self, 0x972) = ReadIntegerProperty(world, nullptr, g_pszKey_MINIMAPA_RTS_game_data_defs) == 1;
    CSF_AT(float, self, 0x974) = (float)ReadFloatProperty(world, nullptr, g_pszKey_RADIO_RTS);

    DataNode_ReadVector3Property(world, nullptr, g_pszKey_MTACTICO_ORIGEN, (int)((uint8_t*)self + 0xDD0));
    DataNode_ReadVector3Property(world, nullptr, g_pszKey_MTACTICO_DESTINO, (int)((uint8_t*)self + 0xDDC));

    MenuExtras_ReadText(world, nullptr, g_pszKey_MTACTICO_TEXTURA, buffer, 256);
    AssignStringRange_429B60((uint8_t*)self + 0x1204, nullptr, buffer, (unsigned int)std::strlen(buffer));
    MenuExtras_ReadText(world, nullptr, g_pszKey_TEXTURA_RTS, buffer, 256);
    AssignStringRange_429B60((uint8_t*)self + 0xD94, nullptr, buffer, (unsigned int)std::strlen(buffer));
    MenuExtras_ReadText(world, nullptr, g_pszKey_FLIS_ESCENARIO, buffer, 256);
    AssignStringRange_429B60((uint8_t*)self + 0x1640, nullptr, buffer, (unsigned int)std::strlen(buffer));

    // ----------------------------------------------------------------------------------------- BICHOS
    uint8_t* bichosNode = FindProperty(root, nullptr, g_pszKey_BICHOS);
    (void)CSF_VTBL(0x00692B80);
    uint32_t bichoCount = MenuExtras_GetChildCount(bichosNode, nullptr);
    if (bichoCount > self->field_0008)
    {
        (void)CSF_VTBL(0x0041E5F0);
        GrowButtonArray(&self->field_0004, nullptr, (int)(bichoCount - self->field_0008));
    }

    for (uint32_t bichoIndex = 0; bichoIndex < bichoCount; ++bichoIndex)
    {
        uint8_t* bichoNode = reinterpret_cast<uint8_t*>(GetComponent(bichosNode, nullptr, bichoIndex));
        int classId = ReadIntegerAttribute(bichoNode, nullptr, g_pszKey_CLASSID);

        void* classTable = reinterpret_cast<void*>(self->field_1fa8);
        int classIndex = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, classTable, 0)[4])(classTable, nullptr, classId);
        if (classIndex == -1)
            continue;

        classTable = reinterpret_cast<void*>(self->field_1fa8);
        uint8_t* classInfo =
            ((uint8_t*(__fastcall*)(void*, void*, int))CSF_AT(void**, classTable, 0)[2])(classTable, nullptr, classIndex);

        uint32_t* bicho = nullptr;
        void* bichoMemory = Mem_Alloc(232);
        if (bichoMemory)
        {
            (void)CSF_VTBL(0x00776FF0);
            bicho = BichoDef_InitDefaults(bichoMemory, nullptr);
        }

        CSF_AT(int, bicho, 0x00) = classId;
        CSF_AT(int, bicho, 0x04) = ReadIntegerAttribute(bichoNode, nullptr, g_pszKey_ID_8A9A90);

        MenuExtras_ReadText(bichoNode, nullptr, g_pszKey_NOMBRE_game_data_defs, buffer, 256);
        if (buffer[0])
            AssignStringRange_429B60((uint8_t*)bicho + 0x08, nullptr, buffer, (unsigned int)std::strlen(buffer));
        else
            AssignStringSubstring((uint8_t*)bicho + 0x08, nullptr, classInfo + 0x20, 0, 0xFFFFFFFFu);

        float position[3] = { 0.0f, 0.0f, 0.0f };
        uint8_t* posNode = LookupNamedElement(bichoNode, nullptr, g_pszKey_POS_game_data_defs);
        if (posNode)
            DataNode_ParseVector3(posNode, nullptr, position);

        uint8_t* cellNode = FindProperty(bichoNode, nullptr, g_pszKey_CELDA);
        if (CSF_AT(int, classInfo, 0xB4) != 26 && cellNode)
        {
            CSF_AT(int, bicho, 0xE0) = ReadIntegerAttribute(cellNode, nullptr, g_pszKey_GRUPO);
            int pointIndex = ReadIntegerAttribute(cellNode, nullptr, g_pszKey_PUNTO);
            CSF_AT(int, bicho, 0xE4) = pointIndex;
            if (pointIndex != -1)
            {
                int cellPoint = NavMesh_GetCellPoint(reinterpret_cast<void*>(self->m_navMesh), nullptr,
                                                     CSF_AT(int, bicho, 0xE0), pointIndex);
                const float* cellPosition = reinterpret_cast<const float*>(cellPoint + 4);
                position[0] = cellPosition[0];
                position[1] = cellPosition[1];
                position[2] = cellPosition[2];
            }
        }
        CSF_AT(float, bicho, 0x2C) = position[0];
        CSF_AT(float, bicho, 0x30) = position[1];
        CSF_AT(float, bicho, 0x34) = position[2];

        CSF_AT(float, bicho, 0x78) = (float)ReadFloatAttribute(bichoNode, nullptr, g_pszKey_ANGULO);
        CSF_AT(uint8_t, bicho, 0x7C) = ReadIntegerAttribute(bichoNode, nullptr, g_pszKey_COLISION) == 1;

        uint8_t* scriptNode = FindProperty(bichoNode, nullptr, g_pszKey_SCRIPT);
        if (scriptNode)
        {
            (void)CSF_VTBL(0x00692B80);
            uint32_t scriptCount = MenuExtras_GetChildCount(scriptNode, nullptr);
            for (uint32_t scriptIndex = 0; scriptIndex < scriptCount; ++scriptIndex)
            {
                uint32_t scriptId = (uint32_t)ParseIntegerComponent(GetComponent(scriptNode, nullptr, scriptIndex));

                // push_back on the script-id vector embedded at +0x80
                uint32_t first = CSF_AT(uint32_t, bicho, 0x84);
                uint32_t size = first ? (CSF_AT(uint32_t, bicho, 0x88) - first) >> 2 : 0;
                if (first && size < (CSF_AT(uint32_t, bicho, 0x8C) - first) >> 2)
                {
                    uint32_t last = CSF_AT(uint32_t, bicho, 0x88);
                    IntVector_FillUninitialized((int)last, 1, (int)&scriptId, (int)((uint8_t*)bicho + 0x80), 0);
                    CSF_AT(uint32_t, bicho, 0x88) = last + 4;
                }
                else
                {
                    IntVector_InsertN((uint8_t*)bicho + 0x80, nullptr,
                                      reinterpret_cast<char*>(CSF_AT(uint32_t, bicho, 0x88)), 1u, &scriptId);
                }
            }
        }

        CLevelData_ReadBichoAnimations(self, nullptr, FindProperty(bichoNode, nullptr, g_pszKey_ANIMACIONES_game_data_defs),
                                       reinterpret_cast<int*>(bicho), (uint8_t*)bicho + 0x90);

        // append to the bicho pointer array (data, capacity, count, growth)
        if (self->field_000c >= self->field_0008)
        {
            uint32_t growth = self->field_0010;
            void* newData = Mem_Alloc(4 * (growth + self->field_0008));
            uint32_t count = self->field_000c;
            if (count)
                std::memcpy(newData, reinterpret_cast<const void*>(self->field_0004), 4 * count);
            self->field_0008 += growth;
            if (self->field_0004)
            {
                Mem_Free(reinterpret_cast<void*>(self->field_0004));
                self->field_0004 = 0;
            }
            self->field_0004 = reinterpret_cast<uint32_t>(newData);
        }
        uint32_t count = self->field_000c;
        uint32_t* slot = reinterpret_cast<uint32_t*>(self->field_0004 + 4 * count);
        self->field_000c = count + 1;
        *slot = reinterpret_cast<uint32_t>(bicho);
    }

    // ----------------------------------------------------------------------------------------- EFECTOS
    uint8_t* effectsNode = FindProperty(root, nullptr, g_pszKey_EFECTOS_game_data_defs);
    if (effectsNode)
    {
        (void)CSF_VTBL(0x00692B80);
        uint32_t effectCount = MenuExtras_GetChildCount(effectsNode, nullptr);
        EffectDefVector_Reserve((uint8_t*)self + 0x14, nullptr, effectCount);

        for (uint32_t effectIndex = 0; effectIndex < effectCount; ++effectIndex)
        {
            uint8_t* effectNode = reinterpret_cast<uint8_t*>(GetComponent(effectsNode, nullptr, effectIndex));

            LevelEffectDef effect;
            LevelData_InitTempString(&effect.name);
            effect.id = -1;
            effect.classId = -1;
            effect.dummy = -1;
            effect.priority = -1;
            effect.flag32 = 0;
            effect.flags30 = 0;
            effect.unknown2C = -1;

            MenuExtras_ReadText(effectNode, nullptr, g_pszKey_NOMBRE_game_data_defs, buffer, 256);
            AssignStringRange_429B60(&effect.name, nullptr, buffer, (unsigned int)std::strlen(buffer));
            effect.id = ReadIntegerAttribute(effectNode, nullptr, g_pszKey_ID_8A9A90);
            effect.classId = ReadIntegerAttribute(effectNode, nullptr, g_pszKey_CLASSID);
            effect.dummy = ReadIntegerAttribute(effectNode, nullptr, g_pszKey_DUMMY);
            if (FindProperty(effectNode, nullptr, g_pszKey_PRIORITY))
                effect.priority = ReadIntegerAttribute(effectNode, nullptr, g_pszKey_PRIORITY);
            else
                effect.priority = -1;

            void* effectTable = reinterpret_cast<void*>(self->field_1fac);
            if (((int(__fastcall*)(void*, void*, int))CSF_AT(void**, effectTable, 0)[4])(effectTable, nullptr, effect.classId) != -1)
                EffectDefVector_PushBack((uint8_t*)self + 0x14, nullptr, (int)&effect);

            if (effect.name.capacity >= 0x10 && effect.name.heapBuffer)
                Mem_FreeDebug(effect.name.heapBuffer);
            LevelData_InitTempString(&effect.name);
        }
    }

    // ----------------------------------------------------------------------------------------- BRIDGES
    uint8_t* bridgesNode = FindProperty(root, nullptr, g_pszKey_BRIDGES);
    if (bridgesNode)
    {
        (void)CSF_VTBL(0x00692B80);
        uint32_t bridgeCount = MenuExtras_GetChildCount(bridgesNode, nullptr);
        for (uint32_t bridgeIndex = 0; bridgeIndex < bridgeCount; ++bridgeIndex)
        {
            BridgeDef bridge;
            LevelData_InitTempString(&bridge.visualRws);
            LevelData_InitTempString(&bridge.physicRws);
            bridge.controlPoints.first = 0;
            bridge.controlPoints.last = 0;
            bridge.controlPoints.end = 0;

            uint8_t* bridgeNode = reinterpret_cast<uint8_t*>(GetComponent(bridgesNode, nullptr, bridgeIndex));
            MenuExtras_ReadText(bridgeNode, nullptr, g_pszKey_VISUALRWS, buffer, 256);
            AssignStringRange_429B60(&bridge.visualRws, nullptr, buffer, (unsigned int)std::strlen(buffer));
            MenuExtras_ReadText(bridgeNode, nullptr, g_pszKey_PHYSICRWS, buffer, 256);
            AssignStringRange_429B60(&bridge.physicRws, nullptr, buffer, (unsigned int)std::strlen(buffer));

            uint8_t* pointsNode = FindProperty(bridgeNode, nullptr, g_pszKey_CONTROL_POINTS);
            if (pointsNode)
            {
                (void)CSF_VTBL(0x00692B80);
                uint32_t pointCount = MenuExtras_GetChildCount(pointsNode, nullptr);
                for (uint32_t pointIndex = 0; pointIndex < pointCount; ++pointIndex)
                {
                    BridgeControlPoint point;
                    point.p1[0] = 0.0f;
                    point.p1[1] = 0.0f;
                    point.p1[2] = 0.0f;
                    point.p2[0] = 0.0f;
                    point.p2[1] = 0.0f;
                    point.p2[2] = 0.0f;
                    LevelData_InitTempString(&point.scn);

                    uint8_t* pointNode = reinterpret_cast<uint8_t*>(GetComponent(pointsNode, nullptr, pointIndex));
                    point.type = ReadIntegerProperty(pointNode, nullptr, g_pszKey_TYPE);
                    DataNode_ReadVector3Property(pointNode, nullptr, g_pszKey_P1, (int)point.p1);
                    DataNode_ReadVector3Property(pointNode, nullptr, g_pszKey_P2, (int)point.p2);
                    point.height = (float)ReadFloatProperty(pointNode, nullptr, g_pszKey_HEIGHT_game_data_defs);
                    MenuExtras_ReadText(pointNode, nullptr, g_pszKey_SCN, buffer, 256);
                    AssignStringRange_429B60(&point.scn, nullptr, buffer, (unsigned int)std::strlen(buffer));

                    ControlPointVector_PushBack(&bridge.controlPoints, nullptr, reinterpret_cast<uint32_t*>(&point));

                    if (point.scn.capacity >= 0x10 && point.scn.heapBuffer)
                        Mem_FreeDebug(point.scn.heapBuffer);
                    LevelData_InitTempString(&point.scn);
                }
                BridgeDefVector_PushBack((uint8_t*)self + 0x24, nullptr, (int)&bridge);
            }
            BridgeDef_Destroy(&bridge, nullptr);
        }
    }

    // No explicit player: default to the id of the first bicho.
    if (CSF_AT(int, self, 0x64) == -1 && self->field_000c)
        CSF_AT(int, self, 0x64) = CSF_AT(int, CSF_AT(void*, reinterpret_cast<void*>(self->field_0004), 0), 0x04);

    return 1;
}

HOOK(0x0077D730, CLevelData_LoadWorldSections, "auto");
