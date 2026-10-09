#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class constructor (CLevelData).
CSF_FUNC(CLevelData*, __fastcall, CLevelData_Construct, 0x00781C30, (CLevelData* self, int edxArg));
// Global game state singleton accessor.
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
// Game heap allocator.
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
// Allocates the sentinel/head node of the list embedded in the destination data block.
CSF_FUNC(uint32_t*, __cdecl, sub_4E98F0, 0x004E98F0, ());

// CGameLevelData constructor: builds the CLevelData base, installs the vtables, clears the owned meshes
// and the level path string, allocates the 20-byte destination data block and wires the class tables
// of the global state into both the derived and the base slots.
static CGameLevelData* __fastcall CGameLevelData_Construct(CGameLevelData* self, void* /*edx*/)
{
    CLevelData_Construct(reinterpret_cast<CLevelData*>(self), 0);

    // Secondary base at +0x1FBC: base vtable first, then the derived one.
    CSF_AT(void**, self, 0x1FBC) = CSF_VTBL(0x0082D428);
    self->vtbl = CSF_VTBL(0x0083ED40);
    CSF_AT(void**, self, 0x1FBC) = CSF_VTBL(0x0083ED24);

    // Empty level path string (small-buffer capacity 15, length 0, terminator).
    self->field_1ffc = 15;
    self->m_levelPathLength = 0;
    CSF_AT(char, self, 0x1FE8) = 0;

    self->m_ownedNavMesh = 0;
    self->m_ownedAreaMesh = 0;
    self->m_dummyMesh = 0;
    self->m_lightMesh = 0;
    self->m_sceneObjMesh = 0;
    self->m_ownedSceneObjMesh = 0;

    // Member initialisers of the secondary base; results unused in the original.
    GetGlobalState_50D190();
    GetGlobalState_50D190();
    GetGlobalState_50D190();
    GetGlobalState_50D190();
    GetGlobalState_50D190();
    GetGlobalState_50D190();

    void* dstData = Mem_Alloc(20);
    if (dstData != nullptr)
    {
        CSF_AT(uint8_t, dstData, 0x0) = 0;
        CSF_AT(uint32_t*, dstData, 0x8) = sub_4E98F0();
        CSF_AT(uint32_t, dstData, 0xC) = 0;
    }
    self->m_dstData = reinterpret_cast<uint32_t>(dstData);

    uint32_t table = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(GetGlobalState_50D190()), 0xD8);
    self->field_1fc4 = table;
    self->field_1fa0 = table;

    table = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(GetGlobalState_50D190()), 0xDC);
    self->field_1fc8 = table;
    self->m_entityClassTable = table;

    table = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(GetGlobalState_50D190()), 0xD4);
    self->field_1fc0 = table;
    self->field_1fa4 = table;

    table = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(GetGlobalState_50D190()), 0xE0);
    self->field_1fcc = table;
    self->m_effectClassTable = table;

    table = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(GetGlobalState_50D190()), 0xE4);
    self->field_1fd0 = table;
    self->field_1fb0 = table;

    table = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(GetGlobalState_50D190()), 0xE8);
    self->field_1fd4 = table;
    self->field_1fb4 = table;

    return self;
}

HOOK(0x004E9EB0, CGameLevelData_Construct, "auto");
