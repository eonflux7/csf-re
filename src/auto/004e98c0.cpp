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

// Looks up a global value slot for (id, index); the two values are pushed by the caller
// and consumed by the thiscall below, so they are passed here.
CSF_FUNC(int, __cdecl, GetGlobalValue8BC1EC, 0x00782600, (int a1, int a2));

// Returns the address of a float slot in the looked-up entry (__thiscall, ret 8).
CSF_FUNC(int, __fastcall, SceneEntry_GetFloatSlotAddress, 0x00782820, (void* self, void* edx, int a2, int a3));

// Returns the float stored at offset 0x10 of the looked-up scene slot (original: __stdcall, ret 4).
static double __stdcall GuiControl_QueryFloatValue(uint32_t* a1)
{
    int objectId = SceneObject_ResolveFromArg(a1);

    uint8_t* globalState = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalState_50D190()));
    int stateIndex = CSF_AT(int, globalState, 0x84);

    int slotEntry = GetGlobalValue8BC1EC(objectId, stateIndex);
    int floatSlotAddress = SceneEntry_GetFloatSlotAddress(
        reinterpret_cast<void*>(static_cast<uintptr_t>(slotEntry)), nullptr, objectId, stateIndex);

    uint8_t* slot = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(floatSlotAddress));
    return static_cast<double>(CSF_AT(float, slot, 0x10));
}

HOOK(0x004E98C0, GuiControl_QueryFloatValue, "auto");
