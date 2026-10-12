#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_FUNC(int, __fastcall, C_00862b04_UpdateEffect, 0x006F98F0, (void* self, void* edx));

static void __fastcall C_00862b04_UpdateIfPositive(
    C_00862b04* self, void* /*edx*/, float delta)
{
    if (delta > g_fZero)
        C_00862b04_UpdateEffect(self, nullptr);
}

HOOK(0x006F9980, C_00862b04_UpdateIfPositive, "auto");
