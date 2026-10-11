#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound by address: TFreeMove_Destroy(this, freeMemory), __thiscall, ret 4.
CSF_FUNC(char*, __fastcall, TFreeMove_Destroy, 0x004215E0, (TFreeMove* self, void* /*edx*/, char freeMemory));

// Tail-jump forwarder: adjusts `this` by -4 (the embedded TFreeMove base sits
// at offset -4 from the attack task), then forwards the stack argument and the
// callee's return value unchanged. Pops 4 bytes, matching the original's `ret` 4
// (the callee's own ret 4 is what the caller sees, since this is a jmp).
static char* __fastcall AttackTasks_DestroyViaFreeMove(char* self, void* /*edx*/, char a2)
{
    return TFreeMove_Destroy(reinterpret_cast<TFreeMove*>(self - 4), nullptr, a2);
}

HOOK(0x0047BE40, AttackTasks_DestroyViaFreeMove, "auto");
