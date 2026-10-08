#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x00521BB0: no-argument accessor returning a global value (also used as a `this`).
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x521BB0, ());

// 0x0050D190: no-argument accessor returning the current global state object.
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x50D190, ());

// 0x00521F10: method invoked on the value returned by GetGlobalValue_521BB0. The original
// loads that value into ecx right before the call, so bind it as a __thiscall-style
// __fastcall (ecx = this, edx unused).
CSF_FUNC(int, __fastcall, Sub_521F10, 0x521F10, (void* self, void* edx));

// Tail-jump portion of the original method (0x0052D8FE .. 0x0052D902):
//     mov edx, dword ptr [eax]
//     mov ecx, eax
//     jmp dword ptr [edx + 0x64]
// The target (vtable slot 25 of the object returned by GetGlobalState_50D190) has an
// unknown signature, so forward registers/stack untouched via a naked thunk.
// The state pointer arrives in ecx (first __fastcall argument).
static __declspec(naked) int __fastcall CMenuScenarioSelect_JumpToGlobalStateSlot25(int /*state*/)
{
    __asm {
        mov eax, ecx
        mov edx, dword ptr [eax]
        mov ecx, eax
        jmp dword ptr [edx + 0x64]
    }
}

static int __fastcall CMenuScenarioSelect_ActivateSelection(CMenuScenarioSelect* self, void* /*edx*/)
{
    // (*(this->field_0014)->vtbl[0x60/4])(this->field_0014, this, 0)
    void* manager = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0014));
    reinterpret_cast<void (__fastcall*)(void*, void*, CMenuScenarioSelect*, int)>(
        CSF_AT(void**, manager, 0)[24])(manager, nullptr, self, 0);

    // GetGlobalValue_521BB0(); Sub_521F10(value);
    int globalValue = GetGlobalValue_521BB0();
    Sub_521F10(reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)), nullptr);

    // return GetGlobalState_50D190()->vtbl[0x64/4]();
    int state = GetGlobalState_50D190();
    return CMenuScenarioSelect_JumpToGlobalStateSlot25(state);
}

HOOK(0x52D8E0, CMenuScenarioSelect_ActivateSelection, "auto");
