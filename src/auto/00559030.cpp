#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee at 0x00558DA0 is __cdecl. The original pushes four dwords (arg1, arg0-as-second,
// ECX, arg1), so the callee receives (arg0, arg1, ECX, arg1). The packet's bind line lists
// only two parameters, but the caller cleans four dwords (add esp, 0x10), so the four-argument
// form is used here.
CSF_FUNC(int, __cdecl, MpRules_ProcessTeamPair, 0x00558DA0,
         (int firstArgument, int secondArgument, std::uintptr_t context, int repeatedArgument));

// 0x00559030: __stdcall thunk with two stack arguments (ret 8). ECX arrives as the first
// __fastcall parameter and is forwarded as the third callee argument.
static int __fastcall MpRules_ForwardTeamPairArguments(
    void* context, void* /*edx*/, int firstArgument, int secondArgument)
{
    return MpRules_ProcessTeamPair(
        firstArgument,
        secondArgument,
        reinterpret_cast<std::uintptr_t>(context),
        secondArgument);
}

HOOK(0x00559030, MpRules_ForwardTeamPairArguments, "auto");
