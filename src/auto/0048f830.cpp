#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static char __fastcall SetPendingState(void* self, void* /*edx*/, const void* state)
{
    void* linkedObject = CSF_AT(void*, self, 0xA8);
    CSF_AT(uint8_t, self, 0x171) = 1;
    CSF_AT(uint8_t, linkedObject, 0x16) = 1;
    CSF_AT(uint32_t, self, 0x12C) = CSF_AT(uint32_t, state, 0x4);
    CSF_AT(uint32_t, self, 0x130) = CSF_AT(uint32_t, state, 0x8);
    char flag = CSF_AT(char, state, 0xC);
    CSF_AT(char, self, 0x134) = flag;
    return flag;
}

HOOK(0x0048F830, SetPendingState, "auto");
