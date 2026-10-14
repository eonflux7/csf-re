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

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, ComputeGlobalTeamCode, 0x0073CD30, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AssignSpawnNameString, 0x007495F0, (void* self, void* edx, int arg0, void* a3, void* a2, int a11));

// Original: __thiscall, 8 stack args, ret 0x20. Frees Block when a9 >= 8 and
// refreshes the spawn-side object held at this+0xC88 (first child's successor).
static uint32_t* __fastcall SpawnPoints_UpdateSideAndFreeBlock(void* self, void* /*edx*/,
                                                              int a2, char a3, void* Block,
                                                              int a5, int a6, int a7, int a8,
                                                              unsigned int a9)
{
    uint32_t* result = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0xC88));
    if (result) {
        result = reinterpret_cast<uint32_t*>(result[1]);
        if (result) {
            C_00858180* sideObject = reinterpret_cast<C_00858180*>(
                reinterpret_cast<uint8_t*>(result) - 0x440);
            int globalCode = GetGlobalDword_73C020();
            int teamByte = ComputeGlobalTeamCode(
                reinterpret_cast<void*>(static_cast<intptr_t>(globalCode)), nullptr);
            result = reinterpret_cast<uint32_t*>(static_cast<intptr_t>(
                AssignSpawnNameString(sideObject, nullptr,
                                      teamByte,
                                      reinterpret_cast<void*>(static_cast<intptr_t>(a2)),
                                      reinterpret_cast<void*>(&a3),
                                      0)));
        }
    }
    if (a9 >= 8) {
        result = reinterpret_cast<uint32_t*>(Block);
        if (Block) {
            return reinterpret_cast<uint32_t*>(static_cast<intptr_t>(Mem_FreeDebug(Block)));
        }
    }
    return result;
}

HOOK(0x5541F0, SpawnPoints_UpdateSideAndFreeBlock, "auto");
