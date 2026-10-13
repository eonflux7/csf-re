#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x004F01A0 is __cdecl (caller cleans 0x10). It receives four values:
// the first stack argument, the second stack argument, the incoming ECX (context),
// and the second argument repeated.
CSF_FUNC(int, __cdecl, TextDatabase_ProcessForwardedArgs, 0x004F01A0,
         (int a1, int a2, int a3, int a4));

// 0x004F0340: __thiscall thunk, callee-cleaned (ret 8). The original pushes
// (a2, ECX, a2, a1) so the callee sees (a1, a2, ECX, a2). ECX arrives as the
// first __fastcall parameter and the two stack arguments are callee-cleaned.
static int __fastcall TextDatabase_ForwardPairWithContext(
    void* context, void* /*edx*/, int firstArgument, int secondArgument)
{
    return TextDatabase_ProcessForwardedArgs(
        firstArgument,
        secondArgument,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(context)),
        secondArgument);
}

HOOK(0x004F0340, TextDatabase_ForwardPairWithContext, "auto");
