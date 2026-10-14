#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, ResolveSpawnTeamFlag, 0x0073CD30, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ApplySpawnPointSelection, 0x007495F0, (void* self, void* edx, int arg0, void* a3, void* a2, int a11));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static uint32_t* __fastcall SpawnPoints_ApplyAndReleaseBlock(void* self, void* /*edx*/, char a2, void* Block, int a4, int a5, int a6, int a7, unsigned int a8)
{
    uint32_t* result = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0xC88));
    if (result)
    {
        result = reinterpret_cast<uint32_t*>(result[1]);
        if (result)
        {
            void* spawnObj = reinterpret_cast<uint8_t*>(result) - 0x440;
            int globalTeamValue = GetGlobalDword_73C020();
            int teamFlag = ResolveSpawnTeamFlag(reinterpret_cast<void*>(static_cast<intptr_t>(globalTeamValue)), nullptr);
            int status = ApplySpawnPointSelection(spawnObj, nullptr, teamFlag,
                                                   reinterpret_cast<void*>(static_cast<uintptr_t>(-1)),
                                                   static_cast<void*>(&a2), 0);
            result = reinterpret_cast<uint32_t*>(static_cast<intptr_t>(status));
        }
    }
    if (a8 >= 8)
    {
        result = static_cast<uint32_t*>(Block);
        if (Block)
            return reinterpret_cast<uint32_t*>(Mem_FreeDebug(Block));
    }
    return result;
}

HOOK(0x554130, SpawnPoints_ApplyAndReleaseBlock, "auto");
