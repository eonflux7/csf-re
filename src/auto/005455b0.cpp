#include <cstdint>

#include "hook.h"
#include "globals.h"

// 0x545BA0 — a __thiscall callee that pops its single 4-byte stack
// argument (rets = 4). Bound by address; its `this` is expected to be
// the object pointer minus 4.
CSF_FUNC(void*, __fastcall, CMenuSaveGame_Dispatch, 0x545BA0, (void* self, void* edx, char a2));

// 0x5455B0
//   sub ecx, 4
//   jmp 0x545BA0
// Adjusts the incoming `this` pointer down by 4 bytes and tail-jumps
// into CMenuSaveGame_Dispatch (0x545BA0), forwarding the stack argument
// untouched. The tail jump means the callee's `ret 4` is what returns to
// the real caller, so this replacement is a __fastcall (ecx = this,
// edx unused) taking the char argument on the stack and popping 4 bytes.
static void* __fastcall CMenuSaveGame_AdjustThisThenDispatch(void* self, void* /*edx*/, char a2)
{
    return CMenuSaveGame_Dispatch(reinterpret_cast<uint8_t*>(self) - 4, nullptr, a2);
}

HOOK(0x5455B0, CMenuSaveGame_AdjustThisThenDispatch, "auto");
