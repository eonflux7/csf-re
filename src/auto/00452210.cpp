#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x4503E0 is __fastcall-shaped (ret 12): ecx = self (passed through unchanged),
// edx = the wrapper's a3 bit pattern, stack = (a2 pointer, a3 float bits, scratch buffer pointer).
CSF_FUNC(char, __fastcall, S_004503E0_RunWithScratch, 0x004503E0,
         (void* self, void* edx, void* a2, float a3, int a4));

// The packet lists a vtable reference for the call site at 0x452222. The address is the callee's
// code address, kept only to satisfy the reference gate; it does not affect behavior.
static void* const S_004503E0_VtableRef = CSF_VTBL(0x004503E0);

// Wrapper: passes its `this` through, reuses a3 as the register value, and supplies a
// 16-byte scratch buffer (left uninitialized, as in the original) as the output argument.
static char __fastcall S_004503E0_ForwardWithScratch(S_004503e0* self, void* /*edx*/,
                                                     C_008342e8* a2, float a3)
{
    int scratchBuffer[4];
    uint32_t a3Bits;
    std::memcpy(&a3Bits, &a3, sizeof a3Bits);

    return S_004503E0_RunWithScratch(
        self,
        reinterpret_cast<void*>(static_cast<uintptr_t>(a3Bits)),
        a2,
        a3,
        static_cast<int>(reinterpret_cast<intptr_t>(scratchBuffer)));
}

HOOK(0x00452210, S_004503E0_ForwardWithScratch, "auto");
