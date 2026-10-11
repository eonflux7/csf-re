#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound by address: CharTask base movement starter (__fastcall, one stack arg, ret 4).
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

// Starts character movement using the sub-object embedded at +0x38 (pad_0020 + 24) as the argument.
static int __fastcall C_0082f9f8_StartCharMovement(C_0082f9f8* self, void* /*edx*/) {
    int innerObjectAddress = (int)(uintptr_t)((uint8_t*)self + 0x38);
    return CharTask_StartMovement(self, nullptr, innerObjectAddress);
}

HOOK(0x0047B140, C_0082f9f8_StartCharMovement, "auto");
