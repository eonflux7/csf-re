#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x004A40E0, __fastcall-style forwarder (this in ecx, edx unused, one stack arg).
CSF_FUNC(int, __fastcall, CopyStructuredMember, 0x004A40E0, (void* self, void* edx, void* a5));

// Tail-jumps to CopyStructuredMember with this+0x68 as the receiver and a2 forwarded
// unchanged. The callee pops the stack argument, so this wrapper's ret is 4.
static int __fastcall C_008344f0_CopyMemberAt68ToArg(C_008344f0* self, void* /*edx*/, int* a2)
{
    void* member = reinterpret_cast<uint8_t*>(self) + 0x68;
    return CopyStructuredMember(member, nullptr, a2);
}

HOOK(0x004A4330, C_008344f0_CopyMemberAt68ToArg, "auto");
