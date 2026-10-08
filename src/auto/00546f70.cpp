#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee is __cdecl (ret 0): the caller pushes all four arguments, exactly as the
// original target does (a1, a2, incoming ecx, a2).
CSF_FUNC(int, __cdecl, MenuExtras_ResetStringRange, 0x00546D60,
         (int firstArgument, int secondArgument, std::uintptr_t context, int repeatedArgument));

// __stdcall shim: incoming ecx is carried through as a pseudo "this"/context value
// and forwarded to the worker together with the two stack arguments.
static int __fastcall MenuExtras_ForwardArgumentsToProcessor(
    void* context, void* /*edx*/, int firstArgument, int secondArgument)
{
    return MenuExtras_ResetStringRange(
        firstArgument,
        secondArgument,
        reinterpret_cast<std::uintptr_t>(context),
        secondArgument);
}

HOOK(0x00546F70, MenuExtras_ForwardArgumentsToProcessor, "auto");
