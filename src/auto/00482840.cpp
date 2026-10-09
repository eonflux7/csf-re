#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Tail-forwarding thunk for 0x482840. The original is `push ebp / mov ebp,esp / pop ebp / jmp 0x480C10`,
// which passes ecx (this), edx and the stack argument straight through. Declaring this as
// __fastcall with one stack argument reproduces that: ecx and edx are forwarded unchanged, a2 stays
// on the stack, and the callee's `ret 4` pops it, so the net stack effect is 4 bytes.
CSF_FUNC(int, __fastcall, CharActionFsm_DispatchStateEvent, 0x00480C10, (void* self, void* edx, int a2));

static int __fastcall CharActionFsm_ForwardToStateDispatch(void* self, void* edx, int a2)
{
    return CharActionFsm_DispatchStateEvent(self, edx, a2);
}

HOOK(0x482840, CharActionFsm_ForwardToStateDispatch, "auto");
