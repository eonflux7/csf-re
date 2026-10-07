#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x64B250: the real method the thunk forwards to. It is reached with an already
// adjusted `this` in ECX and no stack arguments (ret 0), so it behaves like a
// __thiscall(void) returning int.
CSF_FUNC(int, __fastcall, AdjustedThisTargetFunction, 0x64B250, (void* self, void* edx));

// 0x73BF70: compiler-generated adjustor thunk.
//   add ecx, -0x80
//   jmp 0x64B250
// It rewinds `this` from a sub-object to the enclosing object (0x80 bytes earlier)
// and tail-calls the real method at 0x64B250.
static int __fastcall AdjustThisByMinus80(void* self, void* /*edx*/)
{
    return AdjustedThisTargetFunction(static_cast<std::uint8_t*>(self) - 0x80, nullptr);
}

HOOK(0x73BF70, AdjustThisByMinus80, "auto");
