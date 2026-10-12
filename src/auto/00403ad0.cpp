#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of the 4-dword inline storage at +0x10 when the
// size/capacity field at +0x24 is below 0x10 (unsigned compare, jb).
// Otherwise returns the pointer stored at +0x10 (heap buffer).
static uint32_t* __fastcall C_00867444_GetStorageBuffer(C_00867444* self, void* /*edx*/)
{
    if (CSF_AT(uint32_t, self, 0x24) < 0x10u)
        return reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x10);

    return reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x10)));
}

HOOK(0x403AD0, C_00867444_GetStorageBuffer, "auto");
