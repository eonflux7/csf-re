#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_0083e248_ApplyDescriptor, 0x004E2FD0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, C_0083e248_GetState, 0x004E1320, (void* self, void* edx));

// Builds a 3-dword descriptor {vtable, -1, -1} on the stack, applies it to this object
// (the callee's result is not used), then returns the object's state.
static int __fastcall C_0083e248_ApplyDefaultDescriptorAndGetState(C_0083e248* self, void* /*edx*/)
{
    uint32_t descriptor[3];
    descriptor[1] = 0xFFFFFFFFu;
    descriptor[2] = 0xFFFFFFFFu;
    descriptor[0] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));

    C_0083e248_ApplyDescriptor(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(descriptor)), 0);
    return C_0083e248_GetState(self, nullptr);
}

HOOK(0x004E35E0, C_0083e248_ApplyDefaultDescriptorAndGetState, "auto");
