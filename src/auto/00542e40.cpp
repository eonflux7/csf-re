// Commandos: Strike Force (2006) — menu_controls / CMenuControls
// 0x00542E40: if a state field equals 3, replace it with -1 (none/invalid).

#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// __thiscall(void* this, int a2) -> ecx = this, edx unused, callee pops 4 bytes.
static void __fastcall CMenuControls_ResetStateFieldIfThree(void* self, void* /*edx*/, int /*a2*/)
{
    if (CSF_AT(int32_t, self, 0x2d8) == 3)
        CSF_AT(int32_t, self, 0x2d8) = -1;
}

HOOK(0x542E40, CMenuControls_ResetStateFieldIfThree, "auto");
