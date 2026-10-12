#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x4FC210: toggles the mode flag on the object (ret 4).
CSF_FUNC(void, __fastcall, GlobalValue4FBDE0_SetToggleMode, 0x004FC210, (void* self, void* edx, int a3));

// Clears the toggle on the object when its stored id at +0x7C equals the argument.
static void __fastcall CommandoAnims_ClearToggleOnMatch(void* self, void* /*edx*/, int a2)
{
    if (CSF_AT(int, self, 0x7C) == a2)
        GlobalValue4FBDE0_SetToggleMode(self, nullptr, 0);
}

HOOK(0x4FC390, CommandoAnims_ClearToggleOnMatch, "auto");
