#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(float, __fastcall, MenuSystem_GetTransitionAlpha, 0x0050D810, (void* self, void*));
CSF_FUNC(int, __fastcall, C_0083e160_GetLinkedValueDC, 0x004E1110, (C_0083e160* self, void*));

// __thiscall, ret 0: returns the scaled linked value as an unsigned 64-bit result.
static uint64_t __fastcall C_0083e5e8_GetTransitionScaledValue(C_0083e5e8* self, void* /*edx*/) {
    int globalState = GetGlobalState_50D190();
    float transitionAlpha = MenuSystem_GetTransitionAlpha(
        reinterpret_cast<void*>(static_cast<uintptr_t>(globalState)), nullptr);
    int linkedValue = C_0083e160_GetLinkedValueDC(self, nullptr);
    double scaled = static_cast<double>(linkedValue) * static_cast<double>(transitionAlpha);
    return static_cast<uint64_t>(static_cast<int64_t>(scaled));
}

HOOK(0x004E1970, C_0083e5e8_GetTransitionScaledValue, "auto");
