#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_0082e520_PrepareInputForBinding, 0x0041E050, (C_0082e520* self, void* /*edx*/, int a2, int a3));

// Tail-jump wrapper. The original does push ebp / mov ebp,esp / pop ebp / jmp 0x41E050 without
// touching ecx, so the caller's ecx (this) reaches the callee unchanged. Declaring the wrapper
// __fastcall makes the compiler take ecx as `self` and forward it. The stack arguments a2/a3 are
// the same stack slots the callee reads, and the callee's own `ret 8` pops them.
static char __fastcall Vehicle_InitializeWheelData(C_0082e520* self, void* edx, int a2, int a3)
{
    return C_0082e520_PrepareInputForBinding(self, edx, a2, a3);
}

HOOK(0x0040F6F0, Vehicle_InitializeWheelData, "auto");
