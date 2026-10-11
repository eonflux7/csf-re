#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee at 0x4E2C30: __fastcall, self in ecx, edx unused, one stack arg, ret 4.
CSF_FUNC(void*, __fastcall, ProcessSubobjectFrom4E2C30, 0x004E2C30, (void* self, void* edx, char a2));

// 0x4E1940: adjusts `this` back by 4 bytes, then tail-jumps to 0x4E2C30.
// Original is `sub ecx, 4; jmp 0x4E2C30`, so the stack arg (a2) is forwarded
// and popped by the callee (ret 4). This function is __thiscall with one stack
// arg, so it pops 4 bytes, matching the forwarded callee's ret 4.
static void* __fastcall ForwardWithThisOffsetMinus4(char* self, void* /*edx*/, char a2)
{
    return ProcessSubobjectFrom4E2C30(self - 4, nullptr, a2);
}

HOOK(0x004E1940, ForwardWithThisOffsetMinus4, "auto");
