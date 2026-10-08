#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

// 0x00536B70 - __thiscall, pops nothing (ret 0). Called with ecx = this.
CSF_FUNC(int, __fastcall, MenuControls_Sub_536B70, 0x536B70, (void* self, void* edx));

// CMenuControls::... : calls 0x536B70, then if the byte flag at +0x27C is set,
// clears the dword at +0x278. Returns the flag byte (bare `mov al` -> char).
static char __fastcall CMenuControls_ClearValueIfFlagSet(void* self, void* /*edx*/)
{
    MenuControls_Sub_536B70(self, nullptr);

    char flag = CSF_AT(char, self, 0x27C);
    if (flag)
        CSF_AT(uint32_t, self, 0x278) = 0;

    return flag;
}

HOOK(0x543BA0, CMenuControls_ClearValueIfFlagSet, "auto");
