// Commandos: Strike Force — adjustor thunk at 0x6BF330.
// The incoming `this` points at a sub-object embedded at +4 inside the
// complete object; this thunk rewinds it by 4 bytes and tail-jumps into
// sub_6B35C0 (a __thiscall with no stack arguments, ret 0).
//
// 006bf330  sub ecx, 4
// 006bf333  jmp 0x6b35c0

#include <cstdint>
#include "globals.h"
#include "hook.h"

// Real implementation reached by the tail jump: __thiscall, ret 0.
CSF_FUNC(char*, __fastcall, GetInnerObjectAtOffset4, 0x6B35C0, (char* self, void* edx));

static char* __fastcall AdjustThisMinus4AndForward(char* self, void* /*edx*/)
{
    return GetInnerObjectAtOffset4(self - 4, nullptr);
}

HOOK(0x6BF330, AdjustThisMinus4AndForward, "auto");
