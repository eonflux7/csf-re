#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall Game_SetPendingState(void* self, void* /*edx*/, const void* state)
{
    void* related = CSF_AT(void*, self, 0xA8);
    CSF_AT(uint8_t, self, 0x171) = 1;
    CSF_AT(uint8_t, related, 0x1B) = 1;

    CSF_AT(uint32_t, self, 0x148) = CSF_AT(uint32_t, state, 0x04);
    CSF_AT(uint32_t, self, 0x14C) = CSF_AT(uint32_t, state, 0x08);
    CSF_AT(uint8_t, self, 0x150) = CSF_AT(uint8_t, state, 0x0C);
    CSF_AT(uint32_t, self, 0x154) = CSF_AT(uint32_t, state, 0x10);
    CSF_AT(uint32_t, self, 0x158) = CSF_AT(uint32_t, state, 0x14);
    CSF_AT(uint32_t, self, 0x15C) = CSF_AT(uint32_t, state, 0x18);
    CSF_AT(uint32_t, self, 0x160) = CSF_AT(uint32_t, state, 0x1C);
    const int result = CSF_AT(int, state, 0x20);
    CSF_AT(int, self, 0x164) = result;
    return result;
}

HOOK(0x0048F7C0, Game_SetPendingState, "auto");
