#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_ResetMissionState, 0x004E92B0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, MissionData_ClearEntryList, 0x004E9980, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, CLevelData_ReleaseAll, 0x00781CF0, (CLevelData* self, void*));
CSF_FUNC(uint32_t*, __fastcall, MissionData_Release, 0x00782310, (void* self, void* edx));

// Destructor body of CGameLevelData: frees the mission data block, resets mission state,
// destroys the level path string and chains to the CLevelData base destructor.
static int __fastcall CGameLevelData_Destruct(CGameLevelData* self, void* /*edx*/)
{
    uint8_t* missionData = reinterpret_cast<uint8_t*>(self->m_dstData);

    self->vtbl = CSF_VTBL(0x0083ED40);
    CSF_AT(void*, self, 0x1FBC) = CSF_VTBL(0x0083ED24);

    if (missionData != nullptr)
    {
        MissionData_Release(missionData, nullptr);

        uint8_t* entryList = missionData + 4;
        MissionData_ClearEntryList(entryList, nullptr);
        if (CSF_AT(void*, entryList, 4) != nullptr)
            Mem_FreeDebug(CSF_AT(void*, entryList, 4));
        CSF_AT(void*, entryList, 4) = nullptr;

        Mem_Free(missionData);
        self->m_dstData = 0;
    }

    SceneLoader_ResetMissionState(self, nullptr, 0);

    // std::string m_levelPath teardown (heap buffer when capacity >= 16)
    if (self->field_1ffc >= 0x10 && self->m_levelPath != 0)
        Mem_FreeDebug(reinterpret_cast<void*>(self->m_levelPath));
    self->field_1ffc = 15;
    self->m_levelPathLength = 0;
    CSF_AT(char, self, 0x1FE8) = 0;

    CSF_AT(void*, self, 0x1FBC) = CSF_VTBL(0x0082D410);

    return CLevelData_ReleaseAll(self, nullptr);
}

HOOK(0x004EA020, CGameLevelData_Destruct, "auto");
