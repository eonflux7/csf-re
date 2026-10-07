// CGuiStatic::ForwardFloat    0x00721060 (9 bytes)
//
// This entry point is a pure pass-through: it forwards (this, float) unchanged to
// the shared CGuiStatic implementation at 0x00720FD0 (the original is just
// push ebp / mov ebp,esp / pop ebp / jmp 0x720FD0, i.e. a tail transfer with the
// caller's stack argument left for the callee to pop).
//
// Declaring the forwarder as a __fastcall method (ecx = this, edx unused, float on
// the stack, callee pops 4) preserves the exact ABI contract: the caller pushes the
// float once and it is popped exactly once, whether clang emits a call+ret or folds
// it into a tail jmp.

#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Shared implementation that 0x00720FD0 forward-targets (__thiscall, ret 4).
CSF_FUNC(void, __fastcall, CGuiStatic_SharedFloatImpl, 0x00720FD0,
         (CGuiStatic* self, void* /*edx*/, float value));

static void __fastcall CGuiStatic_ForwardFloat(CGuiStatic* self, void* /*edx*/, float value)
{
    CGuiStatic_SharedFloatImpl(self, nullptr, value);
}

HOOK(0x00721060, CGuiStatic_ForwardFloat, "auto");
