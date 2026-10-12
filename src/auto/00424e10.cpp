#include <cstdint>
#include "globals.h"
#include "hook.h"

// __fastcall callee: ecx = this, edx unused, one stack argument popped by the callee (ret 4).
CSF_FUNC(int, __fastcall, TCharTask_ForwardUpdateToChild, 0x4DB0F0, (void* self, void* edx, int a2));

// Original 0x424E10: push ebp / mov ebp,esp / pop ebp / jmp 0x4DB0F0.
// Nothing writes ecx or edx before the jump, so the jump forwards the caller's ecx and edx unchanged.
// Declaring them as __fastcall parameters makes this wrapper receive and forward them, and the
// single stack argument is popped by this callee, matching the original's ret 4.
static int __fastcall TCharTask_ForwardUpdateWrapper(void* self, void* edx, int a2)
{
    return TCharTask_ForwardUpdateToChild(self, edx, a2);
}

HOOK(0x424E10, TCharTask_ForwardUpdateWrapper, "auto");
