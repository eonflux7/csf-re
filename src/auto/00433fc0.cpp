#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee at 0x00433B90 is __cdecl with four forwarded values: the two original
// stack arguments, the incoming ECX (context), and the second argument repeated.
CSF_FUNC(int, __cdecl, Hud_ProcessWidgetPair, 0x00433B90,
         (int* firstArgument, int* secondArgument, std::uintptr_t context, int* repeatedArgument));

// 0x00433FC0: __stdcall thunk. Original pushes (arg1, arg0-as-second, ECX, arg1) in
// that order so the callee sees (arg0, arg1, ECX, arg1). ECX arrives as the first
// __fastcall parameter and the two stack arguments are callee-cleaned (ret 8).
static int __fastcall Hud_ForwardWidgetPairArguments(
    void* context, void* /*edx*/, int* firstArgument, int* secondArgument)
{
    return Hud_ProcessWidgetPair(
        firstArgument,
        secondArgument,
        reinterpret_cast<std::uintptr_t>(context),
        secondArgument);
}

HOOK(0x00433FC0, Hud_ForwardWidgetPairArguments, "auto");
