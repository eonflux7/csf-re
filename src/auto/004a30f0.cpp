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

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneSubsystem_LookupEntry, 0x00519AF0, (void* self, void* edx, int a2));

// Returns nonzero when the virtual lookup yields an id that the scene subsystem also resolves.
static int __fastcall C_008344f0_IsResolvedSceneEntry(C_008344f0* self, void* /*edx*/, int a2)
{
    int entryId = self->vSlot70(a2);
    if (entryId != -1)
    {
        // The subsystem pointer lives at +0x1FC4 inside the object returned by GetValue8B0554.
        uint8_t* sceneRoot = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetValue8B0554()));
        uint32_t sceneSubsystem = CSF_AT(uint32_t, sceneRoot, 0x1FC4);
        if (SceneSubsystem_LookupEntry(reinterpret_cast<void*>(static_cast<uintptr_t>(sceneSubsystem)), nullptr, entryId) == -1)
        {
            entryId = -1;
        }
    }
    return entryId != -1 ? 1 : 0;
}

HOOK(0x4A30F0, C_008344f0_IsResolvedSceneEntry, "auto");
