#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// ---------------------------------------------------------------------------
// Callees (bound by address)
// ---------------------------------------------------------------------------
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, IntVector_UninitializedFillN, 0x0046BFB0, (int a1, int a2, int a3, int a4, int a5));
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
CSF_FUNC(int, __fastcall, DataNode_ReadVector3Property, 0x00693A60, (void* self, void* edx, char* String1, int a3));
CSF_FUNC(int, __fastcall, NavMesh_GetGroupPoint, 0x0075CFB0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(char, __fastcall, CLevelData_ReadCreatureAnimations, 0x00774C70, (void* self, void* edx, void* a2, int* a3, void* a4));
CSF_FUNC(unsigned int, __fastcall, EffectDefVector_Reserve, 0x007760B0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, EffectDefVector_PushBack, 0x00776C50, (void* self, void* edx, int a3));
CSF_FUNC(uint32_t*, __fastcall, CreatureDef_Construct, 0x00776FF0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CLevelData_ReadCreatureGroups, 0x00778BB0, (void* self, void* edx, void* a2));

// ---------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
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
CSF_GLOBAL(char*, g_pszKey_PRIORITY, 0x008A9AC4);
CSF_GLOBAL(char*, g_pszKey_MINIMAPA_RTS_game_data_defs, 0x008A9ACC);
CSF_GLOBAL(char*, g_pszKey_RADIO_RTS, 0x008A9AD0);
CSF_GLOBAL(char*, g_pszKey_MTACTICO_ORIGEN, 0x008A9AE0);
CSF_GLOBAL(char*, g_pszKey_MTACTICO_DESTINO, 0x008A9AE4);
CSF_GLOBAL(char*, g_pszKey_MTACTICO_TEXTURA, 0x008A9AE8);

namespace {
// Stack-local effect record handed to the effect vector (MSVC 7.1 std::string + ids).
struct EffectDefLocal
{
    uint32_t nameAllocator;   // +0x00
    union                     // +0x04
    {
        char  nameBuf[16];
        char* namePtr;
    };
    uint32_t nameLength;      // +0x14
    uint32_t nameCapacity;    // +0x18
    int      id;              // +0x1C
    int      classId;         // +0x20
    int      dummy;           // +0x24
    int      priority;        // +0x28
    int      field_2c;        // +0x2C
    char     flag_30;         // +0x30
    char     flag_31;         // +0x31
    char     flag_32;         // +0x32
};
}

// Reads PLAYER, GRUPOS_BICHOS, MUNDOVIS, BICHOS and EFECTOS from a level data file (one of the versioned loaders).
static char __fastcall CLevelData_LoadWorldCreaturesAndEffects(CLevelData* self, void* /*edx*/, std::uint8_t* root)
{
    char text[256];
    std::memset(text, 0, sizeof(text));

    CSF_AT(int, self, 0x64) = ReadIntegerProperty(root, nullptr, g_pszKey_PLAYER);
    CLevelData_ReadCreatureGroups(self, nullptr, FindProperty(root, nullptr, g_pszKey_GRUPOS_BICHOS));

    // --- MUNDOVIS -----------------------------------------------------------
    std::uint8_t* world = LookupNamedElement(root, nullptr, g_pszKey_MUNDOVIS);

    MenuExtras_ReadText(world, nullptr, g_pszKey_RWS, text, 256);
    AssignStringRange_429B60((uint8_t*)self + 0x1F54, nullptr, text, std::strlen(text));

    if (FindProperty(world, nullptr, g_pszKey_SIMPLIFICADO))
    {
        MenuExtras_ReadText(world, nullptr, g_pszKey_SIMPLIFICADO, text, 256);
        AssignStringRange_429B60((uint8_t*)self + 0x8D4, nullptr, text, std::strlen(text));
    }

    MenuExtras_ReadText(world, nullptr, g_pszKey_TEXTURAS, text, 256);
    AssignStringRange_429B60((uint8_t*)self + 0x1F70, nullptr, text, std::strlen(text));

    CSF_AT(int, self, 0x68) = ReadIntegerProperty(world, nullptr, g_pszKey_SONIDO_AMBIENTE);

    MenuExtras_ReadText(world, nullptr, g_pszKey_CIELO, text, 256);
    AssignStringRange_429B60((uint8_t*)self + 0x488, nullptr, text, std::strlen(text));

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

    MenuExtras_ReadText(world, nullptr, g_pszKey_MTACTICO_TEXTURA, text, 256);
    AssignStringRange_429B60((uint8_t*)self + 0x1204, nullptr, text, std::strlen(text));
    AssignStringRange_429B60((uint8_t*)self + 0xD94, nullptr, (char*)&g_byte_82E65C, 0);
    AssignStringRange_429B60((uint8_t*)self + 0x1640, nullptr, (char*)&g_byte_82E65C, 0);

    // --- BICHOS -------------------------------------------------------------
    std::uint8_t* creaturesNode = FindProperty(root, nullptr, g_pszKey_BICHOS);
    unsigned int creatureCount = MenuExtras_GetChildCount(creaturesNode, nullptr);
    if (creatureCount > self->field_0008)
        GrowButtonArray(&self->field_0004, nullptr, (int)(creatureCount - self->field_0008));

    for (unsigned int creatureIndex = 0; creatureIndex < creatureCount; ++creatureIndex)
    {
        std::uint8_t* creatureNode = (std::uint8_t*)GetComponent(creaturesNode, nullptr, creatureIndex);
        int classId = ReadIntegerAttribute(creatureNode, nullptr, g_pszKey_CLASSID);

        void* classTable = reinterpret_cast<void*>(self->m_entityClassTable);
        int classIndex = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, classTable, 0)[4])(classTable, nullptr, classId);
        if (classIndex == -1)
            continue;

        classTable = reinterpret_cast<void*>(self->m_entityClassTable);
        uint8_t* classDef = ((uint8_t*(__fastcall*)(void*, void*, int))CSF_AT(void**, classTable, 0)[2])(classTable, nullptr, classIndex);

        void* creatureMem = Mem_Alloc(232);
        uint8_t* creature = creatureMem ? (uint8_t*)CreatureDef_Construct(creatureMem, nullptr) : nullptr;

        CSF_AT(int, creature, 0x0) = classId;
        CSF_AT(int, creature, 0x4) = ReadIntegerAttribute(creatureNode, nullptr, g_pszKey_ID_8A9A90);

        MenuExtras_ReadText(creatureNode, nullptr, g_pszKey_NOMBRE_game_data_defs, text, 256);
        if (text[0])
            AssignStringRange_429B60(creature + 0x8, nullptr, text, std::strlen(text));
        else
            AssignStringSubstring(creature + 0x8, nullptr, classDef + 0x20, 0, 0xFFFFFFFFu);

        float position[3] = { 0.0f, 0.0f, 0.0f };
        std::uint8_t* posNode = LookupNamedElement(creatureNode, nullptr, g_pszKey_POS_game_data_defs);
        if (posNode)
            DataNode_ReadVector3(posNode, nullptr, position);

        std::uint8_t* cellNode = FindProperty(creatureNode, nullptr, g_pszKey_CELDA);
        if (CSF_AT(int, classDef, 0xB4) != 26 && cellNode)
        {
            CSF_AT(int, creature, 0xE0) = ReadIntegerAttribute(cellNode, nullptr, g_pszKey_GRUPO);
            int pointIndex = ReadIntegerAttribute(cellNode, nullptr, g_pszKey_PUNTO);
            CSF_AT(int, creature, 0xE4) = pointIndex;
            if (pointIndex != -1)
            {
                int navPoint = NavMesh_GetGroupPoint(reinterpret_cast<void*>(self->m_navMesh), nullptr,
                                                     CSF_AT(int, creature, 0xE0), pointIndex);
                position[0] = CSF_AT(float, navPoint, 0x4);
                position[1] = CSF_AT(float, navPoint, 0x8);
                position[2] = CSF_AT(float, navPoint, 0xC);
            }
        }
        CSF_AT(float, creature, 0x2C) = position[0];
        CSF_AT(float, creature, 0x30) = position[1];
        CSF_AT(float, creature, 0x34) = position[2];

        CSF_AT(float, creature, 0x78) = (float)ReadFloatAttribute(creatureNode, nullptr, g_pszKey_ANGULO);
        CSF_AT(uint8_t, creature, 0x7C) = ReadIntegerAttribute(creatureNode, nullptr, g_pszKey_COLISION) == 1;

        std::uint8_t* scriptNode = FindProperty(creatureNode, nullptr, g_pszKey_SCRIPT);
        if (scriptNode)
        {
            unsigned int scriptCount = MenuExtras_GetChildCount(scriptNode, nullptr);
            uint8_t* scriptVector = creature + 0x80;
            for (unsigned int scriptIndex = 0; scriptIndex < scriptCount; ++scriptIndex)
            {
                uint32_t scriptValue = (uint32_t)ParseIntegerComponent(GetComponent(scriptNode, nullptr, scriptIndex));

                uint32_t first = CSF_AT(uint32_t, scriptVector, 0x4);
                unsigned int size = first ? (CSF_AT(uint32_t, scriptVector, 0x8) - first) >> 2 : 0;
                if (first && size < (CSF_AT(uint32_t, scriptVector, 0xC) - first) >> 2)
                {
                    uint32_t last = CSF_AT(uint32_t, scriptVector, 0x8);
                    IntVector_UninitializedFillN((int)last, 1, (int)&scriptValue, (int)scriptVector, 0);
                    CSF_AT(uint32_t, scriptVector, 0x8) = last + 4;
                }
                else
                {
                    IntVector_InsertN(scriptVector, nullptr, CSF_AT(char*, scriptVector, 0x8), 1u, &scriptValue);
                }
            }
        }

        CLevelData_ReadCreatureAnimations(self, nullptr,
                                          FindProperty(creatureNode, nullptr, g_pszKey_ANIMACIONES_game_data_defs),
                                          (int*)creature, creature + 0x24);

        // push_back into the creature pointer array (data/capacity/count/growBy)
        uint32_t capacity = self->field_0008;
        if (self->field_000c >= capacity)
        {
            uint32_t growBy = self->field_0010;
            void* newData = Mem_Alloc(4 * (growBy + capacity));
            if (self->field_000c)
                std::memcpy(newData, reinterpret_cast<void*>(self->field_0004), 4 * self->field_000c);
            self->field_0008 += growBy;
            if (self->field_0004)
            {
                Mem_Free(reinterpret_cast<void*>(self->field_0004));
                self->field_0004 = 0;
            }
            self->field_0004 = reinterpret_cast<uint32_t>(newData);
        }
        uint32_t count = self->field_000c;
        uint8_t** slot = reinterpret_cast<uint8_t**>(self->field_0004 + 4 * count);
        self->field_000c = count + 1;
        *slot = creature;
    }

    // --- EFECTOS ------------------------------------------------------------
    std::uint8_t* effectsNode = FindProperty(root, nullptr, g_pszKey_EFECTOS_game_data_defs);
    if (effectsNode)
    {
        unsigned int effectCount = MenuExtras_GetChildCount(effectsNode, nullptr);
        EffectDefVector_Reserve((uint8_t*)self + 0x14, nullptr, effectCount);

        for (unsigned int effectIndex = 0; effectIndex < effectCount; ++effectIndex)
        {
            std::uint8_t* effectNode = (std::uint8_t*)GetComponent(effectsNode, nullptr, effectIndex);

            EffectDefLocal effect;
            effect.nameCapacity = 15;
            effect.nameLength = 0;
            effect.nameBuf[0] = 0;
            effect.id = -1;
            effect.classId = -1;
            effect.dummy = -1;
            effect.priority = -1;
            effect.flag_31 = 0;
            effect.flag_32 = 0;
            effect.flag_30 = 0;
            effect.field_2c = -1;

            MenuExtras_ReadText(effectNode, nullptr, g_pszKey_NOMBRE_game_data_defs, text, 256);
            AssignStringRange_429B60(&effect, nullptr, text, std::strlen(text));

            effect.id = ReadIntegerAttribute(effectNode, nullptr, g_pszKey_ID_8A9A90);
            effect.classId = ReadIntegerAttribute(effectNode, nullptr, g_pszKey_CLASSID);
            effect.dummy = ReadIntegerAttribute(effectNode, nullptr, g_pszKey_DUMMY);
            if (FindProperty(effectNode, nullptr, g_pszKey_PRIORITY))
                effect.priority = ReadIntegerAttribute(effectNode, nullptr, g_pszKey_PRIORITY);
            else
                effect.priority = -1;

            void* effectClassTable = reinterpret_cast<void*>(self->m_effectClassTable);
            if (((int(__fastcall*)(void*, void*, int))CSF_AT(void**, effectClassTable, 0)[4])(effectClassTable, nullptr, effect.classId) != -1)
                EffectDefVector_PushBack((uint8_t*)self + 0x14, nullptr, (int)&effect);

            // inlined std::string destructor
            if (effect.nameCapacity >= 0x10 && effect.namePtr)
                Mem_FreeDebug(effect.namePtr);
            effect.nameCapacity = 15;
            effect.nameLength = 0;
            effect.nameBuf[0] = 0;
        }
    }

    // Default player: first creature's id
    if (CSF_AT(int, self, 0x64) == -1 && self->field_000c)
        CSF_AT(int, self, 0x64) = CSF_AT(int, CSF_AT(uint8_t*, self->field_0004, 0), 0x4);

    return 1;
}

HOOK(0x0077C120, CLevelData_LoadWorldCreaturesAndEffects, "auto");
