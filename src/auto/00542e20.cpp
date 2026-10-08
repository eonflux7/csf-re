// Commandos: Strike Force (2006) — menu_controls / CMenuControls
// 0x00542E20: if a state field equals 3, replace it with -1 (invalid/none).

#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// __thiscall(void* this, int, int) -> ecx = this, edx unused, callee pops 8 bytes.
static void __fastcall CMenuControls_ResetStateIfThree(void* self, void* /*edx*/, int /*a2*/, int /*a3*/)
{
    if (CSF_AT(int32_t, self, 0x2d8) == 3)
        CSF_AT(int32_t, self, 0x2d8) = -1;
}

HOOK(0x542E20, CMenuControls_ResetStateIfThree, "auto");
