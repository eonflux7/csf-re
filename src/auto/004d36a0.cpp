#include <cstdint>
#include "globals.h"
#include "hook.h"

// Marks the scene entity referenced by self->field_0054 as finalized (bit 0 of its
// flags dword at +4), unless it is already marked or self has flags 0x42 set.
// Returns the scene entity pointer (or 0) regardless of whether the bit was set.
static int __fastcall FinalizeSceneEntityTask(void* self, void* /*edx*/)
{
    int sceneEntityAddress = CSF_AT(int, self, 0x54);
    if (sceneEntityAddress != 0)
    {
        uint8_t* sceneEntity = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(sceneEntityAddress));
        if ((CSF_AT(uint8_t, sceneEntity, 4) & 1) == 0 &&
            (CSF_AT(uint8_t, self, 0x44) & 0x42) == 0)
        {
            CSF_AT(uint32_t, sceneEntity, 4) |= 1u;
        }
    }
    return sceneEntityAddress;
}

HOOK(0x4D36A0, FinalizeSceneEntityTask, "auto");
