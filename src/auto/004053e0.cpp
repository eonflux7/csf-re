#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Original 0x004059D0: __fastcall (this in ecx, edx unused, one stack int), pops 4.
CSF_FUNC(char, __fastcall, sub_4059D0, 0x004059D0, (void* self, void* edx, int a2));

// Vtable slot forwarder of C_0082d510: a bare tail jump to 0x4059D0, which reads ecx. The jump passes this and
// the stack argument straight through.
static char __fastcall C_0082d510_ForwardSlotCall(void* self, void* /*edx*/, int a2)
{
    return sub_4059D0(self, nullptr, a2);
}

HOOK(0x004053E0, C_0082d510_ForwardSlotCall, "auto");
