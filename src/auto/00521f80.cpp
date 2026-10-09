#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

// Gets the global value object, calls its vtable slot 0x70 and returns that result.
// The result is later used as the tail-jump target object (0 means "no result").
static int __cdecl ResolveFlushTargetObject() {
    int globalValue = GetGlobalValue_70DDE0();
    void* globalObject = reinterpret_cast<void*>(globalValue);

    using Slot28Fn = int(__fastcall*)(void* self, void* edx);
    Slot28Fn slot28 = reinterpret_cast<Slot28Fn>(CSF_AT(void**, globalObject, 0)[0x70 / 4]);
    return slot28(globalObject, nullptr);
}

// Pointer to the helper, so the naked thunk loads it as data and calls through a register.
static int (__cdecl* const g_FlushTargetResolver)() = ResolveFlushTargetObject;

// Naked entry: resolves the target object, returns 0 when there is none, otherwise
// tail-jumps to that object's vtable slot 5 (0x14) with the caller's stack untouched.
static __declspec(naked) int FlushGlobalValueState() {
    __asm {
        mov eax, dword ptr [g_FlushTargetResolver]
        call eax
        test eax, eax
        je no_result
        mov edx, dword ptr [eax]
        mov ecx, eax
        jmp dword ptr [edx + 0x14]
    no_result:
        ret
    }
}

HOOK(0x00521F80, FlushGlobalValueState, "auto");
