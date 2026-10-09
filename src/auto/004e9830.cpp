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

// Resolves the scene object referenced by the argument (original: sub_4E9100, __stdcall, ret 4).
CSF_FUNC(int, __stdcall, SceneObject_ResolveFromArg, 0x004E9100, (uint32_t* a1));

// Returns the global state block (original: GetGlobalState, __cdecl).
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

// Looks up a global value slot for (id, index); the original pushes two cdecl args
// that are left on the stack for the callee below to pop, so they are passed here.
CSF_FUNC(int, __cdecl, GetGlobalValue8BC1EC, 0x00782600, (int a1, int a2));

// Returns the address of a float slot in the looked-up entry (__thiscall, ret 8).
CSF_FUNC(int, __fastcall, SceneEntry_GetFloatSlotAddress, 0x00782820, (void* self, void* edx, int a2, int a3));

// Returns the float stored at offset 0x04 of the looked-up scene slot (original: __stdcall, ret 4).
static double __stdcall GetSceneObjectFloatAt0C(uint32_t* a1)
{
    int objectId = SceneObject_ResolveFromArg(a1);

    uint8_t* globalState = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalState_50D190()));
    int stateIndex = CSF_AT(int, globalState, 0x84);

    int slotEntry = GetGlobalValue8BC1EC(objectId, stateIndex);
    int floatSlotAddress = SceneEntry_GetFloatSlotAddress(
        reinterpret_cast<void*>(static_cast<uintptr_t>(slotEntry)), nullptr, objectId, stateIndex);

    return static_cast<double>(*reinterpret_cast<float*>(static_cast<uintptr_t>(floatSlotAddress) + 4));
}

HOOK(0x004E9830, GetSceneObjectFloatAt0C, "auto");
