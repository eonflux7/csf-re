#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee bindings (addresses from the packet).
// 0x004E9100: __stdcall, one stack arg, ret 4.
CSF_FUNC(int, __stdcall, SceneMissionSetup_ResolveHandle, 0x004E9100, (uint32_t* a1));
// 0x0050D190: __cdecl, no args, ret 0.
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
// 0x00782600: __cdecl, two stack args, ret 0. Caller-side cleanup is not emitted by the original;
// the following __thiscall pops the same two pushed values.
CSF_FUNC(void*, __cdecl, GetGlobalValue8BC1EC, 0x00782600, (int a1, int a2));
// 0x00782820: __thiscall (ecx = this, two stack args), ret 8.
CSF_FUNC(int, __fastcall, SceneMissionSetup_GetEntryAt, 0x00782820, (void* self, void* edx, int a2, int a3));

// Returns the 32-bit value stored at +0x0C of the scene entry resolved from a1.
static int __stdcall GetSceneEntryIntAt0C(uint32_t* a1) {
    int resolvedHandle = SceneMissionSetup_ResolveHandle(a1);

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    int stateValue = CSF_AT(int, globalState, 0x84);

    void* sceneObject = GetGlobalValue8BC1EC(resolvedHandle, stateValue);
    void* entry = reinterpret_cast<void*>(
        SceneMissionSetup_GetEntryAt(sceneObject, nullptr, resolvedHandle, stateValue));

    return CSF_AT(int, entry, 0xC);
}

HOOK(0x004E9890, GetSceneEntryIntAt0C, "auto");
