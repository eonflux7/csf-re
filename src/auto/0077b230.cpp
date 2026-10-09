#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// ---------------------------------------------------------------------------------------------
// Callees (bound by address)
// ---------------------------------------------------------------------------------------------
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, IntVector_FillAtEnd, 0x0046BFB0, (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(unsigned int, __fastcall, IntVector_InsertGrow, 0x0046C940, (void* self, void* edx, char* Src, unsigned int a3, uint32_t* a4));
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
CSF_FUNC(int, __fastcall, NavMesh_GetCellPoint, 0x0075CFB0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(char, __fastcall, CLevelData_ReadEntityAnimations, 0x00774C70, (void* self, void* edx, void* a2, int* a3, void* a4));
CSF_FUNC(unsigned int, __fastcall, EffectList_Reserve, 0x007760B0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, EffectList_PushBack, 0x00776C50, (void* self, void* edx, int a3));
CSF_FUNC(uint32_t*, __fastcall, EntityDef_InitDefaults, 0x00776FF0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CLevelData_ReadEntityGroups, 0x00778BB0, (void* self, void* edx, void* a2));

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C); // empty string

CSF_GLOBAL(char*, g_pszKey_PLAYER, 0x008A99EC);
CSF_GLOBAL(char*, g_pszKey_SCRIPT, 0x008A99F0);
CSF_GLOBAL(char*, g_pszKey_ANIMACIONES_game_data_defs, 0x008A99F4);
CSF_GLOBAL(char*, g_pszKey_MUNDOVIS, 0x008A9A14);
CSF_GLOBAL(char*, g_pszKey_RWS, 0x008A9A18);
CSF_GLOBAL(char*, g_pszKey_SIMPLIFICADO, 0x008A9A1C);
CSF_GLOBAL(char*, g_pszKey_TEXTURAS, 0x008A9A20);
CSF_GLOBAL(char*, g_pszKey_SONIDO_AMBIENTE, 0x008A9A2C);
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
CSF_GLOBAL(char*, g_pszKey_MINIMAPA_RTS_game_data_defs, 0x008A9ACC);

// Temporary effect entry built on the stack and copied into the level's effect list (0x34 bytes).
struct LevelEffectEntry
{
    uint32_t nameHeader;      // +0x00
    union
    {
        char  nameInline[16]; // +0x04
        char* nameHeap;       // +0x04
    };
    uint32_t nameLength;      // +0x14
    uint32_t nameCapacity;    // +0x18
    int      id;              // +0x1C
    int      classId;         // +0x20
    int      dummy;           // +0x24
    int      unk28;           // +0x28
    int      unk2C;           // +0x2C
    char     flag30;          // +0x30
    char     flag31;          // +0x31
    char     flag32;          // +0x32
};

// Reads the version-specific body of the level data file: player, visual world settings,
// the entity ("bichos") list and the effects list.
static char __fastcall CLevelData_LoadEntitiesAndWorld(CLevelData* self, void* /*edx*/, uint8_t* root)
{
    char text[256];
    std::memset(text, 0, sizeof(text));

    CSF_AT(int, self, 0x64) = ReadIntegerProperty(root, nullptr, g_pszKey_PLAYER);

    CLevelData_ReadEntityGroups(self, nullptr, FindProperty(root, nullptr, g_pszKey_GRUPOS_BICHOS));

    // ---- MUNDOVIS -------------------------------------------------------------------------
    uint8_t* world = LookupNamedElement(root, nullptr, g_pszKey_MUNDOVIS);

    MenuExtras_ReadText(world, nullptr, g_pszKey_RWS, text, 256);
    AssignStringRange_429B60((uint8_t*)self + 0x1F54, nullptr, text, (unsigned int)std::strlen(text));

    if (FindProperty(world, nullptr, g_pszKey_SIMPLIFICADO))
    {
        MenuExtras_ReadText(world, nullptr, g_pszKey_SIMPLIFICADO, text, 256);
        AssignStringRange_429B60((uint8_t*)self + 0x8D4, nullptr, text, (unsigned int)std::strlen(text));
    }

    MenuExtras_ReadText(world, nullptr, g_pszKey_TEXTURAS, text, 256);
    AssignStringRange_429B60((uint8_t*)self + 0x1F70, nullptr, text, (unsigned int)std::strlen(text));

    CSF_AT(int, self, 0x68) = ReadIntegerProperty(world, nullptr, g_pszKey_SONIDO_AMBIENTE);

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
    CSF_AT(uint8_t, self, 0x972) = ReadIntegerProperty(world, nullptr, g_pszKey_MINIMAPA_RTS_game_data_defs) == 1;

    AssignStringRange_429B60((uint8_t*)self + 0x1204, nullptr, (char*)&g_byte_82E65C, 0);
    AssignStringRange_429B60((uint8_t*)self + 0xD94, nullptr, (char*)&g_byte_82E65C, 0);
    AssignStringRange_429B60((uint8_t*)self + 0x1640, nullptr, (char*)&g_byte_82E65C, 0);

    // ---- BICHOS (entities) ----------------------------------------------------------------
    uint8_t* entitiesNode = FindProperty(root, nullptr, g_pszKey_BICHOS);

    (void)CSF_VTBL(0x00692B80);
    uint32_t entityCount = MenuExtras_GetChildCount(entitiesNode, nullptr);

    uint32_t capacity = self->field_0008;
    if (entityCount > capacity)
    {
        (void)CSF_VTBL(0x0041E5F0);
        GrowButtonArray((uint8_t*)self + 4, nullptr, (int)(entityCount - capacity));
    }

    for (uint32_t entityIndex = 0; entityIndex < entityCount; ++entityIndex)
    {
        uint8_t* entityNode = reinterpret_cast<uint8_t*>(GetComponent(entitiesNode, nullptr, entityIndex));

        int classId = ReadIntegerAttribute(entityNode, nullptr, g_pszKey_CLASSID);

        void* classTable = reinterpret_cast<void*>(self->field_1fa8);
        int classIndex = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, classTable, 0)[4])(classTable, nullptr, classId);
        if (classIndex == -1)
            continue;

        classTable = reinterpret_cast<void*>(self->field_1fa8);
        uint8_t* classDef = ((uint8_t*(__fastcall*)(void*, void*, int))CSF_AT(void**, classTable, 0)[2])(classTable, nullptr, classIndex);

        uint32_t* entity = nullptr;
        void* entityMemory = Mem_Alloc(232);
        if (entityMemory)
        {
            (void)CSF_VTBL(0x00776FF0);
            entity = EntityDef_InitDefaults(entityMemory, nullptr);
        }

        CSF_AT(int, entity, 0x00) = classId;
        CSF_AT(int, entity, 0x04) = ReadIntegerAttribute(entityNode, nullptr, g_pszKey_ID_8A9A90);

        MenuExtras_ReadText(entityNode, nullptr, g_pszKey_NOMBRE_game_data_defs, text, 256);
        if (text[0])
            AssignStringRange_429B60((uint8_t*)entity + 0x08, nullptr, text, (unsigned int)std::strlen(text));
        else
            AssignStringSubstring((uint8_t*)entity + 0x08, nullptr, classDef + 32, 0, 0xFFFFFFFFu);

        uint32_t position[3] = { 0, 0, 0 };
        uint8_t* positionNode = LookupNamedElement(entityNode, nullptr, g_pszKey_POS_game_data_defs);
        if (positionNode)
            DataNode_ReadVector3(positionNode, nullptr, reinterpret_cast<float*>(position));

        uint8_t* cellNode = FindProperty(entityNode, nullptr, g_pszKey_CELDA);
        if (CSF_AT(int, classDef, 180) != 26 && cellNode)
        {
            CSF_AT(int, entity, 0xE0) = ReadIntegerAttribute(cellNode, nullptr, g_pszKey_GRUPO);
            int pointIndex = ReadIntegerAttribute(cellNode, nullptr, g_pszKey_PUNTO);
            CSF_AT(int, entity, 0xE4) = pointIndex;
            if (pointIndex != -1)
            {
                int cellPoint = NavMesh_GetCellPoint(reinterpret_cast<void*>(self->m_navMesh), nullptr,
                                                     CSF_AT(int, entity, 0xE0), pointIndex);
                position[0] = CSF_AT(uint32_t, cellPoint, 4);
                position[1] = CSF_AT(uint32_t, cellPoint, 8);
                position[2] = CSF_AT(uint32_t, cellPoint, 12);
            }
        }

        CSF_AT(uint32_t, entity, 0x2C) = position[0];
        CSF_AT(uint32_t, entity, 0x30) = position[1];
        CSF_AT(uint32_t, entity, 0x34) = position[2];

        CSF_AT(float, entity, 0x78) = (float)ReadFloatAttribute(entityNode, nullptr, g_pszKey_ANGULO);
        CSF_AT(uint8_t, entity, 0x7C) = ReadIntegerAttribute(entityNode, nullptr, g_pszKey_COLISION) == 1;

        uint8_t* scriptNode = FindProperty(entityNode, nullptr, g_pszKey_SCRIPT);
        if (scriptNode)
        {
            (void)CSF_VTBL(0x00692B80);
            uint32_t scriptCount = MenuExtras_GetChildCount(scriptNode, nullptr);
            uint8_t* scriptVector = (uint8_t*)entity + 0x80;

            for (uint32_t scriptIndex = 0; scriptIndex < scriptCount; ++scriptIndex)
            {
                uint32_t scriptId = (uint32_t)ParseIntegerComponent(GetComponent(scriptNode, nullptr, scriptIndex));

                uint32_t first = CSF_AT(uint32_t, scriptVector, 4);
                uint32_t size = first ? (CSF_AT(uint32_t, scriptVector, 8) - first) >> 2 : 0;

                if (first && size < (CSF_AT(uint32_t, scriptVector, 12) - first) >> 2)
                {
                    uint32_t last = CSF_AT(uint32_t, scriptVector, 8);
                    IntVector_FillAtEnd((int)last, 1, (int)&scriptId, (int)scriptVector, 0);
                    CSF_AT(uint32_t, scriptVector, 8) = last + 4;
                }
                else
                {
                    IntVector_InsertGrow(scriptVector, nullptr, CSF_AT(char*, scriptVector, 8), 1u, &scriptId);
                }
            }
        }

        uint8_t* animationsNode = FindProperty(entityNode, nullptr, g_pszKey_ANIMACIONES_game_data_defs);
        CLevelData_ReadEntityAnimations(self, nullptr, animationsNode, reinterpret_cast<int*>(entity), (uint8_t*)entity + 0x90);

        // Append to the entity pointer array (data +4, capacity +8, count +C, growBy +10).
        uint32_t currentCapacity = self->field_0008;
        if (self->field_000c >= currentCapacity)
        {
            uint32_t growBy = self->field_0010;
            void* newData = Mem_Alloc(4 * (growBy + currentCapacity));
            uint32_t count = self->field_000c;
            if (count)
                std::memcpy(newData, reinterpret_cast<const void*>(self->field_0004), 4 * count);
            self->field_0008 += growBy;
            if (self->field_0004)
            {
                Mem_Free(reinterpret_cast<void*>(self->field_0004));
                self->field_0004 = 0;
            }
            self->field_0004 = reinterpret_cast<uint32_t>(newData);
        }

        uint32_t slotIndex = self->field_000c;
        uint32_t** slot = reinterpret_cast<uint32_t**>(self->field_0004 + 4 * slotIndex);
        self->field_000c = slotIndex + 1;
        *slot = entity;
    }

    // ---- EFECTOS --------------------------------------------------------------------------
    uint8_t* effectsNode = FindProperty(root, nullptr, g_pszKey_EFECTOS_game_data_defs);
    if (effectsNode)
    {
        (void)CSF_VTBL(0x00692B80);
        uint32_t effectCount = MenuExtras_GetChildCount(effectsNode, nullptr);
        EffectList_Reserve((uint8_t*)self + 0x14, nullptr, effectCount);

        for (uint32_t effectIndex = 0; effectIndex < effectCount; ++effectIndex)
        {
            uint8_t* effectNode = reinterpret_cast<uint8_t*>(GetComponent(effectsNode, nullptr, effectIndex));

            LevelEffectEntry entry;
            entry.nameCapacity = 15;
            entry.nameLength = 0;
            entry.nameInline[0] = 0;
            entry.id = -1;
            entry.classId = -1;
            entry.dummy = -1;
            entry.unk28 = -1;
            entry.flag31 = 0;
            entry.flag32 = 0;
            entry.flag30 = 0;
            entry.unk2C = -1;

            MenuExtras_ReadText(effectNode, nullptr, g_pszKey_NOMBRE_game_data_defs, text, 256);
            AssignStringRange_429B60(&entry, nullptr, text, (unsigned int)std::strlen(text));

            entry.id = ReadIntegerAttribute(effectNode, nullptr, g_pszKey_ID_8A9A90);
            entry.classId = ReadIntegerAttribute(effectNode, nullptr, g_pszKey_CLASSID);
            entry.dummy = ReadIntegerAttribute(effectNode, nullptr, g_pszKey_DUMMY);

            void* effectTable = reinterpret_cast<void*>(self->field_1fac);
            if (((int(__fastcall*)(void*, void*, int))CSF_AT(void**, effectTable, 0)[4])(effectTable, nullptr, entry.classId) != -1)
                EffectList_PushBack((uint8_t*)self + 0x14, nullptr, (int)&entry);

            if (entry.nameCapacity >= 0x10 && entry.nameHeap)
                Mem_FreeDebug(entry.nameHeap);
            entry.nameCapacity = 15;
            entry.nameLength = 0;
            entry.nameInline[0] = 0;
        }
    }

    // No explicit player: default to the id of the first entity.
    if (CSF_AT(int, self, 0x64) == -1 && self->field_000c)
        CSF_AT(int, self, 0x64) = CSF_AT(int, CSF_AT(void*, self->field_0004, 0), 4);

    return 1;
}

HOOK(0x0077B230, CLevelData_LoadEntitiesAndWorld, "auto");
