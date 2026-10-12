#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x0040E3D0 (bind as given in the packet).
CSF_FUNC(void, __fastcall, sub_40E3D0, 0x0040E3D0, (void* self, void* edx, float a2));

// Original: __stdcall(float) that reads ecx implicitly and tail-jumps.
// Reads self->field_0014 -> inner->field_004C, then forwards (inner, a1) to 0x40E3D0.
// Written as __fastcall so ecx is passed as `self`; the single float stack arg
// is popped by the callee, matching the original's 4-byte pop.
static int __fastcall ForwardFloatToNestedChild(void* self, void* /*edx*/, float a1)
{
    void* inner = CSF_AT(void*, self, 0x14);
    void* child = CSF_AT(void*, inner, 0x4C);
    sub_40E3D0(child, nullptr, a1);
    return 0;
}

HOOK(0x425590, ForwardFloatToNestedChild, "auto");
