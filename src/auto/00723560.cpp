#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Forward-argument helper: re-pushes its two arguments, the caller's context
// (incoming ecx) and the second argument again, then tail-calls the processor.
CSF_FUNC(int, __cdecl, ProcessForwardedArguments, 0x007233D0,
         (int firstArgument, int secondArgument, std::uintptr_t context, int repeatedArgument));

static int __fastcall ForwardArgumentsToProcessor(
    void* context, void* /*edx*/, int firstArgument, int secondArgument)
{
    return ProcessForwardedArguments(
        firstArgument,
        secondArgument,
        reinterpret_cast<std::uintptr_t>(context),
        secondArgument);
}

HOOK(0x00723560, ForwardArgumentsToProcessor, "auto");
