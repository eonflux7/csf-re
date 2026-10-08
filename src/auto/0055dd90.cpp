#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

// 0x0054ABA0 — shared menu-side handler the thunk forwards to (already reimplemented in the
// packet). It is a __thiscall (callee pops its 4 stack bytes), so it is bound as a
// __fastcall free function with an unused edx.
CSF_FUNC(int, __fastcall, CMenuMpGameMode_SetBackButton, 0x54ABA0, (CMenuPlayerList* self, void* edx, int a2));

// 0x0055DD90
// push ebp / mov ebp, esp / pop ebp / jmp 0x54ABA0
// Pure tail thunk: forwards ecx (this) and the single stack argument unchanged to 0x54ABA0 and
// returns whatever that handler returns. The redundant frame setup has no observable effect.
static int __fastcall CMenuPlayerList_ForwardCall(CMenuPlayerList* self, void* /*edx*/, int a2)
{
    return CMenuMpGameMode_SetBackButton(self, nullptr, a2);
}

HOOK(0x55DD90, CMenuPlayerList_ForwardCall, "auto");
