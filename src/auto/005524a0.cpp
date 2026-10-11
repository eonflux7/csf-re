#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, sub_551680, 0x00551680, (void* self, void* edx, int a2, int a3, int a4, int a5));
CSF_FUNC(int, __fastcall, sub_73BE00, 0x0073BE00, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, sub_73CD30, 0x0073CD30, (void* self, void* edx));

// Reads a handle record (+0x08 handle, +0x10 signed index), resolves it in the current scene,
// updates the associated stats object and dispatches the team spawn selection.
static C_00833908* __fastcall MpSpawn_ApplyTeamSpawnFromHandle(C_00844694* self, void* /*edx*/, int recordAddr)
{
    uint8_t* record = reinterpret_cast<uint8_t*>(recordAddr);
    C_00833908* result = reinterpret_cast<C_00833908*>(recordAddr);

    if (CSF_AT(int, record, 0x10) >= 0)
    {
        int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, recordAddr + 8);
        result = reinterpret_cast<C_00833908*>(resolved);
        if (resolved != 0)
        {
            uint32_t sceneObject = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(resolved), 0x18);
            result = reinterpret_cast<C_00833908*>(sceneObject);
            if (sceneObject != 0 && sceneObject != 0x80)
            {
                uint8_t* statsRecord = reinterpret_cast<uint8_t*>(sceneObject) - 0x80;
                sub_73BE00(statsRecord, nullptr, 1);

                uint32_t playerObject = self->field_0c8c;
                result = reinterpret_cast<C_00833908*>(playerObject);
                if (playerObject != 0)
                {
                    uint32_t playerData = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(playerObject), 0x18);
                    result = reinterpret_cast<C_00833908*>(playerData);
                    if (playerData != 0)
                    {
                        uint8_t* teamData = reinterpret_cast<uint8_t*>(playerData) - 1712;
                        result = reinterpret_cast<C_00833908*>(teamData);
                        if (teamData != nullptr)
                        {
                            uint8_t teamFlag = CSF_AT(uint8_t, teamData, 0x443);
                            int globalValue = GetGlobalDword_73C020();
                            int spawnKey = sub_73CD30(reinterpret_cast<void*>(globalValue), nullptr);
                            int flagDelta = (teamFlag != 1) ? 1 : 0;
                            return reinterpret_cast<C_00833908*>(
                                sub_551680(self, nullptr, 1396846643 - flagDelta, spawnKey, -1, -1));
                        }
                    }
                }
            }
        }
    }
    return result;
}

HOOK(0x005524A0, MpSpawn_ApplyTeamSpawnFromHandle, "auto");
