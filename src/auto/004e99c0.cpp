#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Game heap allocator
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Mission state reset performed before loading the level
CSF_FUNC(int, __fastcall, SceneLoader_ResetMissionState, 0x004E92B0, (void* self, void* edx, int a2));

// Constructors of the concrete mesh/container objects
CSF_FUNC(uint32_t*, __fastcall, GameNavMesh_Construct, 0x004DDC90, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, GameAreaMesh_Construct, 0x004EF4B0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, DummyMesh_Construct, 0x00768E10, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, GameLightMesh_Construct, 0x004F04A0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, GameSceneObjMesh_Construct, 0x004F0990, (void* self, void* edx));

// Base class load
CSF_FUNC(char, __fastcall, CLevelData_Load, 0x00781E40, (CLevelData* self, void* , const char* path, int));

// Global manager that receives the loaded level name
CSF_FUNC(int, __cdecl, GetGlobalValue8BC1EC, 0x00782600, ());
CSF_FUNC(char, __fastcall, LevelRegistry_SetLevelName, 0x00782730, (void* self, void* edx, char* a2));

static char __fastcall CGameLevelData_Load(CGameLevelData* self, void* /*edx*/, const char* path, int loadArg)
{
    SceneLoader_ResetMissionState(self, nullptr, loadArg);

    // Navigation mesh (owned)
    void* navMem = Mem_Alloc(0x74);
    uint32_t* navMesh = navMem ? GameNavMesh_Construct(navMem, nullptr) : nullptr;
    self->m_ownedNavMesh = reinterpret_cast<uint32_t>(navMesh);
    self->m_navMesh = reinterpret_cast<uint32_t>(navMesh);

    // Area mesh (owned)
    void* areaMem = Mem_Alloc(0x34);
    uint32_t* areaMesh = areaMem ? GameAreaMesh_Construct(areaMem, nullptr) : nullptr;
    self->m_ownedAreaMesh = reinterpret_cast<uint32_t>(areaMesh);
    self->m_areaMesh = reinterpret_cast<uint32_t>(areaMesh);

    // Dummy mesh
    void* dummyMem = Mem_Alloc(0x14);
    uint32_t* dummyMesh = dummyMem ? DummyMesh_Construct(dummyMem, nullptr) : nullptr;
    self->m_dummyMesh = reinterpret_cast<uint32_t>(dummyMesh);

    // Light mesh
    void* lightMem = Mem_Alloc(0x14);
    uint32_t* lightMesh = lightMem ? GameLightMesh_Construct(lightMem, nullptr) : nullptr;
    self->m_lightMesh = reinterpret_cast<uint32_t>(lightMesh);

    // Scene object mesh (owned)
    void* sceneObjMem = Mem_Alloc(0x14);
    uint32_t* sceneObjMesh = sceneObjMem ? GameSceneObjMesh_Construct(sceneObjMem, nullptr) : nullptr;
    self->m_ownedSceneObjMesh = reinterpret_cast<uint32_t>(sceneObjMesh);
    self->m_sceneObjMesh = reinterpret_cast<uint32_t>(sceneObjMesh);

    char loaded = CLevelData_Load(reinterpret_cast<CLevelData*>(self), nullptr, path, loadArg);

    // std::string c_str(): small-buffer when capacity < 16, heap pointer otherwise
    char* levelName;
    if (CSF_AT(uint32_t, self, 0x1F28) < 0x10u)
        levelName = reinterpret_cast<char*>(reinterpret_cast<uint8_t*>(self) + 0x1F14);
    else
        levelName = CSF_AT(char*, self, 0x1F14);

    void* registry = reinterpret_cast<void*>(GetGlobalValue8BC1EC());
    LevelRegistry_SetLevelName(registry, nullptr, levelName);

    return loaded;
}

HOOK(0x004E99C0, CGameLevelData_Load, "auto");
