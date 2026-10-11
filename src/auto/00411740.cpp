#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_40FBA0, 0x0040FBA0, (void* self, void* edx, int a2, int a3));

// Original: 0x411740, __thiscall with two stack args (ret 8).
// Forwards to the base-class routine at 0x40FBA0, then sets flag 0x20 in field_0030.
// The callee's AL result is returned unchanged.
static char __fastcall C_0082dfd0_CallBaseAndSetFlag20(C_0082dfd0* self, void* /*edx*/, int a2, int a3)
{
    char result = sub_40FBA0(self, nullptr, a2, a3);
    self->field_0030 |= 0x20u;
    return result;
}

HOOK(0x411740, C_0082dfd0_CallBaseAndSetFlag20, "auto");
