#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Sets the environment preset value (offset 0x9C) of the sub-object stored at this+0xBC.
// Returns that sub-object pointer (left in eax by the original).
static int __fastcall SceneLoader_SetEnvironmentPreset(void* self, void* /*edx*/, int presetValue)
{
    uint32_t environmentObjectAddress = CSF_AT(uint32_t, self, 0xBC);
    CSF_AT(int, reinterpret_cast<void*>(environmentObjectAddress), 0x9C) = presetValue;
    return static_cast<int>(environmentObjectAddress);
}

HOOK(0x6CAF20, SceneLoader_SetEnvironmentPreset, "auto");
