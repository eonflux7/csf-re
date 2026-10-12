#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Original callee: stack argument a2, ecx forwarded from the caller's this.
CSF_FUNC(int, __fastcall, sub_696AF0, 0x00696AF0, (void* self, void* edx, char* a2));

// Referenced by the original at the call site per the packet; the target is a code address.
[[maybe_unused]] static auto* const s_gateCallTargetRef = CSF_VTBL(0x00696AF0);

// Returns true (1) only when the gate check returns 0.
static int __fastcall MenuAccount_IsGateClear_696B60(S_00696af0* self, void* /*edx*/, char* a2)
{
    // ecx (self) is forwarded unchanged to the callee, as the original leaves ecx untouched.
    const int gateResult = sub_696AF0(self, nullptr, a2);
    return gateResult == 0 ? 1 : 0;
}

HOOK(0x696B60, MenuAccount_IsGateClear_696B60, "auto");
