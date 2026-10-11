#include <cstdint>
#include "globals.h"
#include "hook.h"

static int __fastcall ResetPendingStateAndGetInnerBlockAddress(void* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0xFC) = 1;
    CSF_AT(uint32_t, self, 0xF4) = 0;
    return reinterpret_cast<int>(static_cast<uint8_t*>(self) + 0xEC);
}

HOOK(0x445F20, ResetPendingStateAndGetInnerBlockAddress, "auto");
