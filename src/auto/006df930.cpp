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
CSF_FUNC(int, __fastcall, C_008611d4_Construct, 0x006D9B10, (C_008611d4* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(void, __stdcall, C_0085ec18_NullSub, 0x004B2A20, (int));
CSF_FUNC(int, __fastcall, PTank_InitFromSceneRegion, 0x006D9CC0, (void* self, void* , int a2));

// Lazily creates the tank object held at +0x48 (316-byte block, constructed via
// C_008611d4_Construct), then forwards the scene region to PTank_InitFromSceneRegion
// using the stored object as `this`.
static int __fastcall C_00861988_EnsureTankAndInitFromSceneRegion(C_00861988* self, void* /*edx*/, int sceneRegion)
{
    if (self->field_0048 == 0)
    {
        C_008611d4* tankObject = reinterpret_cast<C_008611d4*>(CoreMemory_AllocUntagged(316));
        uint32_t constructedTank = 0;
        if (tankObject != nullptr)
            constructedTank = static_cast<uint32_t>(C_008611d4_Construct(tankObject, nullptr));

        self->field_0048 = constructedTank;
        GetGlobalValue_6C5760();
        C_0085ec18_NullSub(static_cast<int>(constructedTank));
    }

    void* tank = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0048));
    return PTank_InitFromSceneRegion(tank, nullptr, sceneRegion);
}

HOOK(0x006DF930, C_00861988_EnsureTankAndInitFromSceneRegion, "auto");
