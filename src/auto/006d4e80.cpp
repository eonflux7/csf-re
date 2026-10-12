#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x006D4B40 is __cdecl. The original pushes four dwords (a2, ECX, a2, a1),
// so the callee sees (a1, a2, ECX, a2). The packet's bind line lists two parameters,
// but the original passes four stack values, so all four are bound here.
// Returns char (the original uses the al result).
CSF_FUNC(char, __cdecl, ClumpDict_ProcessPair6D4B40, 0x006D4B40,
         (int a1, int a2, int a3, int a4));

// 0x006D4E80: __stdcall, callee-cleaned (ret 8). The incoming ECX is forwarded as the
// third callee argument, so this is bound __fastcall (ecx = context, stack args popped by callee).
static int __fastcall ForwardPairWithIncomingEcx(
    void* context, void* /*edx*/, int firstArgument, int secondArgument)
{
    return ClumpDict_ProcessPair6D4B40(
        firstArgument,
        secondArgument,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(context)),
        secondArgument);
}

HOOK(0x006D4E80, ForwardPairWithIncomingEcx, "auto");
