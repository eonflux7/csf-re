#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee at 0x0069C950 (__fastcall-style this, no stack args, ret 0).
CSF_FUNC(int, __fastcall, sub_69C950, 0x0069C950, (void* self, void* edx));

// Returns the value stored at +0x30 after the callee has run.
// __thiscall, ret 0: ecx = this, nothing popped from the stack.
static int __fastcall C_0085e92c_GetField0030(C_0085e92c* self, void* /*edx*/)
{
    sub_69C950(self, nullptr);
    return static_cast<int>(self->field_0030);
}

HOOK(0x0069CBE0, C_0085e92c_GetField0030, "auto");
