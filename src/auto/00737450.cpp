#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original 0x737B40: char* __thiscall(this, char a2), ret 4.
CSF_FUNC(char*, __fastcall, SubobjectResolveWithFlag, 0x00737B40, (void* self, void* edx, char a2));

// Thunk at 0x737450: `sub ecx, 4; jmp 0x737B40`. Adjusts this back by 4 and tail-forwards
// the single stack argument, so the callee's ret 4 pops it for us.
static char* __fastcall ForwardWithBaseOffsetMinus4(char* self, void* /*edx*/, char a2)
{
    return SubobjectResolveWithFlag(reinterpret_cast<C_00867260*>(self - 4), nullptr, a2);
}

HOOK(0x737450, ForwardWithBaseOffsetMinus4, "auto");
