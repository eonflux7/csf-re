#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Original callee at 0x00727370 takes the two stack arguments reloaded from the
// caller plus the incoming ECX register value. The call site pushes 4 dwords and
// cleans the stack itself (cdecl).
CSF_FUNC(int, __cdecl, ForwardArgumentsToProcessor, 0x00727370,
         (int firstArgument, int secondArgument, std::uintptr_t context, int repeatedArgument));

// 0x007275D0: loads the two stack arguments, forwards them together with the
// incoming ECX (context) to 0x00727370, discarding the callee's left-overs by
// cleaning 0x10 bytes off the stack. Pops 8 bytes of its own stack args (ret 8).
static int __fastcall ForwardArgumentsWithContext(
    void* context, void* /*edx*/, int firstArgument, int secondArgument)
{
    return ForwardArgumentsToProcessor(
        firstArgument,
        secondArgument,
        reinterpret_cast<std::uintptr_t>(context),
        secondArgument);
}

HOOK(0x007275D0, ForwardArgumentsWithContext, "auto");
