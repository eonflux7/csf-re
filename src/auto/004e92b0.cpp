#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Shuts down the owned area mesh (this = CGameLevelData::m_ownedAreaMesh).
CSF_FUNC(void, __fastcall, AreaMesh_Shutdown, 0x004EF610, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CLevelData_ReleaseLoadedData, 0x0077D5F0, (CLevelData* self, void*));

// Releases the concrete nav/area/dummy/light/scene-object meshes created by the
// load slot, then lets the base class release the rest of the loaded level data.
static int __fastcall CGameLevelData_ReleaseLoadedData(CGameLevelData* self, void* /*edx*/, int /*unused*/)
{
    if (self->m_navMesh)
    {
        void* navMesh = reinterpret_cast<void*>(self->m_navMesh);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, navMesh, 0)[0x20 / 4])(navMesh, nullptr);
    }

    if (self->m_ownedAreaMesh)
        AreaMesh_Shutdown(reinterpret_cast<void*>(self->m_ownedAreaMesh), nullptr);

    if (self->m_navMesh)
    {
        void* navMesh = reinterpret_cast<void*>(self->m_navMesh);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, navMesh, 0)[0])(navMesh, nullptr, 1);
        self->m_navMesh = 0;
    }

    if (self->m_areaMesh)
    {
        void* areaMesh = reinterpret_cast<void*>(self->m_areaMesh);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, areaMesh, 0)[0])(areaMesh, nullptr, 1);
        self->m_areaMesh = 0;
    }

    if (self->m_dummyMesh)
    {
        void* dummyMesh = reinterpret_cast<void*>(self->m_dummyMesh);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, dummyMesh, 0)[0])(dummyMesh, nullptr, 1);
        self->m_dummyMesh = 0;
    }

    if (self->m_lightMesh)
    {
        void* lightMesh = reinterpret_cast<void*>(self->m_lightMesh);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, lightMesh, 0)[0])(lightMesh, nullptr, 1);
        self->m_lightMesh = 0;
    }

    if (self->m_sceneObjMesh)
    {
        void* sceneObjMesh = reinterpret_cast<void*>(self->m_sceneObjMesh);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, sceneObjMesh, 0)[0])(sceneObjMesh, nullptr, 1);
        self->m_sceneObjMesh = 0;
    }

    self->m_ownedNavMesh = 0;
    self->m_ownedAreaMesh = 0;
    self->m_dummyMesh = 0;
    self->m_lightMesh = 0;
    self->m_sceneObjMesh = 0;

    return CLevelData_ReleaseLoadedData(reinterpret_cast<CLevelData*>(self), nullptr);
}

HOOK(0x004E92B0, CGameLevelData_ReleaseLoadedData, "auto");
