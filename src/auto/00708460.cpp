#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x006E82F0 HAnim_ResolveStackOrder, __cdecl, 2 args, caller pops.
CSF_FUNC(int, __cdecl, HAnim_ResolveStackOrder, 0x006E82F0, (int a1, int a2));

// Resolves the hierarchy stack order for this object's stored value and the
// argument, then forwards that result to the object's virtual slot +0x6C.
static int __fastcall C_00863430_ResolveAnimStackOrder(C_00863430* self, void* /*edx*/, int a2)
{
    int resolvedOrder = HAnim_ResolveStackOrder(static_cast<int>(self->field_0004), a2);
    return self->vSlot6C(resolvedOrder);
}

HOOK(0x708460, C_00863430_ResolveAnimStackOrder, "auto");
