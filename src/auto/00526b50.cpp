#include "hook.h"
#include "types/auto_classes.h"

// Not-taken path: this->field_0060 is null, so the result is the float comparison.
// Unordered (NaN) operands yield false, matching the fcomp/fnstsw/jp sequence.
static char __fastcall C_008411a0_IsRangeBelowLimit(C_008411a0* self, void* /*edx*/) {
    return static_cast<char>(self->field_0048 < self->field_0050);
}

// Taken path: forward to field_0060's vtable slot 0x3C with the original's stack args
// untouched, as a tail jump. Otherwise tail-jump to the comparison helper.
static __declspec(naked) void C_008411a0_ForwardToChildOrReturnRangeCheck() {
    __asm {
        mov edx, dword ptr [ecx + 0x60]
        test edx, edx
        je NoChild
        mov ecx, edx
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x3c]
    NoChild:
        jmp C_008411a0_IsRangeBelowLimit
    }
}

HOOK(0x00526B50, C_008411a0_ForwardToChildOrReturnRangeCheck, "auto");
