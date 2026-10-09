#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __cdecl, Hud_ProcessChild, 0x0042F4F0,
         (int* child, int flag));

static void __fastcall Hud_ProcessChildrenIfEnabled(void* self, void* /*edx*/)
{
    if (CSF_AT(uint8_t, self, 0x126)) {
        const uint32_t childCount = CSF_AT(uint32_t, self, 0xB4);
        for (uint32_t i = 0; i < childCount; ++i) {
            Hud_ProcessChild(CSF_AT(int**, self, 0xAC)[i], 0);
        }
    }
}

HOOK(0x00430FE0, Hud_ProcessChildrenIfEnabled, "auto");
