#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Binding for the global-value getter (cdecl, no stack args, result in eax).
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());

// Binding for the __fastcall-style method that tests a mask against the global object.
// ecx = object, mask argument popped by callee (ret 4), result in eax.
CSF_FUNC(int, __fastcall, C_008656d0_IsMaskSubset, 0x006C5780, (void* self, void* /*edx*/, int mask));

// Returns whether mask 1024 is a subset of the flags of the global object
// returned by GetGlobalValue_6C5760. Plain function: it never reads ecx itself.
static int __cdecl GfxScene_IsGlobalValueMaskSubset1024()
{
    const int kSubsetMask = 1024;

    // The getter's eax becomes the 'this' pointer for the next call.
    void* globalValueObject = reinterpret_cast<void*>(
        static_cast<uintptr_t>(static_cast<uint32_t>(GetGlobalValue_6C5760())));

    return C_008656d0_IsMaskSubset(globalValueObject, nullptr, kSubsetMask);
}

HOOK(0x006CAC40, GfxScene_IsGlobalValueMaskSubset1024, "auto");
