#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, IntVector_UninitializedFill, 0x0046BFB0, (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(unsigned int, __fastcall, IntVector_InsertN, 0x0046C940, (void* self, void* edx, char* Src, unsigned int a3, uint32_t* a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
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
CSF_FUNC(int, __fastcall, NavMesh_GetGroupPoint, 0x0075CFB0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(unsigned int, __fastcall, EffectDefVector_Reserve, 0x007760B0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, EffectDefVector_PushBack, 0x00776C50, (void* self, void* edx, int a3));
CSF_FUNC(uint32_t*, __fastcall, EntityDef_Construct, 0x00776FF0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CLevelData_LoadEntityGroups, 0x00778BB0, (void* self, void* edx, void* a2));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(const char*, g_pszKey_PLAYER, 0x008A99EC);
CSF_GLOBAL(const char*, g_pszKey_SCRIPT, 0x008A99F0);
CSF_GLOBAL(const char*, g_pszKey_MUNDOVIS, 0x008A9A14);
CSF_GLOBAL(const char*, g_pszKey_RWS, 0x008A9A18);
CSF_GLOBAL(const char*, g_pszKey_SIMPLIFICADO, 0x008A9A1C);
CSF_GLOBAL(const char*, g_pszKey_TEXTURAS, 0x008A9A20);
CSF_GLOBAL(const char*, g_pszKey_CIELO, 0x008A9A30);
CSF_GLOBAL(const char*, g_pszKey_AMBIENTE, 0x008A9A48);
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
CSF_GLOBAL(const char*, g_pszKey_COLISION, 0x008A9AB0);
CSF_GLOBAL(const char*, g_pszKey_GRUPOS_BICHOS, 0x008A9ABC);
CSF_GLOBAL(const char*, g_pszKey_DUMMY, 0x008A9AC0);

// Temporary effect definition built on the stack (MSVC 7.1 std::string + ids).
struct S_EffectDefTemp
{
    uint32_t allocator;      // +0x00
    union
    {
        char  inlineBuf[16]; // +0x04
        char* heapPtr;
    };
    uint32_t length;         // +0x14
    uint32_t capacity;       // +0x18
    int id;                  // +0x1C
    int classId;             // +0x20
    int dummy;               // +0x24
    int unk28;               // +0x28
    int unk2C;               // +0x2C
    char flag30;             // +0x30
    char flag31;             // +0x31
    char flag32;             // +0x32
};

static char __fastcall CLevelData_LoadVersionedSceneData(CLevelData* self, void* /*edx*/, uint8_t* root)
{
    char text[256];
    memset(text, 0, sizeof(text));

    CSF_AT(int, self, 0x64) = ReadIntegerProperty(root, nullptr, (char*)g_pszKey_PLAYER);

    uint8_t* groupsNode = FindProperty(root, nullptr, (char*)g_pszKey_GRUPOS_BICHOS);
    CLevelData_LoadEntityGroups(self, nullptr, groupsNode);

    uint8_t* worldNode = LookupNamedElement(root, nullptr, g_pszKey_MUNDOVIS);

    MenuExtras_ReadText(worldNode, nullptr, (char*)g_pszKey_RWS, text, 256);
    AssignStringRange_429B60((uint8_t*)self + 0x1F54, nullptr, text, strlen(text));

    if (FindProperty(worldNode, nullptr, (char*)g_pszKey_SIMPLIFICADO))
    {
        MenuExtras_ReadText(worldNode, nullptr, (char*)g_pszKey_SIMPLIFICADO, text, 256);
        AssignStringRange_429B60((uint8_t*)self + 0x8D4, nullptr, text, strlen(text));
    }

    MenuExtras_ReadText(worldNode, nullptr, (char*)g_pszKey_TEXTURAS, text, 256);
    AssignStringRange_429B60((uint8_t*)self + 0x1F70, nullptr, text, strlen(text));

    CSF_AT(int, self, 0x68) = -1;

    MenuExtras_ReadText(worldNode, nullptr, (char*)g_pszKey_CIELO, text, 256);
    AssignStringRange_429B60((uint8_t*)self + 0x488, nullptr, text, strlen(text));

    CSF_AT(uint8_t, self, 0x8F4) = 0;
    CSF_AT(int, self, 0x8F8) = 0;
    CSF_AT(int, self, 0x8FC) = ReadIntegerProperty(worldNode, nullptr, (char*)g_pszKey_AMBIENTE);
    CSF_AT(float, self, 0x904) = (float)ReadFloatProperty(worldNode, nullptr, (char*)g_pszKey_CAMERANEAR);
    CSF_AT(float, self, 0x908) = (float)ReadFloatProperty(worldNode, nullptr, (char*)g_pszKey_CAMERAFAR);
    CSF_AT(uint8_t, self, 0x910) = ReadIntegerProperty(worldNode, nullptr, (char*)g_pszKey_FOGENABLED) == 1;
    CSF_AT(int, self, 0x914) = ReadIntegerProperty(worldNode, nullptr, (char*)g_pszKey_FOGTYPE);
    CSF_AT(int, self, 0x928) = ReadIntegerProperty(worldNode, nullptr, (char*)g_pszKey_FOGCOLOR);
    CSF_AT(float, self, 0x930) = (float)ReadFloatProperty(worldNode, nullptr, (char*)g_pszKey_FOGDENSITY);
    CSF_AT(float, self, 0x934) = (float)ReadFloatProperty(worldNode, nullptr, (char*)g_pszKey_FOGDISTANCE);

    AssignStringRange_429B60((uint8_t*)self + 0x1204, nullptr, (char*)&g_byte_82E65C, 0);
    AssignStringRange_429B60((uint8_t*)self + 0xD94, nullptr, (char*)&g_byte_82E65C, 0);
    AssignStringRange_429B60((uint8_t*)self + 0x1640, nullptr, (char*)&g_byte_82E65C, 0);

    // ---- BICHOS (entities) ----
    uint8_t* entitiesNode = FindProperty(root, nullptr, (char*)g_pszKey_BICHOS);
    unsigned int entityCount = MenuExtras_GetChildCount(entitiesNode, nullptr);
    if (entityCount > self->field_0008)
        GrowButtonArray(&self->field_0004, nullptr, (int)(entityCount - self->field_0008));

    for (unsigned int entityIdx = 0; entityIdx < entityCount; ++entityIdx)
    {
        uint8_t* entityNode = (uint8_t*)GetComponent(entitiesNode, nullptr, entityIdx);
        int classId = ReadIntegerAttribute(entityNode, nullptr, (char*)g_pszKey_CLASSID);

        void* classTable = reinterpret_cast<void*>(self->field_1fa8);
        int classIndex = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, classTable, 0)[4])(classTable, nullptr, classId);
        if (classIndex == -1)
            continue;

        classTable = reinterpret_cast<void*>(self->field_1fa8);
        uint8_t* classDef = (uint8_t*)((int(__fastcall*)(void*, void*, int))CSF_AT(void**, classTable, 0)[2])(classTable, nullptr, classIndex);

        void* entityMem = Mem_Alloc(232);
        uint8_t* entity = entityMem ? (uint8_t*)EntityDef_Construct(entityMem, nullptr) : nullptr;

        CSF_AT(int, entity, 0x0) = classId;
        CSF_AT(int, entity, 0x4) = ReadIntegerAttribute(entityNode, nullptr, (char*)g_pszKey_ID_8A9A90);

        MenuExtras_ReadText(entityNode, nullptr, (char*)g_pszKey_NOMBRE_game_data_defs, text, 256);
        if (text[0])
            AssignStringRange_429B60(entity + 0x8, nullptr, text, strlen(text));
        else
            AssignStringSubstring(entity + 0x8, nullptr, classDef + 0x20, 0, 0xFFFFFFFF);

        float position[3] = { 0.0f, 0.0f, 0.0f };
        uint8_t* posNode = LookupNamedElement(entityNode, nullptr, g_pszKey_POS_game_data_defs);
        if (posNode)
            DataNode_ReadVector3(posNode, nullptr, position);

        uint8_t* cellNode = FindProperty(entityNode, nullptr, (char*)g_pszKey_CELDA);
        if (CSF_AT(int, classDef, 0xB4) != 26 && cellNode)
        {
            CSF_AT(int, entity, 0xE0) = ReadIntegerAttribute(cellNode, nullptr, (char*)g_pszKey_GRUPO);
            int pointIndex = ReadIntegerAttribute(cellNode, nullptr, (char*)g_pszKey_PUNTO);
            CSF_AT(int, entity, 0xE4) = pointIndex;
            if (pointIndex != -1)
            {
                int navPoint = NavMesh_GetGroupPoint(reinterpret_cast<void*>(self->m_navMesh), nullptr,
                                                     CSF_AT(int, entity, 0xE0), pointIndex);
                position[0] = CSF_AT(float, navPoint, 0x4);
                position[1] = CSF_AT(float, navPoint, 0x8);
                position[2] = CSF_AT(float, navPoint, 0xC);
            }
        }

        CSF_AT(float, entity, 0xB0) = position[0];
        CSF_AT(float, entity, 0xB4) = position[1];
        CSF_AT(float, entity, 0xB8) = position[2];
        CSF_AT(float, entity, 0x78) = (float)ReadFloatAttribute(entityNode, nullptr, (char*)g_pszKey_ANGULO);
        CSF_AT(uint8_t, entity, 0x7C) = ReadIntegerAttribute(entityNode, nullptr, (char*)g_pszKey_COLISION) == 1;

        uint8_t* scriptNode = FindProperty(entityNode, nullptr, (char*)g_pszKey_SCRIPT);
        if (scriptNode)
        {
            unsigned int scriptCount = MenuExtras_GetChildCount(scriptNode, nullptr);
            uint8_t* scriptVec = entity + 0x80;
            for (unsigned int scriptIdx = 0; scriptIdx < scriptCount; ++scriptIdx)
            {
                uint32_t component = GetComponent(scriptNode, nullptr, scriptIdx);
                uint32_t scriptValue = (uint32_t)ParseIntegerComponent(component);

                uint32_t first = CSF_AT(uint32_t, scriptVec, 0x4);
                unsigned int size = first ? (CSF_AT(uint32_t, scriptVec, 0x8) - first) >> 2 : 0;
                if (first && size < (CSF_AT(uint32_t, scriptVec, 0xC) - first) >> 2)
                {
                    uint32_t last = CSF_AT(uint32_t, scriptVec, 0x8);
                    IntVector_UninitializedFill((int)last, 1, (int)&scriptValue, (int)scriptVec, 0);
                    CSF_AT(uint32_t, scriptVec, 0x8) = last + 4;
                }
                else
                {
                    IntVector_InsertN(scriptVec, nullptr, CSF_AT(char*, scriptVec, 0x8), 1, &scriptValue);
                }
            }
        }

        // append to the entity pointer array
        if (self->field_000c >= self->field_0008)
        {
            uint32_t growBy = self->field_0010;
            void* newItems = Mem_Alloc(4 * (growBy + self->field_0008));
            if (self->field_000c)
                memcpy(newItems, reinterpret_cast<void*>(self->field_0004), 4 * self->field_000c);
            self->field_0008 += growBy;
            if (self->field_0004)
            {
                Mem_Free(reinterpret_cast<void*>(self->field_0004));
                self->field_0004 = 0;
            }
            self->field_0004 = reinterpret_cast<uint32_t>(newItems);
        }
        uint32_t slot = self->field_000c;
        uint8_t** items = reinterpret_cast<uint8_t**>(self->field_0004);
        self->field_000c = slot + 1;
        items[slot] = entity;
    }

    // ---- EFECTOS ----
    uint8_t* effectsNode = FindProperty(root, nullptr, (char*)g_pszKey_EFECTOS_game_data_defs);
    if (effectsNode)
    {
        unsigned int effectCount = MenuExtras_GetChildCount(effectsNode, nullptr);
        EffectDefVector_Reserve((uint8_t*)self + 0x14, nullptr, effectCount);

        for (unsigned int effectIdx = 0; effectIdx < effectCount; ++effectIdx)
        {
            uint8_t* effectNode = (uint8_t*)GetComponent(effectsNode, nullptr, effectIdx);

            S_EffectDefTemp effect;
            effect.capacity = 15;
            effect.length = 0;
            effect.inlineBuf[0] = 0;
            effect.id = -1;
            effect.classId = -1;
            effect.dummy = -1;
            effect.unk28 = -1;
            effect.flag31 = 0;
            effect.flag32 = 0;
            effect.flag30 = 0;
            effect.unk2C = -1;

            MenuExtras_ReadText(effectNode, nullptr, (char*)g_pszKey_NOMBRE_game_data_defs, text, 256);
            AssignStringRange_429B60(&effect, nullptr, text, strlen(text));
            effect.id = ReadIntegerAttribute(effectNode, nullptr, (char*)g_pszKey_ID_8A9A90);
            effect.classId = ReadIntegerAttribute(effectNode, nullptr, (char*)g_pszKey_CLASSID);
            effect.dummy = ReadIntegerAttribute(effectNode, nullptr, (char*)g_pszKey_DUMMY);

            void* effectTable = reinterpret_cast<void*>(self->field_1fac);
            if (((int(__fastcall*)(void*, void*, int))CSF_AT(void**, effectTable, 0)[4])(effectTable, nullptr, effect.classId) != -1)
                EffectDefVector_PushBack((uint8_t*)self + 0x14, nullptr, (int)&effect);

            if (effect.capacity >= 0x10 && effect.heapPtr)
                Mem_FreeDebug(effect.heapPtr);
            effect.capacity = 15;
            effect.length = 0;
            effect.inlineBuf[0] = 0;
        }
    }

    if (CSF_AT(int, self, 0x64) == -1 && self->field_000c)
    {
        uint8_t* firstEntity = *reinterpret_cast<uint8_t**>(self->field_0004);
        CSF_AT(int, self, 0x64) = CSF_AT(int, firstEntity, 0x4);
    }
    return 1;
}

HOOK(0x007793D0, CLevelData_LoadVersionedSceneData, "auto");
