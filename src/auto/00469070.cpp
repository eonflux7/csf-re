#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: sub_468FD0 (__thiscall, one stack arg). Its role is not yet known,
// so it keeps the descriptive name below until its own function is reimplemented.
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_ProcessSceneRegion, 0x00468FD0, (void* self, void* edx, int a2));

// Walks the scene-region pointer table (count at +0x20, array at +0x18) and
// invokes the region handler for every non-null entry whose first byte is non-zero.
static void __fastcall SceneLoader_ResetSceneRegions(S_004686a0* self, void* /*edx*/)
{
    uint32_t regionCount = self->field_0020;
    for (uint32_t i = 0; i < regionCount; ++i)
    {
        uint8_t* regionName = reinterpret_cast<uint8_t*>(
            reinterpret_cast<uint32_t*>(self->field_0018)[i]);
        if (regionName != nullptr && *regionName != 0)
        {
            SceneLoader_ProcessSceneRegion(self, nullptr,
                static_cast<int>(reinterpret_cast<uint32_t*>(self->field_0018)[i]));
        }
    }
}

HOOK(0x00469070, SceneLoader_ResetSceneRegions, "auto");
