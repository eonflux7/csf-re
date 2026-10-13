#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x0078BE10 is __cdecl and takes four values: the first stack
// argument, the second stack argument, the incoming ECX (context), and the
// second argument repeated.
CSF_FUNC(int, __cdecl, NetPerfCounters_ProcessPair78BE10, 0x0078BE10,
         (int a1, int a2, int a3, int a4));

// 0x0078C2B0: __thiscall thunk, callee-cleaned (ret 8). The original pushes
// (a2, ECX, a2, a1) so the callee sees (a1, a2, ECX, a2). ECX arrives as the
// first __fastcall parameter and the two stack arguments are callee-cleaned.
static int __fastcall ForwardDataSetPairWithContext(
    void* context, void* /*edx*/, int firstArgument, int secondArgument)
{
    return NetPerfCounters_ProcessPair78BE10(
        firstArgument,
        secondArgument,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(context)),
        secondArgument);
}

HOOK(0x0078C2B0, ForwardDataSetPairWithContext, "auto");
