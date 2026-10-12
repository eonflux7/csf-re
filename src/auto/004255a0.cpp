#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee: sub_40E430, __fastcall(self, edx, float). Stack float, ret 4.
CSF_FUNC(void, __fastcall, ApplyFloatToSubObject, 0x0040E430, (void* self, void* edx, float a2));

// Original: if (this->state_0020 == 2) tail-jump to 0x40E430 with ecx = this->field_0014->field_004C.
// Otherwise the result is an uninitialized eax, which is passed along as 0.
static int __fastcall ForwardFloatIfStateTwo(void* self, void* /*edx*/, float a2)
{
    if (CSF_AT(int, self, 0x20) == 2)
    {
        void* innerObject = CSF_AT(void*, self, 0x14);
        void* target = CSF_AT(void*, innerObject, 0x4C);
        ApplyFloatToSubObject(target, nullptr, a2);
        return 0;
    }
    return 0;
}

HOOK(0x4255A0, ForwardFloatIfStateTwo, "auto");
