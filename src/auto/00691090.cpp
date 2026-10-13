#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, C_0085eac0_GetPhysMemoryDelta, 0x0069E240, (C_0085eac0* self, void*));

CSF_GLOBAL(uint8_t, g_unk_8BAD98, 0x008BAD98);

static uint32_t SceneLoader_BeginMemoryMaintenance() {
    return C_0085eac0_GetPhysMemoryDelta(reinterpret_cast<C_0085eac0*>(&g_unk_8BAD98), nullptr);
}

HOOK(0x691090, SceneLoader_BeginMemoryMaintenance, "auto");
