#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee at 0x00559910 is __cdecl. The original pushes four values, so the
// callee receives (a1, a2, incoming ECX, a2). The caller cleans up with add esp, 0x10.
CSF_FUNC(int, __cdecl, MpRules_ProcessBombSettings, 0x00559910,
         (int a1, int a2, std::uintptr_t context, int repeatedArg2));

// 0x00559A20: __stdcall thunk with two stack arguments (ret 8). ECX arrives as the
// first __fastcall parameter and is forwarded as the third callee argument.
static int __fastcall MpRules_ForwardBombArguments(
    void* context, void* /*edx*/, int a1, int a2)
{
    return MpRules_ProcessBombSettings(
        a1,
        a2,
        reinterpret_cast<std::uintptr_t>(context),
        a2);
}

HOOK(0x00559A20, MpRules_ForwardBombArguments, "auto");
