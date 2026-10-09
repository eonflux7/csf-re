#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// ---- callees (bound by address) ----
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(unsigned int, __fastcall, IntArray_PushBack, 0x004FD290, (void* self, void* edx, uint32_t* a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(int, __fastcall, DataNode_ReadVector3, 0x00693020, (void* self, void* edx, float* a2));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __fastcall, NavMesh_GetCellPoint, 0x0075CFB0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(unsigned int, __fastcall, EffectArray_Reserve, 0x007760B0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, EffectArray_PushBack, 0x00776C50, (void* self, void* edx, int a3));
CSF_FUNC(uint32_t*, __fastcall, LevelEntityDef_Construct, 0x00776FF0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CLevelData_LoadEntityGroups, 0x00778BB0, (void* self, void* edx, void* a2));

// ---- keyword globals ----
CSF_GLOBAL(char*, g_pszKey_PLAYER, 0x008A99EC);
CSF_GLOBAL(char*, g_pszKey_SCRIPT, 0x008A99F0);
CSF_GLOBAL(char*, g_pszKey_MUNDOVIS, 0x008A9A14);
CSF_GLOBAL(char*, g_pszKey_RWS, 0x008A9A18);
CSF_GLOBAL(char*, g_pszKey_SIMPLIFICADO, 0x008A9A1C);
CSF_GLOBAL(char*, g_pszKey_TEXTURAS, 0x008A9A20);
CSF_GLOBAL(char*, g_pszKey_CIELO, 0x008A9A30);
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
CSF_GLOBAL(char*, g_pszKey_ATMOSFERICFX, 0x008A9A78);
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

namespace {
// Temporary effect definition pushed into the effect array (0x34 bytes).
struct EffectDef
{
    uint32_t nameAllocator;     // +0x00
    union {                     // +0x04
        char  nameInline[16];
        char* nameHeap;
    };
    uint32_t nameLength;        // +0x14
    uint32_t nameCapacity;      // +0x18
    int      id;                // +0x1C
    int      classId;           // +0x20
    int      dummy;             // +0x24
    int      unknown28;         // +0x28
    int      unknown2C;         // +0x2C
    char     flag30;            // +0x30
    char     flag31;            // +0x31
    char     flag32;            // +0x32
};
}

// Version-specific loader: PLAYER, GRUPOS_BICHOS, MUNDOVIS, BICHOS and EFECTOS sections.
static char __fastcall CLevelData_LoadVersion0(CLevelData* self, void* /*edx*/, uint8_t* root)
{
    char text[256];
    std::memset(text, 0, sizeof(text));

    uint8_t* playerNode = FindProperty(root, nullptr, g_pszKey_PLAYER);
    if (playerNode)
        CSF_AT(int, self, 0x64) = ParseIntegerComponent((std::uint32_t)playerNode);

    CLevelData_LoadEntityGroups(self, nullptr, FindProperty(root, nullptr, g_pszKey_GRUPOS_BICHOS));

    uint8_t* world = LookupNamedElement(root, nullptr, g_pszKey_MUNDOVIS);
    if (world)
    {
        MenuExtras_ReadText(world, nullptr, g_pszKey_RWS, text, 256);
        AssignStringRange_429B60((uint8_t*)self + 0x1F54, nullptr, text, (unsigned int)std::strlen(text));

        if (FindProperty(world, nullptr, g_pszKey_SIMPLIFICADO))
        {
            MenuExtras_ReadText(world, nullptr, g_pszKey_SIMPLIFICADO, text, 256);
            AssignStringRange_429B60((uint8_t*)self + 0x8D4, nullptr, text, (unsigned int)std::strlen(text));
        }

        MenuExtras_ReadText(world, nullptr, g_pszKey_TEXTURAS, text, 256);
        AssignStringRange_429B60((uint8_t*)self + 0x1F70, nullptr, text, (unsigned int)std::strlen(text));

        CSF_AT(int, self, 0x68) = -1;

        MenuExtras_ReadText(world, nullptr, g_pszKey_CIELO, text, 256);
        AssignStringRange_429B60((uint8_t*)self + 0x488, nullptr, text, (unsigned int)std::strlen(text));

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
        CSF_AT(int, self, 0x95C) = ReadIntegerProperty(world, nullptr, g_pszKey_ATMOSFERICFX);
    }

    uint8_t* entities = FindProperty(root, nullptr, g_pszKey_BICHOS);
    if (entities)
    {
        unsigned int entityCount = MenuExtras_GetChildCount(entities, nullptr);
        if (entityCount > self->field_0008)
            GrowButtonArray((uint8_t*)self + 4, nullptr, (int)(entityCount - self->field_0008));

        for (unsigned int index = 0; index < entityCount; ++index)
        {
            uint8_t* node = (uint8_t*)GetComponent(entities, nullptr, index);
            int classId = ReadIntegerAttribute(node, nullptr, g_pszKey_CLASSID);

            void* classTable = (void*)self->field_1fa8;
            int classIndex = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, classTable, 0)[4])(classTable, nullptr, classId);
            if (classIndex == -1)
                continue;

            classTable = (void*)self->field_1fa8;
            uint8_t* classInfo = (uint8_t*)((int(__fastcall*)(void*, void*, int))CSF_AT(void**, classTable, 0)[2])(classTable, nullptr, classIndex);

            void* block = Mem_Alloc(232);
            uint8_t* entity = block ? (uint8_t*)LevelEntityDef_Construct(block, nullptr) : nullptr;

            CSF_AT(int, entity, 0x0) = classId;
            CSF_AT(int, entity, 0x4) = ReadIntegerAttribute(node, nullptr, g_pszKey_ID_8A9A90);

            MenuExtras_ReadText(node, nullptr, g_pszKey_NOMBRE_game_data_defs, text, 256);
            if (text[0])
                AssignStringRange_429B60(entity + 0x8, nullptr, text, (unsigned int)std::strlen(text));
            else
                AssignStringSubstring(entity + 0x8, nullptr, classInfo + 32, 0, 0xFFFFFFFF);

            float position[3] = { 0.0f, 0.0f, 0.0f };
            uint8_t* posNode = LookupNamedElement(node, nullptr, g_pszKey_POS_game_data_defs);
            if (posNode)
                DataNode_ReadVector3(posNode, nullptr, position);

            uint8_t* cellNode = FindProperty(node, nullptr, g_pszKey_CELDA);
            if (CSF_AT(int, classInfo, 180) != 26 && cellNode)
            {
                CSF_AT(int, entity, 0xE0) = ReadIntegerAttribute(cellNode, nullptr, g_pszKey_GRUPO);
                int point = ReadIntegerAttribute(cellNode, nullptr, g_pszKey_PUNTO);
                CSF_AT(int, entity, 0xE4) = point;
                if (point != -1)
                {
                    int cellPoint = NavMesh_GetCellPoint((void*)self->m_navMesh, nullptr, CSF_AT(int, entity, 0xE0), point);
                    position[0] = CSF_AT(float, cellPoint, 0x4);
                    position[1] = CSF_AT(float, cellPoint, 0x8);
                    position[2] = CSF_AT(float, cellPoint, 0xC);
                }
            }

            CSF_AT(float, entity, 0x2C) = position[0];
            CSF_AT(float, entity, 0x30) = position[1];
            CSF_AT(float, entity, 0x34) = position[2];
            CSF_AT(float, entity, 0x78) = (float)ReadFloatAttribute(node, nullptr, g_pszKey_ANGULO);

            uint8_t* collisionNode = FindProperty(node, nullptr, g_pszKey_COLISION);
            if (collisionNode)
                CSF_AT(uint8_t, entity, 0x7C) = ParseIntegerComponent((std::uint32_t)collisionNode) == 1;

            uint8_t* scriptNode = FindProperty(node, nullptr, g_pszKey_SCRIPT);
            if (scriptNode)
            {
                unsigned int scriptCount = MenuExtras_GetChildCount(scriptNode, nullptr);
                for (unsigned int scriptIndex = 0; scriptIndex < scriptCount; ++scriptIndex)
                {
                    uint32_t scriptId = (uint32_t)ParseIntegerComponent(GetComponent(scriptNode, nullptr, scriptIndex));
                    IntArray_PushBack(entity + 0x20, nullptr, &scriptId);
                }
            }

            if (self->field_000c >= self->field_0008)
                GrowButtonArray((uint8_t*)self + 4, nullptr, (int)self->field_0010);
            uint32_t used = self->field_000c;
            uint8_t** slot = reinterpret_cast<uint8_t**>(self->field_0004) + used;
            self->field_000c = used + 1;
            *slot = entity;
        }
    }

    uint8_t* effects = FindProperty(root, nullptr, g_pszKey_EFECTOS_game_data_defs);
    if (effects)
    {
        unsigned int effectCount = MenuExtras_GetChildCount(effects, nullptr);
        EffectArray_Reserve((uint8_t*)self + 0x14, nullptr, effectCount);

        for (unsigned int index = 0; index < effectCount; ++index)
        {
            uint8_t* node = (uint8_t*)GetComponent(effects, nullptr, index);

            EffectDef effect;
            effect.nameCapacity = 15;
            effect.nameLength = 0;
            effect.nameInline[0] = 0;
            effect.id = -1;
            effect.classId = -1;
            effect.dummy = -1;
            effect.unknown28 = -1;
            effect.flag31 = 0;
            effect.flag32 = 0;
            effect.flag30 = 0;
            effect.unknown2C = -1;

            effect.id = ReadIntegerAttribute(node, nullptr, g_pszKey_ID_8A9A90);
            MenuExtras_ReadText(node, nullptr, g_pszKey_NOMBRE_game_data_defs, text, 256);
            AssignStringRange_429B60(&effect, nullptr, text, (unsigned int)std::strlen(text));
            effect.classId = ReadIntegerAttribute(node, nullptr, g_pszKey_CLASSID);
            effect.dummy = ReadIntegerAttribute(node, nullptr, g_pszKey_DUMMY);

            void* effectTable = (void*)self->field_1fac;
            if (((int(__fastcall*)(void*, void*, int))CSF_AT(void**, effectTable, 0)[4])(effectTable, nullptr, effect.classId) != -1)
                EffectArray_PushBack((uint8_t*)self + 0x14, nullptr, (int)&effect);

            if (effect.nameCapacity >= 0x10 && effect.nameHeap)
                Mem_FreeDebug(effect.nameHeap);
            effect.nameCapacity = 15;
            effect.nameLength = 0;
            effect.nameInline[0] = 0;
        }
    }

    if (CSF_AT(int, self, 0x64) == -1 && self->field_000c)
        CSF_AT(int, self, 0x64) = CSF_AT(int, *reinterpret_cast<uint8_t**>(self->field_0004), 0x4);

    return 1;
}

HOOK(0x00778D50, CLevelData_LoadVersion0, "auto");
