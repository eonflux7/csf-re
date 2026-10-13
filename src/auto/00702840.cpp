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

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, GfxSkin_ApplySceneRegionToObject, 0x006C58D0, (C_0085e7b8** self, void* /*edx*/, int a2));
CSF_FUNC(int, __fastcall, C_008636fc_Init, 0x0070B6E0, (C_008636fc* self, void* /*edx*/, int initialParam));

// Initializes an atomic emitter record: stores the owner-supplied value at +8, allocates a
// 36-byte helper object, initializes it against this record, and applies the global scene region.
static char __fastcall AtomicEmitter_InitAndApplyRegion(void* self, void* /*edx*/, int arg0)
{
    CSF_AT(uint8_t, self, 0x4) = 0;
    CSF_AT(int, self, 0x8) = arg0;

    C_008636fc* helper = reinterpret_cast<C_008636fc*>(CoreMemory_AllocUntagged(36));
    int helperInitResult;
    if (helper)
        helperInitResult = C_008636fc_Init(helper, nullptr, static_cast<int>(reinterpret_cast<std::uintptr_t>(self)));
    else
        helperInitResult = 0;

    CSF_AT(int, self, 0xC) = helperInitResult;

    int sceneRegion = GetGlobalValue_6C5760();
    GfxSkin_ApplySceneRegionToObject(reinterpret_cast<C_0085e7b8**>(sceneRegion), nullptr, helperInitResult);

    CSF_AT(uint8_t, self, 0x4) = 1;
    return 1;
}

HOOK(0x00702840, AtomicEmitter_InitAndApplyRegion, "auto");
