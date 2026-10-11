#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Original 0x004059D0: __fastcall (this in ecx, edx unused, one stack int), pops 4.
CSF_FUNC(char, __fastcall, sub_4059D0, 0x004059D0, (void* self, void* edx, int a2));

// Vtable slot forwarder of C_0082d510. The original is a bare tail jump to
// 0x4059D0 with no ecx read of its own, so this is written as a plain __stdcall
// function that forwards its single argument. Stdcall pops the 4-byte argument,
// matching the callee's ret 4.
static char __stdcall C_0082d510_ForwardSlotCall(int a2)
{
    return sub_4059D0(nullptr, nullptr, a2);
}

HOOK(0x004053E0, C_0082d510_ForwardSlotCall, "auto");
