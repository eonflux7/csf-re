#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x4DB440: one stack argument (ret 4), thiscall-style, bound as __fastcall.
CSF_FUNC(int, __fastcall, AiTaskStates_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

// 0x4210B0: returns AiTaskStates_LookupByOffset(this, this->field_000c + 40).
// The second argument is the 32-bit value at +0x0C plus 0x28, computed before the call.
static int __fastcall C_00834fa8_QueryAtFieldOffset0C(C_00834fa8* self, void* /*edx*/, int /*a2*/)
{
    uint32_t fieldValue = CSF_AT(uint32_t, self, 0x0C);
    int lookupArg = static_cast<int>(fieldValue + 0x28);
    return AiTaskStates_LookupByOffset(self, nullptr, lookupArg);
}

HOOK(0x004210B0, C_00834fa8_QueryAtFieldOffset0C, "auto");
