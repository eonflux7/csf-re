#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Game heap allocator (cdecl, returns block or nullptr).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Particle system source/enable setter (__fastcall, two stack args, ret 8).
CSF_FUNC(char, __fastcall, ParticleSystemDefs_SetSourceAndEnabledFlag, 0x007012A0, (void* self, void* , int a2, char a3));

// Allocates a 28-byte projection record, zero-initializes it, sets the
// default value 10 at +0x18, and registers the particle source/enable flag.
// Returns the record, or 0 if the allocation failed.
static int GfxCameraProjection_CreateRecord()
{
    int recordPtr = 0;
    void* allocated = Mem_Alloc(28);
    recordPtr = reinterpret_cast<int>(allocated);
    int result = 0;
    if (recordPtr)
    {
        CSF_AT(uint8_t, recordPtr, 0) = 0;
        CSF_AT(int, recordPtr, 0x0C) = 0;
        CSF_AT(int, recordPtr, 0x10) = 0;
        CSF_AT(int, recordPtr, 0x14) = 0;
        CSF_AT(int, recordPtr, 0x18) = 10;
        ParticleSystemDefs_SetSourceAndEnabledFlag(reinterpret_cast<void*>(recordPtr), nullptr, 897988541, 0);
        return recordPtr;
    }
    return result;
}

HOOK(0x6F5C90, GfxCameraProjection_CreateRecord, "auto");
