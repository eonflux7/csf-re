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

CSF_FUNC(void*, __fastcall, S_0046bea0_AssignNineElements, 0x006D9C40, (S_0046bea0* self, void* , int source));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_0085e7b8_GetSceneRegionAt, 0x004FC130, (C_0085e7b8* self, void* , int index));
CSF_FUNC(int, __fastcall, C_008670f0_SetField0C, 0x006C4620, (C_008670f0* self, void* , int value));

// Initializes the PTank's nine-element block from the argument, then stores the
// scene region (index 13) of the global context into the sub-object at +0x11C,
// and marks the tank as initialized (byte at +0x98).
static int __fastcall PTank_InitFromSceneRegion(void* self, void* /*edx*/, int a2)
{
    uint8_t* tank = static_cast<uint8_t*>(self);

    S_0046bea0_AssignNineElements(reinterpret_cast<S_0046bea0*>(tank + 8), nullptr, a2);

    uint8_t* globalContext = reinterpret_cast<uint8_t*>(GetGlobalValue_6C5760());
    C_0085e7b8* sceneRegionHolder = CSF_AT(C_0085e7b8*, globalContext, 4);
    int sceneRegion = C_0085e7b8_GetSceneRegionAt(sceneRegionHolder, nullptr, 13);

    int result = C_008670f0_SetField0C(reinterpret_cast<C_008670f0*>(tank + 0x11C), nullptr, sceneRegion);
    CSF_AT(uint8_t, tank, 0x98) = 1;
    return result;
}

HOOK(0x006D9CC0, PTank_InitFromSceneRegion, "auto");
