#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(bool, __fastcall, ParticleSystemDefs_InitSpawnState, 0x007012C0, (void* self, void* , char a2));

// Allocates and zero-initializes a 28-byte particle-system spawn-state block,
// sets its default counter to 10, runs the spawn-state initializer, and returns
// the block (or 0 when allocation fails, in which case the initializer still runs).
static int __cdecl ParticleSystemDefs_AllocSpawnState()
{
    void* allocated = Mem_Alloc(0x1c);
    if (allocated != nullptr)
    {
        uint8_t* spawnState = static_cast<uint8_t*>(allocated);
        CSF_AT(uint8_t, spawnState, 0x0) = 0;
        CSF_AT(int, spawnState, 0xC) = 0;
        CSF_AT(int, spawnState, 0x10) = 0;
        CSF_AT(int, spawnState, 0x14) = 0;
        CSF_AT(int, spawnState, 0x18) = 10;
        // Original: mov ecx, esi (esi = spawnState); push ecx (ecx was zeroed by xor ecx, ecx)
        ParticleSystemDefs_InitSpawnState(spawnState, nullptr, 0);
        return static_cast<int>(reinterpret_cast<std::uintptr_t>(spawnState));
    }

    // Original failure path: xor esi, esi; mov ecx, esi; push ecx (0).
    // ecx holds esi, which is 0 here, so this = 0 faithfully mirrors the original.
    void* failedSpawnState = nullptr;
    ParticleSystemDefs_InitSpawnState(failedSpawnState, nullptr, 0);
    return 0;
}

HOOK(0x6D9FA0, ParticleSystemDefs_AllocSpawnState, "auto");
