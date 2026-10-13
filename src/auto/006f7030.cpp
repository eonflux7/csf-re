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

CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(int, __fastcall, C_00860894_Init, 0x006C45F0, (C_00860894* self, void*, int a2, int a3));
CSF_FUNC(int, __fastcall, C_008670f0_SetField0C, 0x006C4620, (C_008670f0* self, void*, int value));
CSF_FUNC(int, __fastcall, C_0085e7b8_GetSceneRegionAt, 0x004FC130, (C_0085e7b8* self, void*, int index));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());

// Constructor-style init: sets vtable, initializes embedded member at +8,
// stores the argument, and binds the member to the scene region at index 1.
static C_0086287c* __fastcall C_0086287c_Construct(C_0086287c* self, void* /*edx*/, int a2)
{
    C_00860178_Construct(self, nullptr);
    *(void***)self = CSF_VTBL(0x0086287C);

    C_00860894* memberAt8 = reinterpret_cast<C_00860894*>(self->pad_0008);
    C_00860894_Init(memberAt8, nullptr, -1, 0);

    self->field_0004 = static_cast<uint32_t>(a2);

    int globalValue = GetGlobalValue_6C5760();
    C_0085e7b8* sceneRoot = CSF_AT(C_0085e7b8*, reinterpret_cast<void*>(globalValue), 0x4);
    int sceneRegion = C_0085e7b8_GetSceneRegionAt(sceneRoot, nullptr, 1);

    C_008670f0_SetField0C(reinterpret_cast<C_008670f0*>(self->pad_0008), nullptr, sceneRegion);
    return self;
}

HOOK(0x6F7030, C_0086287c_Construct, "auto");
