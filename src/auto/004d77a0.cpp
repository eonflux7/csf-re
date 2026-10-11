#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __fastcall, TCharTask_ForwardInnerVirtualCall, 0x004DB260, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

// Forwards the inner virtual call, then starts movement on the sub-object at +0x68.
static int __fastcall C_0083dbb0_ForwardAndStartMovement(void* self, void* /*edx*/)
{
    TCharTask_ForwardInnerVirtualCall(self, nullptr);
    int subObjectAddress = reinterpret_cast<int>(static_cast<uint8_t*>(self) + 0x68);
    return CharTask_StartMovement(self, nullptr, subObjectAddress);
}

HOOK(0x004D77A0, C_0083dbb0_ForwardAndStartMovement, "auto");
