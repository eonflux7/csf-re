#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall with no stack args, so `ret 0`. The receiver arrives in ecx and is
// passed here as `self`. The class is not identified, so the offsets go through CSF_AT.
static int __fastcall ClearPendingFlagAndGetInnerBlockAddress(void* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x158) = 1;
    CSF_AT(uint32_t, self, 0x150) = 0;
    return reinterpret_cast<int>(static_cast<uint8_t*>(self) + 0x148);
}

HOOK(0x44FFD0, ClearPendingFlagAndGetInnerBlockAddress, "auto");
