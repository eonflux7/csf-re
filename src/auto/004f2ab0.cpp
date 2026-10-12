#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, HudStringShow_ProcessPair, 0x004F23A0,
         (int a1, int a2));

static int __fastcall HudStringShow_ForwardPairArguments(
    void* context, void* /*edx*/, int firstArgument, int secondArgument)
{
    // The disassembly forwards four values despite the supplied two-argument
    // callee prototype.
    using ForwardedCall = int(__cdecl*)(int, int, std::uintptr_t, int);
    return reinterpret_cast<ForwardedCall>(HudStringShow_ProcessPair)(
        firstArgument, secondArgument,
        reinterpret_cast<std::uintptr_t>(context), secondArgument);
}

HOOK(0x004F2AB0, HudStringShow_ForwardPairArguments, "auto");
