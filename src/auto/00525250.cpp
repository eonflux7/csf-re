#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Binding for the global-value getter (cdecl, no stack args, result in eax).
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());

// Binding for the __thiscall-style method that applies a float to the global object.
// ecx = object, float argument popped by callee (ret 4).
CSF_FUNC(unsigned int, __fastcall, ApplyFloatToGlobalObject, 0x005221E0, (void* self, void* edx, float a2));

// Applies the constant 1000002.0 to the global object returned by GetGlobalValue_521BB0.
// The original pushes the float constant before calling the getter (cdecl, so it stays
// on the stack) and then passes it to ApplyFloatToGlobalObject, which pops it (ret 4).
static unsigned int __cdecl SoundManager_ApplyGlobalFloatValue()
{
    const float kGlobalFloatValue = 1000002.0f;

    // The getter's eax becomes the 'this' pointer for the next call.
    void* globalValueObject = reinterpret_cast<void*>(
        static_cast<uintptr_t>(static_cast<uint32_t>(GetGlobalValue_521BB0())));

    return ApplyFloatToGlobalObject(globalValueObject, nullptr, kGlobalFloatValue);
}

HOOK(0x00525250, SoundManager_ApplyGlobalFloatValue, "auto");
